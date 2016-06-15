/** 
 * @file Example2.cpp
 * @date September 2, 2015
 * @author Nikolaos Apostolakos
 */

#include <iostream>
#include <random>
#include <functional>
#include <vector>
#include "ElementsKernel/PathSearch.h"
#include "ModelFitting/Parameters/ManualParameter.h"
#include "ModelFitting/Models/ConstantModel.h"
#include "ModelFitting/Models/PointModel.h"
#include "ModelFitting/Models/ExtendedModel.h"
#include "ModelFitting/Models/CircularlySymmetricModelComponent.h"
#include "ModelFitting/Models/OnlySmooth.h"
#include "ModelFitting/Models/AutoSharp.h"
#include "ModelFitting/Models/FrameModel.h"
#include "ModelFitting/Image/OpenCvPsf.h"
#include "ModelFitting/Image/OpenCvMatImageTraits.h"
#include "utils.h"

using namespace std;
using namespace ModelFitting;

// This example demonstrates how to create a FrameModel with a single constant
// model and a random number of point and extended models.

int main() {
  
  // First we create the random number generators we will use. We create one that
  // creates random intengers in the range [10,20] and one that creates random
  // doubles in the range [0.,1.]
  random_device rd;
  default_random_engine generator {rd()};
  uniform_int_distribution<int> int_distribution {10, 20};
  auto rand_int = bind(int_distribution, generator);
  uniform_real_distribution<double> double_distribution {0., 1.};
  auto rand_double = bind(double_distribution, generator);
  
  // To create a FrameModel we need to specify its size and its pixel scale
  double pixel_scale = .1;
  std::size_t width = 401;
  std::size_t height = 201;
  
  // The FrameModel can contain any number of constant, point and extended
  // models. Here we construct the vectors which will keep these models.
  vector<ConstantModel> constant_models {};
  vector<PointModel> point_models {};
  vector<ExtendedModel> extended_models {};
  
  // We will use a single constant model, which simulates the background. The
  // constant model gets a single parameter, its value.
  ManualParameter back_value {1E-6};
  constant_models.emplace_back(back_value);
  
  // We add a random number of random point sources
  for (auto point_no=rand_int(); point_no>0; --point_no) {
    // The point sources get three parameters, its position on the frame (in
    // pixels) and its value
    ManualParameter x {rand_double() * width};
    ManualParameter y {rand_double() * height};
    ManualParameter value {rand_double() * 5.};
    point_models.emplace_back(x, y, value);
  }
  
  // We add a random number of random extended sources
  for (auto ext_no=rand_int(); ext_no>0; --ext_no) {
    // The extended source gets the following parameters:
    // - Its model components
    std::vector<std::unique_ptr<ModelComponent>> component_list {};
    // - Its scale factors
    ManualParameter x_scale {(rand_double() * .4) + .1};
    ManualParameter y_scale {(rand_double() * .4) + .1};
    // - Its rotation angle
    ManualParameter rot_angle {rand_double() * M_PI};
    // - Its position on the frame
    ManualParameter x {rand_double() * width};
    ManualParameter y {rand_double() * height};
    // - Its size in arcsec (??? from detection step ???)
    double width = 15. * std::max(x_scale.getValue(), y_scale.getValue());
    double height = 15. * std::max(x_scale.getValue(), y_scale.getValue());
    
    // We add two model components, an exponential:
    ManualParameter exp_i0 {rand_double() * 1E2 + .1};
    ManualParameter exp_n {1.};
    ManualParameter exp_k {1.};
    auto exp_reg_man = make_unique<OnlySmooth>();
    auto exp = make_unique<SersicModelComponent>(move(exp_reg_man), exp_i0, exp_n, exp_k);
    component_list.emplace_back(move(exp));
    // and a De Vacouleurs
    ManualParameter dev_i0 {rand_double() * 5E3 + 1.};
    ManualParameter dev_n {4.};
    ManualParameter dev_k {7.66924944};
    auto dev_reg_man = make_unique<AutoSharp>();
    auto dev = make_unique<SersicModelComponent>(move(dev_reg_man), dev_i0, dev_n, dev_k);
    
    // Finally we create the extended model and we add it to the list
    extended_models.emplace_back(std::move(component_list), x_scale, y_scale, 
                                 rot_angle, width, height, x, y);
  }
  
  // The FrameModel needs a PSF so it can convolve its models. The type of the
  // PSF is abstracted as a template parameter. The following creates an instance
  // of OpenCvPsf (which is a concrete implementation of the interface) from a
  // FITS file.
  // Note that we use the PathSearch tool of elements to locate the file in the
  // auxdir directory
  auto psf_path = Elements::pathSearchInEnvVariable("psf.fits", "ELEMENTS_AUX_PATH");
  auto psf = readPsf(psf_path[0].string());
  
  // Finally we can create the FrameModel. Note that we must include the file
  // ModelFitting/Image/OpenCvMatImageTraits.h to enable the ImageTraits for
  // the cv::Mat type.
  FrameModel<OpenCvPsf, cv::Mat> frame_model {
        pixel_scale, width, height, move(constant_models),
        move(point_models), move(extended_models), move(psf)
  };
                                
  //
  // Model demonstration
  //
  // As an example of using the FrameModel we get its image and we store it in
  // a file
  auto image = frame_model.getImage();
  writeToFits(image, "example2.fits");
  
}