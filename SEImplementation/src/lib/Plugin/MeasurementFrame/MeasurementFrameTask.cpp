/** Copyright © 2019 Université de Genève, LMU Munich - Faculty of Physics, IAP-CNRS/Sorbonne Université
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
/*
 * MeasurementFrameTask.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: mschefer
 */

#include "SEFramework/Property/DetectionFrame.h"
#include "SEImplementation/Plugin/MeasurementFrame/MeasurementFrame.h"
#include "SEImplementation/Plugin/MeasurementFrame/MeasurementFrameTask.h"

namespace SExtractor {

void MeasurementFrameTask::computeProperties(SourceInterface& source) const {
  source.setIndexedProperty<MeasurementFrame>(m_instance, m_measurement_frame);
}

void DefaultMeasurementFrameTask::computeProperties(SourceInterface& source) const {
  const auto& detection_frame = source.getProperty<DetectionFrame>();

  source.setIndexedProperty<MeasurementFrame>(
      m_instance,
      detection_frame.getFrame());
}

}





