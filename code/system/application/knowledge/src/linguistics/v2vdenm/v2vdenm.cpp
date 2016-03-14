/**
 * Copyright (C) 2015 Chalmers REVERE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"

#include "linguistics/v2vdenm/v2vdenm.hpp"

namespace opendlv {
namespace knowledge {
namespace linguistics {
namespace v2vdenm {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2vDenm::V2vDenm(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "knowledge-linguistics-v2vdenm")
{
}

V2vDenm::~V2vDenm()
{
}

/**
 * Receives .
 * Sends .
 */
void V2vDenm::nextContainer(odcore::data::Container &)
{
}

void V2vDenm::setUp()
{
}

void V2vDenm::tearDown()
{
}

} // v2vdenm
} // linguistics
} // knowledge
} // opendlv