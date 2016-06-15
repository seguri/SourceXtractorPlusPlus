/**
 * @file SEFramework/Image/VectorImage.h
 * @date 06/14/16
 * @author nikoapos
 */

#ifndef _SEFRAMEWORK_IMAGE_VECTORIMAGE_H
#define _SEFRAMEWORK_IMAGE_VECTORIMAGE_H

#include <vector>
#include "SEFramework/Image/Image.h"

namespace SExtractor {

/**
 * @class VectorImage
 * @brief Image implementation which keeps the pixel values in memory
 * 
 * @details
 * The pixel values are kept in a vector. The first element of the vector is
 * the pixel with coordinate (0,0) and the following elements have increasing
 * x and y indices, with the x index (in the width direction) increasing more
 * rapidly. This means the order of the vector elements should be:
 * 
 * (0,0), (1,0), (2,0), ..., (width-1,0), (0,1), (1,1), ..., (width-1,height-1)
 * 
 * @tparam T the type of the pixel values
 */
template <typename T>
class VectorImage : public Image<T> {

public:

  VectorImage(int m_width, int m_height, std::vector<T> m_data) :
          m_width(m_width), m_height(m_height), m_data(std::move(m_data)) {
  }
  
  int getHeight() const override {
    return m_height;
  }
  
  int getWidth() const override {
    return m_width;
  }

  T getValue(int x, int y) const override {
    return m_data[x + y * m_width];
  }
  
  const std::vector<T>& getData() const {
    return m_data;
  }

  /**
   * @brief Destructor
   */
  virtual ~VectorImage() = default;


private:

  int m_width;
  int m_height;
  std::vector<T> m_data;

}; /* End of VectorImage class */

} /* namespace SExtractor */


#endif