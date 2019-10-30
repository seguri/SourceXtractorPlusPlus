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
 * ImageChunk.h
 *
 *  Created on: Aug 30, 2017
 *      Author: mschefer
 */

#ifndef _SEFRAMEWORK_IMAGE_IMAGECHUNK_H_
#define _SEFRAMEWORK_IMAGE_IMAGECHUNK_H_

#include <cassert>

#include "SEFramework/Image/Image.h"

namespace SourceXtractor {

template <typename T>
class ImageChunk : public Image<T>, public std::enable_shared_from_this<ImageChunk<T>> {
protected:
  ImageChunk(const T* data, int width, int height, int stride, std::shared_ptr<const Image<T>> image = nullptr) :
      m_data(data),
      m_stride(stride),
      m_width(width),
      m_height(height),
      m_image(image) {}

public:
  static std::shared_ptr<ImageChunk<T>> create(const T* data, int width, int height,
      int stride, std::shared_ptr<const Image<T>> image = nullptr) {
    return std::shared_ptr<ImageChunk<T>>(new ImageChunk<T>(data, width, height, stride, image));
  }

  virtual ~ImageChunk() {
  }

  std::string getRepr() const override {
    return "ImageChunk<" + std::to_string(m_width) + "," + std::to_string(m_height) + ">(" + m_image->getRepr() + ")";
  }

  /// Returns the value of the pixel with the coordinates (x,y)
  T getValue(int x, int y) const final {
    assert(x >= 0 && y >=0 && x < m_width && y < m_height);
    return m_data[x + y * m_stride];
  }

  /// Returns the width of the image chunk in pixels
  int getWidth() const final {
    return m_width;
  }

  /// Returns the height of the image chunk in pixels
  int getHeight() const final {
    return m_height;
  }

  virtual std::shared_ptr<ImageChunk<T>> getChunk(int x, int y, int width, int height) const override {
    return create(&m_data[x + y * m_stride], width, height, m_stride, this->shared_from_this());
  }

protected:

  // We use this in cases when the ImageChunk subclass allocates a buffer to store the chunk after construction
  // of the base ImageChunk
  void setDataPtr(const T* data) {
    m_data = data;
  }

private:
  const T* m_data;
  int m_stride;
  int m_width, m_height;

  std::shared_ptr<const Image<T>> m_image;
};

template <typename T>
class UniversalImageChunk : public ImageChunk<T> {

protected:
  UniversalImageChunk(std::shared_ptr<const Image<T>> image, int x, int y, int width, int height) :
      ImageChunk<T>(nullptr, width, height, width),
      m_chunk_vector(width * height) {

    this->setDataPtr(&m_chunk_vector[0]);

    for (int cy=0; cy < height; cy++) {
      for (int cx=0; cx < width; cx++) {
        m_chunk_vector[cx + cy * width] = image->getValue(x + cx, y + cy);
      }
    }
  }

  UniversalImageChunk(std::vector<T> &&data, int width, int height):
    ImageChunk<T>(nullptr, width, height, width), m_chunk_vector(std::move(data))
  {
    assert(static_cast<int>(m_chunk_vector.size()) == width * height);
    this->setDataPtr(&m_chunk_vector[0]);
  }

public:
  template <typename... Args>
  static std::shared_ptr<UniversalImageChunk<T>> create(Args&&... args) {
    return std::shared_ptr<UniversalImageChunk<T>>(new UniversalImageChunk<T>(std::forward<Args>(args)...));
  }

  virtual ~UniversalImageChunk() {
  }

private:
  std::vector<T> m_chunk_vector;

};


}

#endif /* _SEFRAMEWORK_IMAGE_IMAGECHUNK_H_ */
