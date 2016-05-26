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

#include <cstdlib>
#include <GL/glut.h>

#include "opendavinci/odcore/data/Container.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "userinterface/userinterface.hpp"

namespace opendlv {
namespace proxy {
namespace userinterface {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
UserInterface::UserInterface(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-userinterface"),
    //m_openglThread(&UserInterface::RunGui)
    m_openglThread()
{
}

UserInterface::~UserInterface()
{
  m_openglThread.join();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode UserInterface::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

 //   auto sonarReadings = m_device->GetEchoReadings();
 //   odcore::data::Container sonarReadingContainer(sonarReadings);
 //   getConference().send(sonarReadingContainer);
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void UserInterface::nextContainer(odcore::data::Container &a_container)
{
  (void) a_container;
/* 
  if (a_container.getDataType() == opendlv::proxy::UserInterfaceRequest::ID()) {
    opendlv::proxy::UserInterfaceRequest request = 
    a_container.getData<opendlv::proxy::UserInterfaceRequest>();

    std::string deviceId = request.getDeviceId();

    if (deviceId != getIdentifier()) {
      return;
    }

    bool userinterfaceValue = request.getUserInterfaceValue();
    uint8_t userinterfaceIndex = request.getUserInterfaceIndex();

    m_device->SetValue(userinterfaceIndex, userinterfaceValue);
  }
  */
}

void UserInterface::RunGui()
{
}

void UserInterface::setUp()
{
  std::thread (call_from_thread);
}

void UserInterface::tearDown()
{
}

} // userinterface
} // proxy
} // opendlv
