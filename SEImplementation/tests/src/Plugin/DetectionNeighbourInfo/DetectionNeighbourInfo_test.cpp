/*
 *  DetectionNeighbourInfo_test.cpp
 *
 *  Created on: Oct 02, 2018
 *      Author: Alejandro Alvarez Ayllon
 */

#include <boost/test/unit_test.hpp>
#include "SEFramework/Property/DetectionFrame.h"
#include "SEFramework/Image/VectorImage.h"
#include "SEFramework/Source/SimpleSource.h"
#include "SEImplementation/Property/PixelCoordinateList.h"
#include "SEImplementation/Plugin/PixelBoundaries/PixelBoundaries.h"
#include "SEImplementation/Plugin/DetectionNeighbourInfo/DetectionNeighbourInfo.h"
#include "SEImplementation/Plugin/DetectionNeighbourInfo/DetectionNeighbourInfoTask.h"

#include "SEFramework/tests/src/Image/CompareImages.h"

using namespace SExtractor;


struct DetectionNeighbourInfo_Fixture {
  // Two "sources", once centered at 2,2 (42), and another one
  // at 4,2 (24). Tests will run on the centered one, whose pixels
  // are identified by detection_coordinates.
  std::shared_ptr<Image<float>> detection_image = VectorImage<SeFloat>::create(5, 5, std::vector<float>{
    0.,  0.,  0.,  0.,  8.,
    0.,  6., 12., 12., 24.,
    0., 12., 42., 16.,  8.,
    0.,  6., 12.,  6.,  0.,
    0.,  0.,  0.,  0.,  0.,
  });

  std::shared_ptr<Image<float>> variance_map = ConstantImage<float>::create(5, 5, .0001);

  std::vector<PixelCoordinate> detection_pixel_list{
    {1, 1},
    {2, 1},
    {1, 2},
    {2, 2},
    {3, 2},
    {1, 3},
    {2, 3},
    {3, 3},
  };

  SimpleSource source;

  DetectionNeighbourInfo_Fixture() {
    source.setProperty<PixelCoordinateList>(detection_pixel_list);
    source.setProperty<PixelBoundaries>(1, 1, 3, 3);
    source.setProperty<DetectionFrame>(std::make_shared<DetectionImageFrame>(
      detection_image, variance_map, 0., nullptr, 0., 60000, 0
    ));
  }
};

BOOST_AUTO_TEST_SUITE(NeighbourInfo_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(OneToOne_test, DetectionNeighbourInfo_Fixture) {
  DetectionNeighbourInfoTask task;

  task.computeProperties(source);

  auto neighbour_info = source.getProperty<DetectionNeighbourInfo>().getImage();

  auto expected = VectorImage<int>::create(3, 3, std::vector<int>{
    0, 0, 1,
    0, 0, 0,
    0, 0, 0,
  });

  BOOST_CHECK(compareImages(expected, neighbour_info));
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
