/* 
 * File:   OutputRegistry.cpp
 * Author: nikoapos
 *
 * Created on July 28, 2016, 7:12 PM
 */

#include <algorithm>

#include "ElementsKernel/Exception.h"

#include "SEFramework/Output/OutputRegistry.h"

#include <iostream>

using namespace Euclid::Table;

namespace SExtractor {

auto OutputRegistry::getSourceToRowConverter(const std::vector<std::string>& enabled_properties) -> SourceToRowConverter {
  std::vector<std::type_index> out_prop_list {};
  for (auto& prop : enabled_properties) {
    if (m_output_properties.count(prop) == 0) {
      throw Elements::Exception() << "Unknown output property " << prop;
    }
    auto matching_properties = m_output_properties.equal_range(prop);
    for (auto i = matching_properties.first; i != matching_properties.second; ++i) {
      if (std::find(out_prop_list.begin(), out_prop_list.end(), i->second) == out_prop_list.end()) {
        out_prop_list.emplace_back(i->second);
      }
    }
  }
  return [this, out_prop_list](const SourceInterface& source) {
    std::vector<ColumnInfo::info_type> info_list {};
    std::vector<Row::cell_type> cell_values {};
    for (const auto& property : out_prop_list) {
      for (const auto& name : m_property_to_names_map.at(property)) {
        auto& col_info = m_name_to_col_info_map.at(name);
        info_list.emplace_back(name, m_name_to_converter_map.at(name).first,
                               col_info.unit, col_info.description);
        cell_values.emplace_back(m_name_to_converter_map.at(name).second(source));
      }
    }
    return Row {std::move(cell_values), std::make_shared<ColumnInfo>(move(info_list))};
  };
}

void OutputRegistry::printPropertyColumnMap(const std::vector<std::string>& properties) {
  std::set<std::string> properties_set {properties.begin(), properties.end()};
  for (auto& prop : m_output_properties) {
    if (!properties.empty() && properties_set.find(prop.first) == properties_set.end()) {
      continue;
    }
    std::cout << prop.first << ":\n";
    for (auto& col : m_property_to_names_map.at(prop.second)) {
      std::cout << "  - " << col;
      auto& info = m_name_to_col_info_map.at(col);
      if (info.description != "") {
        std::cout << " : " << info.description;
      }
      if (info.unit != "") {
        std::cout << " (" << info.unit << ")";
      }
      std::cout << '\n';
    }
  }
}


}
