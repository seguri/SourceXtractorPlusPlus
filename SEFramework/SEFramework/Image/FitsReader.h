/**
 * @file SEFramework/Image/FitsReader.h
 * @date 06/14/16
 * @author nikoapos
 */

#ifndef _SEFRAMEWORK_IMAGE_FITSREADER_H
#define _SEFRAMEWORK_IMAGE_FITSREADER_H

#include <CCfits/CCfits>
#include "SEFramework/Image/Image.h"
#include "SEFramework/Image/VectorImage.h"

namespace SExtractor {

/**
 * @class FitsReader
 * @brief
 *
 */
template <typename T>
class FitsReader {

public:

  /**
   * @brief Destructor
   */
  virtual ~FitsReader() = default;

  static std::shared_ptr<Image<T>> readFile(const std::string& filename) {
    CCfits::FITS in_file {filename, CCfits::Read, true};
    auto& image = in_file.pHDU();
    image.readAllKeys();
    std::valarray<T> data {};
    image.read(data);
    std::vector<T> data_v {std::begin(data), std::end(data)};
    int width = image.axis(0);
    int height = image.axis(1);
    return VectorImage<T>::create(width, height, data_v);
  }

}; /* End of FitsReader class */

} /* namespace SExtractor */


#endif
