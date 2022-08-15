/**
 * Copyright © 2022 Université de Genève, LMU Munich - Faculty of Physics, IAP-CNRS/Sorbonne Université
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

#ifndef SOURCEXTRACTORPLUSPLUS_PARTITION_H
#define SOURCEXTRACTORPLUSPLUS_PARTITION_H

#include "SEFramework/Pipeline/Partition.h"
#include "SEPythonModule/Context.h"
#include <boost/python/object.hpp>

namespace SourceXPy {

class Partition : public SourceXtractor::PipelineReceiver<SourceXtractor::SourceInterface> {
public:
  explicit Partition(ContextPtr context);
  ~Partition() override = default;

  std::string repr() const;

  void setNextStage(const boost::python::object& callback);

  void receiveSource(std::unique_ptr<SourceXtractor::SourceInterface> source) override;

  void receiveProcessSignal(const SourceXtractor::ProcessSourcesEvent& event) override;

  void call(const boost::python::object& obj) const;

private:
  ContextPtr                                 m_context;
  std::shared_ptr<SourceXtractor::Partition> m_partition;
};

}  // namespace SourceXPy

#endif  // SOURCEXTRACTORPLUSPLUS_PARTITION_H
