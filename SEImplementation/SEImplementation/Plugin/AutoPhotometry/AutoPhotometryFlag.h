/*
 * AutoPhotometryFlag.h
 *
 *  Created on: Oct 10, 2018
 *      Author: Alejandro Alvarez Ayllon
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_AUTOPHOTOMETRY_AUTOPHOTOMETRYFLAG_H_
#define _SEIMPLEMENTATION_PLUGIN_AUTOPHOTOMETRY_AUTOPHOTOMETRYFLAG_H_

#include "SEUtils/Types.h"
#include "SEFramework/Property/Property.h"
#include "SEFramework/Source/SourceFlags.h"
#include <vector>

namespace SExtractor {

class AutoPhotometryFlag: public Property {
public:
  virtual ~AutoPhotometryFlag() = default;

  AutoPhotometryFlag(const std::vector<Flags> &flags): m_flags{flags} {}

  const std::vector<Flags>& getFlags() const {
    return m_flags;
  }

private:
  std::vector<Flags> m_flags;
};

} /* namespace SExtractor */

#endif /* _SEIMPLEMENTATION_PLUGIN_AUTOPHOTOMETRY_AUTOPHOTOMETRYFLAG_H_ */