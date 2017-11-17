/*
 * Background
 *
 *  Created on: Oct 11, 2016
 *      Author: mschefer
 */

#ifndef _SEIMPLEMENTATION_BACKGROUND_SIMPLEBACKGROUNDANALYZER_H_
#define _SEIMPLEMENTATION_BACKGROUND_SIMPLEBACKGROUNDANALYZER_H_

#include "SEFramework/Frame/Frame.h"
#include "SEFramework/Image/Image.h"
#include "SEFramework/Background/BackgroundAnalyzer.h"

namespace SExtractor {

class SimpleBackgroundLevelAnalyzer : public BackgroundAnalyzer {
public:

  virtual ~SimpleBackgroundLevelAnalyzer() = default;

  //void analyzeBackground(std::shared_ptr<DetectionImageFrame> frame) const override;

  std::shared_ptr<Image<SeFloat>> analyzeBackground(
      std::shared_ptr<DetectionImage> image, std::shared_ptr<WeightImage> variance_map, std::shared_ptr<Image<unsigned char>> mask) const override;
  virtual void setParameters(std::string cell_size, std::string smoothing_box) override {};

private:
  SeFloat getMedian(std::shared_ptr<DetectionImageFrame> frame) const;
};

}

#endif /* _SEIMPLEMENTATION_BACKGROUND_SIMPLEBACKGROUNDANALYZER_H_ */
