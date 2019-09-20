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
 * GroupIdCheckImage.h
 *
 *  Created on: 2019 M01 30
 *      Author: mschefer
 */

#ifndef _SEIMPLEMENTATION_CHECKIMAGES_GROUPIDCHECKIMAGE_H_
#define _SEIMPLEMENTATION_CHECKIMAGES_GROUPIDCHECKIMAGE_H_

#include "SEUtils/Observable.h"
#include "SEFramework/Image/WriteableImage.h"
#include "SEFramework/Source/SourceGroupInterface.h"

namespace SExtractor {

class GroupIdCheckImage : public Observer<std::shared_ptr<SourceGroupInterface>> {
public:

  GroupIdCheckImage(std::shared_ptr<WriteableImage<unsigned int>> check_image) :
      m_check_image(check_image) {}

  virtual void handleMessage(const std::shared_ptr<SourceGroupInterface>& group);

private:
  std::shared_ptr<WriteableImage<unsigned int>> m_check_image;
};


} /* namespace SExtractor */


#endif /* SEIMPLEMENTATION_SEIMPLEMENTATION_CHECKIMAGES_GROUPIDCHECKIMAGE_H_ */
