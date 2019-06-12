/*
 * MoffatModelFittingPlugin.cpp
 *
 *  Created on: May 2, 2017
 *      Author: mschefer
 */

#include "SEImplementation/Plugin/MoffatModelFitting/MoffatModelFitting.h"
#include "SEImplementation/Plugin/MoffatModelFitting/MoffatModelFittingPlugin.h"
#include "SEImplementation/Plugin/MoffatModelFitting/MoffatModelFittingTaskFactory.h"
#include "SEFramework/Plugin/StaticPlugin.h"


namespace SExtractor {

static StaticPlugin<MoffatModelFittingPlugin> simple_modelfitting_plugin;

void MoffatModelFittingPlugin::registerPlugin(PluginAPI& plugin_api) {
  plugin_api.getTaskFactoryRegistry().registerTaskFactory<MoffatModelFittingTaskFactory, MoffatModelFitting>();

  plugin_api.getOutputRegistry().registerColumnConverter<MoffatModelFitting, double>(
          "smf_x",
          [](const MoffatModelFitting& prop) {
            return prop.getX() + 1.0;
          },
          "[pixel]",
          "X-position of the Moffat fit"
  );

  plugin_api.getOutputRegistry().registerColumnConverter<MoffatModelFitting, double>(
          "smf_y",
          [](const MoffatModelFitting& prop) {
            return prop.getY() + 1.0;
          },
          "[pixel]",
          "Y-position of the Moffat fit"
  );

  plugin_api.getOutputRegistry().registerColumnConverter<MoffatModelFitting, int>(
          "smf_iter",
          [](const MoffatModelFitting& prop){
            return prop.getIterations();
          },
          "",
          "Number of iterations in the Moffat fitting"
  );

  plugin_api.getOutputRegistry().enableOutput<MoffatModelFitting>("MoffatModelFitting");
}

std::string MoffatModelFittingPlugin::getIdString() const {
  return "MoffatModelFitting";
}

}