/**
 * @file CompareImages.h
 * @date 11/09/18
 * @author Alejandro Alvarez Ayllon
 */


#ifndef _COMPAREIMAGES_H
#define _COMPAREIMAGES_H

#include "SEFramework/Image/Image.h"

namespace SExtractor {

template <typename T, typename U>
boost::test_tools::predicate_result compareImages(
  const T &ref, const U &val) {
  boost::test_tools::predicate_result res(true);

  for (int x = 0; x < ref->getWidth(); ++x) {
    for (int y = 0; y < ref->getHeight(); ++y) {
      auto expected = ref->getValue(x, y);
      auto value = val->getValue(x, y);
      if (expected != value) {
        res = false;
        res.message() << "Not matching values at position " << x << "," << y
                      << ": " << expected << " != " << value << "\n";
      }
    }
  }

  return res;
}

} // end SExtractor

#endif // _COMPAREIMAGES_H