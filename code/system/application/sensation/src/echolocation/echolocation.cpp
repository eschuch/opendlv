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

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "echolocation/echolocation.hpp"

namespace opendlv {
namespace sensation {
namespace echolocation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Echolocation::Echolocation(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "sensation-echolocation")
{
}

Echolocation::~Echolocation()
{
}

/**
 * Receives .
 * Sends .
 */
void Echolocation::nextContainer(odcore::data::Container &a_c)
{
	if(a_c.getDataType() != odcore::proxy::EchoReading::ID()){
  	return;
  }
  odcore::data::TimeStamp now;

  opendlv::proxy::EchoReading reading = c.getData<opendlv::proxy::EchoReading>();
  std::vector<opendlv::model::Direction> directions = reading.getDirections();
  std::vector<double> distances = reading.getRadii();
  uint32_t nPoints = reading.getNumberOfPoints();

  std::vector<float> angles;

  for(int i = 0; i < nPoints; i++) {
  	angles.push_back(directions[i].getAzimuth())
  }

  m_angles.insret(m_angles.begin(), angles.begin(), angles.end());
  m_distances.insert(m_distances.begin(), distances.begin(), distances.end());
  
  std::vector<opendlv::perception::Object> identifiedObjects;



  /*
  if(lidar){
  	bool isObject = false;
  	for(int i = 0; i < nPoints; i++){
			if(distances[i] < )
  	}

  }



  if(sonar){
  }
	*/

}

double Echolocation::PointDistance(float a_angle1, double a_dist1, float a_angle2, double a_dist2)
{
	double x1 = std::cos(a_angle1)*a_dist1;
	double y1 = std::sin(a_angle1)*a_dist1;
	double x2 = std::cos(a_angle2)*a_dist2;
	double y2 = std::sin(a_angle2)*a_dist2;
	double between = sqrt(pow(x1-x2,2) + pow(y1-y2,2));
	return between;
}

void Echolocation::setUp()
{
}

void Echolocation::tearDown()
{
}

} // echolocation
} // sensation
} // opendlv
