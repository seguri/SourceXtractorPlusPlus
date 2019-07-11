/*
 * VariablePsf.h
 *
 *  Created on: Jun 25, 2018
 *      Author:
 */

#ifndef _SEIMPLEMENTATION_PSF_VARIABLEPSFSTACK_H_
#define _SEIMPLEMENTATION_PSF_VARIABLEPSFSTACK_H_

#include <CCfits/CCfits>
#include <SEFramework/Psf/VariablePsf.h>
#include <SEFramework/Image/VectorImage.h>
//#include "SEFramework/Property/PropertyHolder.h"

namespace SExtractor {


/**
 * @class VariablePsfStack
 *
 * @brief
 *
 * @details
 *
 * @todo
 *
 */
class VariablePsfStack: public VariablePsf {
public:

  /**
   * Constructor
   */
  VariablePsfStack(std::shared_ptr<CCfits::FITS> pFits):
    VariablePsf(0.1, VectorImage<SeFloat>::create(11, 11)), m_pFits(pFits), m_psf_size(0), m_pixel_scale(0.0){
    setup(pFits);
  };

  /**
   * Destructor
   */
  virtual ~VariablePsfStack() = default;

  /**
   * @return The width of the PSF
   */
  int getWidth() const{
    return m_psf_size;
  };

  /**
   * @return The height of the PSF
   */
  int getHeight() const{
    return m_psf_size;
  };

  /**
   * @return The pixel scale, as passed to the constructor
   */
  double getPixelScale() const{
    return m_pixel_scale;
  };

  /**
   * @return A reference to the list of components
   */
  const std::vector<VariablePsf::Component>& getComponents() const{
    return m_components;
  };

  /**
   *
   */
  std::shared_ptr<VectorImage<SeFloat>> getPsf(const std::vector<double> &values) const;

private:
  std::shared_ptr<CCfits::FITS> m_pFits;

  int m_psf_size;
  int m_grid_offset;

  double m_pixel_scale;

  long m_nrows;

  std::vector<SeFloat> m_ra_values;
  std::vector<SeFloat> m_dec_values;
  std::vector<SeFloat> m_x_values;
  std::vector<SeFloat> m_y_values;
  std::vector<int> m_gridx_values;
  std::vector<int> m_gridy_values;

  std::vector<Component> m_components;

  /*
   * Check the file, load the positions and so on
   */
  void setup(std::shared_ptr<CCfits::FITS> pFits);
};

}

#endif //_SEIMPLEMENTATION_PSF_VARIABLEPSFSTACK_H_
