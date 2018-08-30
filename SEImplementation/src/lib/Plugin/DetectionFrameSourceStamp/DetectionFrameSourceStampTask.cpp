/**
 * @file src/lib/Task/DetectionFrameSourceStampTask.cpp
 * @date 06/16/16
 * @author mschefer
 */

#include <mutex>

#include "SEFramework/Image/Image.h"
#include "SEFramework/Image/VectorImage.h"
#include "SEFramework/Property/DetectionFrame.h"
#include "SEImplementation/Plugin/PixelBoundaries/PixelBoundaries.h"

#include "SEImplementation/Plugin/DetectionFrameSourceStamp/DetectionFrameSourceStamp.h"
#include "SEImplementation/Plugin/DetectionFrameSourceStamp/DetectionFrameSourceStampTask.h"

#include "SEImplementation/Measurement/MultithreadedMeasurement.h"

namespace SExtractor {

void DetectionFrameSourceStampTask::computeProperties(SourceInterface& source) const {
  std::lock_guard<std::recursive_mutex> lock(MultithreadedMeasurement::g_global_mutex);

  auto detection_frame = source.getProperty<DetectionFrame>().getFrame()->getSubtractedImage();

  const auto& boundaries = source.getProperty<PixelBoundaries>();
  const auto& min = boundaries.getMin();
  const auto& max = boundaries.getMax();

  auto width = max.m_x - min.m_x +1;
  auto height = max.m_y - min.m_y + 1;
  std::vector<DetectionImage::PixelType> data (width * height);
  for (auto x = min.m_x; x <= max.m_x; ++x) {
    for (auto y = min.m_y; y <= max.m_y; ++y) {
      data[(x-min.m_x) + (y-min.m_y) * width] = detection_frame->getValue(x, y);
    }
  }
  std::shared_ptr<DetectionImage> stamp = VectorImage<DetectionImage::PixelType>::create(width, height, data);
  source.setProperty<DetectionFrameSourceStamp>(stamp);
}

} // SEImplementation namespace



