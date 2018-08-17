/*
 * MultiThreadedMeasurement.cpp
 *
 *  Created on: May 23, 2018
 *      Author: mschefer
 */

#include <iostream>
#include <chrono>

#include "SEImplementation/Plugin/SourceIDs/SourceID.h"

#include "SEImplementation/Measurement/MultithreadedMeasurement.h"

using namespace SExtractor;

std::recursive_mutex MultithreadedMeasurement::g_global_mutex;

void MultithreadedMeasurement::startThreads() {
  // Start worker threads
  m_active_threads = m_worker_threads_nb;
  for (int i=0; i<m_worker_threads_nb; i++) {
    m_worker_threads.emplace_back(std::make_shared<std::thread>(workerThreadStatic, this));
  }

  // Start output thread
  m_output_thread = std::make_shared<std::thread>(outputThreadStatic, this);
}

void MultithreadedMeasurement::waitForThreads() {
  // set flag to indicate no new input will be coming
  {
    std::unique_lock<std::mutex> input_lock(m_input_queue_mutex);
    m_input_done = true;
    m_new_input.notify_all();
  }

  // Wait for all threads to finish
  for (int i=0; i<m_worker_threads_nb; i++) {
    m_worker_threads[i]->join();
  }
  m_output_thread->join();
}

void MultithreadedMeasurement::handleMessage(const std::shared_ptr<SourceGroupInterface>& source_group) {
  std::unique_lock<std::mutex> input_lock(m_input_queue_mutex);

  //Force computation of SourceID here, where the order is still deterministic
  for (auto& source : *source_group) {
    source.getProperty<SourceID>();
  }

  // put the new SourceGroup into the input queue
  m_input_queue.emplace_back(m_group_counter++, source_group);

  // notify one worker thread that there is an available input
  m_new_input.notify_one();
}

void MultithreadedMeasurement::workerThreadStatic(MultithreadedMeasurement* measurement) {
  measurement->workerThreadLoop();
}

void MultithreadedMeasurement::outputThreadStatic(MultithreadedMeasurement* measurement) {
  measurement->outputThreadLoop();
}

void MultithreadedMeasurement::workerThreadLoop() {
  while (true) {
    int order_number;
    std::shared_ptr<SourceGroupInterface> source_group;
    {
      std::unique_lock<std::mutex> input_lock(m_input_queue_mutex);

      // We should end the thread once we're done with all input
      if (m_input_done && m_input_queue.empty()) {
        break;
      }

      // If the queue is empty but we expect more data, wait
      if (m_input_queue.empty()) {
        m_new_input.wait_for(input_lock, std::chrono::milliseconds(100));
        continue;
      }

      order_number = m_input_queue.front().first;
      source_group = m_input_queue.front().second;
      m_input_queue.pop_front();
    }

    // Trigger measurements
    for (auto& source : *source_group) {
      m_source_to_row(source);
    }

    {
      std::unique_lock<std::mutex> output_lock(m_output_queue_mutex);
      m_output_queue[order_number] = source_group;
      source_group = nullptr;
      m_new_output.notify_one();
    }
  }

  // Before ending the thread, decrement active threads counter
  {
    std::unique_lock<std::mutex> active_threads_lock(m_active_threads_mutex);
    m_active_threads--;
  }
}

void MultithreadedMeasurement::outputThreadLoop() {
  while (true) {
    {
      std::unique_lock<std::mutex> output_lock(m_output_queue_mutex);

      // Wait for something in the output queue
      if (m_output_queue.empty() || m_output_queue.begin()->first != m_output_counter) {
        m_new_output.wait_for(output_lock, std::chrono::milliseconds(100));
      }

      // Process the output queue
      while(!m_output_queue.empty() && m_output_queue.begin()->first == m_output_counter) {
        notifyObservers(m_output_queue.begin()->second);
        m_output_queue.erase(m_output_counter++);
      }
    }

    {
      std::unique_lock<std::mutex> output_lock(m_output_queue_mutex);
      std::unique_lock<std::mutex> active_threads_lock(m_active_threads_mutex);
      if (m_active_threads <= 0 && m_output_queue.empty()) {
        break;
      }
    }
  }
}

