/**
 * @file src/lib/SegmentationConfig.cpp
 * @date 06/08/16
 * @author mschefer
 */

#include <boost/algorithm/string.hpp>

#include "ElementsKernel/Exception.h"

#include "Configuration/ConfigManager.h"
#include "Configuration/CatalogConfig.h"

#include "SEImplementation/Configuration/SegmentationConfig.h"

using namespace Euclid::Configuration;
namespace po = boost::program_options;

namespace SExtractor {

static const std::string SEGMENTATION_ALGORITHM {"segmentation-algorithm" };
static const std::string SEGMENTATION_DISABLE_FILTERING {"segmentation-disable-filtering" };

SegmentationConfig::SegmentationConfig(long manager_id) : Configuration(manager_id),
    m_selected_algorithm(Algorithm::UNKNOWN), m_filtering_enabled(true) {
}

std::map<std::string, Configuration::OptionDescriptionList> SegmentationConfig::getProgramOptions() {
  return { {"Detection image", {
      {SEGMENTATION_ALGORITHM.c_str(), po::value<std::string>()->default_value("LUTZ"),
          "Segmentation algorithm to be used. Currently LUTZ is the only choice"},
      {SEGMENTATION_DISABLE_FILTERING.c_str(), po::bool_switch(),
          "Disables filtering"},
  }}};
}

void SegmentationConfig::preInitialize(const UserValues& args) {
  auto& algorithm_name = args.at(SEGMENTATION_ALGORITHM).as<std::string>();
  if (algorithm_name != "LUTZ") {
    throw Elements::Exception() << "Unknown segmentation algorithm : " << algorithm_name;
  }
  if (args.at(SEGMENTATION_DISABLE_FILTERING).as<bool>()) {
    m_filtering_enabled = false;
  }
}

void SegmentationConfig::initialize(const UserValues&) {
  m_selected_algorithm = Algorithm::LUTZ;
}

} // SExtractor namespace
