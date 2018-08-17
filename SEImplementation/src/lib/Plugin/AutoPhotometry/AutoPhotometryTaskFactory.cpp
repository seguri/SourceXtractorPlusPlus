/*
 * AutoPhotometryTaskFactory.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: mkuemmel@usm.lmu.de
 */

#include <iostream>
#include <sstream>

#include "SEFramework/Property/PropertyId.h"
#include "SEFramework/Task/Task.h"

// TEMP
#include "SEFramework/Image/FitsWriter.h"
#include "SEImplementation/Configuration/DetectionImageConfig.h"
//#include "SEFramework/Image/VectorImage.h"
//#include "SEFramework/Image/FitsWriter.h"
//#include "SEFramework/Image/WriteableImage.h"

#include "SEImplementation/Configuration/MagnitudeConfig.h"

#include "SEImplementation/Plugin/AutoPhotometry/AutoPhotometry.h"
#include "SEImplementation/Plugin/AutoPhotometry/AutoPhotometryConfig.h"
#include "SEImplementation/Plugin/AutoPhotometry/AutoPhotometryTask.h"
#include "SEImplementation/Plugin/AutoPhotometry/AutoPhotometryTaskFactory.h"

namespace SExtractor {

void AutoPhotometryTaskFactory::reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const {
  manager.registerConfiguration<MagnitudeConfig>();
  manager.registerConfiguration<AutoPhotometryConfig>();

  // TEMP
  manager.registerConfiguration<DetectionImageConfig>();

}

void AutoPhotometryTaskFactory::configure(Euclid::Configuration::ConfigManager& manager) {
  m_magnitude_zero_point = manager.getConfiguration<MagnitudeConfig>().getMagnitudeZeroPoint();
  m_kron_factor = manager.getConfiguration<AutoPhotometryConfig>().getAutoKronFactor();
  m_kron_minrad = manager.getConfiguration<AutoPhotometryConfig>().getAutoKronMinrad();
  m_kron_estimation = manager.getConfiguration<AutoPhotometryConfig>().getAutoKronEstimation();
  m_kron_measurement = manager.getConfiguration<AutoPhotometryConfig>().getAutoKronMeasurement();

  // TEMP
  auto detection_image = manager.getConfiguration<DetectionImageConfig>().getDetectionImage();
  //m_tmp_check_image = VectorImage<int>::create(detection_image->getWidth(), detection_image->getHeight());
  std::string bbb("tmp.fits");
  m_tmp_check_image = FitsWriter::newImage<unsigned int>(bbb, detection_image->getWidth(), detection_image->getHeight());
}

std::shared_ptr<Task> AutoPhotometryTaskFactory::createTask(const PropertyId& property_id) const {
  if (property_id == PropertyId::create<AutoPhotometry>()) {
    return std::make_shared<AutoPhotometryTask>(m_magnitude_zero_point, m_kron_factor, m_kron_minrad, m_kron_estimation, m_kron_measurement, true, m_tmp_check_image);
  } else {
    return nullptr;
  }
}

}

