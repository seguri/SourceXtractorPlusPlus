/** 
 * @file CircularlySymmetricModelComponent.h
 * @date August 25, 2015
 * @author Nikolaos Apostolakos
 */

#ifndef MODELFITTING_CIRCULARLYSYMMETRICMODELCOMPONENT_H
#define	MODELFITTING_CIRCULARLYSYMMETRICMODELCOMPONENT_H

#include <memory> // For std::unique_ptr
#include <vector>
#include "ModelFitting/Models/SharpRegionManager.h"
#include "ModelFitting/Models/ModelComponent.h"
#include "ModelFitting/Models/SersicProfile.h"

namespace ModelFitting {

template <typename Profile>
class CircularlySymmetricModelComponent : public ModelComponent {
  
public:
  
  template <typename... ProfileParameters>
  CircularlySymmetricModelComponent(std::unique_ptr<SharpRegionManager> sharp_manager,
                                    ProfileParameters&&... proj_parameters);
  
  virtual ~CircularlySymmetricModelComponent();
  
  virtual double getValue(double x, double y);
  
  virtual void updateRasterizationInfo(double scale, double r_max);
  
  virtual std::vector<ModelSample> getSharpSampling();
  
  virtual bool insideSharpRegion(double x, double y);
  
private:
  
  std::unique_ptr<SharpRegionManager> m_sharp_manager;
  Profile m_profile;
  
}; // end of class CircularlySymmetricModelComponent

using SersicModelComponent = CircularlySymmetricModelComponent<SersicProfile>;

} // end of namespace ModelFitting

#include "_impl/CircularlySymmetricModelComponent.icpp"

#endif	/* MODELFITTING_CIRCULARLYSYMMETRICMODELCOMPONENT_H */
