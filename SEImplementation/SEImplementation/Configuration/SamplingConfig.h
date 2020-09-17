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

#ifndef _SEIMPLEMENTATION_CONFIGURATION_SAMPLINGCONFIG_H_
#define _SEIMPLEMENTATION_CONFIGURATION_SAMPLINGCONFIG_H_

#include "Configuration/Configuration.h"

namespace SourceXtractor {

class SamplingConfig : public Euclid::Configuration::Configuration {

public:

  enum class SamplingMethod {
    LEGACY=0,
    ADAPTIVE=1,
    GRID=2,
    STOCHASTIC=3,
    NEW=4,
  };

  SamplingConfig(long manager_id);

  virtual ~SamplingConfig() = default;

  std::map<std::string, OptionDescriptionList> getProgramOptions() override;

  void preInitialize(const UserValues& args) override;

  void initialize(const UserValues& args) override;

  SamplingMethod getSamplingMethod() const {
    return m_sampling_method;
  }

  int getSampleNb() const {
    return m_sample_nb;
  }

  double getAdaptiveTargetPrecision() const {
    return m_adaptive_target;
  }

  double getScaleFactor() const {
    return m_scale_factor;
  }

  bool getRenormalize() const {
    return m_renormalize;
  }

private:
  SamplingMethod m_sampling_method;
  int m_sample_nb;
  double m_adaptive_target;
  double m_scale_factor;
  bool m_renormalize;
};


} /* namespace SourceXtractor */

#endif /* _SEIMPLEMENTATION_CONFIGURATION_SAMPLINGCONFIG_H_ */
