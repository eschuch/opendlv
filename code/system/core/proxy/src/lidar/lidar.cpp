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
#include <fstream>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/wrapper/SerialPort.h"
#include "opendavinci/odcore/wrapper/SerialPortFactory.h"

#include "lidar/lidar.hpp"
#include "lidar/device.hpp"

namespace opendlv {
namespace proxy {
namespace lidar {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Lidar::Lidar(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "proxy-lidar")
    , m_firstHeader(true)
    , m_indicator(true)
    , m_startConfirmed(false)
    , m_counter(0)
    , m_freshCoordinates()
    , m_latestReading()
    , m_device()
    , m_sick()
{
}

Lidar::~Lidar()
{
}


// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Lidar::body()
{
  std::cout << "In body";

  StartStream(); //Sends a message to sickan to start streaming data

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    std::cout << "Testvärde: " << m_distances[0] << " ";
    if(m_startConfirmed) {
      SendData();
      std::cout << "Kom igen då!" << std::endl;
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Lidar::StartStream()
{
  unsigned char streamStart[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x24, 0x34, 0x08};
  std::string startString( reinterpret_cast< char const* >(streamStart), 8) ;
  m_sick->send(startString);
}

void Lidar::StopStream()
{
  unsigned char streamStop[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x25, 0x35, 0x08};
  std::string stopString( reinterpret_cast< char const* >(streamStop), 8) ;
  m_sick->send(stopString);
}

void Lidar::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-lidar.type");

  m_position[0] = kv.getValue<float>("proxy-lidar.mount.x");
  m_position[1] = kv.getValue<float>("proxy-lidar.mount.y");
  m_position[2] = kv.getValue<float>("proxy-lidar.mount.z");

  //if (type.compare("sick") == 0) {
    //      m_device = std::unique_ptr<Device>(new SickDevice());
  //}

  //if (m_device.get() == nullptr) {
   // std::cerr << "[proxy-lidar] No valid device driver defined."
   //           << std::endl;
  //}

  string SERIAL_PORT = kv.getValue<std::string>("proxy-lidar.port");
  uint32_t BAUD_RATE = kv.getValue<uint32_t>("proxy-lidar.baudrate");

  try {
    m_sick = shared_ptr<odcore::wrapper::SerialPort>(odcore::wrapper::SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE));
    m_sick->setStringListener(this);
    m_sick->start();
  }
  catch(string &exception) {
    cerr << "[" << getName() << "] Could not connect to Sickan: " << exception << endl;
  }

  m_startResponse[0] = 0x06;  //What we hope to get from sickan
  m_startResponse[1] = 0x02;
  m_startResponse[2] = 0x80;
  m_startResponse[3] = 0x03;
  m_startResponse[4] = 0x00;
  m_startResponse[5] = 0xA0;
  m_startResponse[6] = 0x00;
  m_startResponse[7] = 0x10;
  m_startResponse[8] = 0x16;
  m_startResponse[9] = 0x0A;
  m_measurementHeader[0] = 0x02;
  m_measurementHeader[1] = 0x80;
  m_measurementHeader[2] = 0xD6;
  m_measurementHeader[3] = 0x02;
  m_measurementHeader[4] = 0xB0;
  m_measurementHeader[5] = 0x69;
  m_measurementHeader[6] = 0x41;

  std::cout << "Setup done" << std::endl;
}

void Lidar::nextString(const std::string &s) 
{
  unsigned char byte = (unsigned char)s[0]; //Storing byte as unsigned char

  if(m_counter == 0) {
    std::cout << "New reading " << s << " ";
  }

  //Updating buffer
  for(uint32_t i = 0; i < 9; i++) { 
    m_buffer[i] = m_buffer[i+1]; 
  }
  m_buffer[9] = byte;

  if(m_counter > 999) { //Safety clause
    m_counter--;
  }

  m_measurements[m_counter] = byte;
  m_counter++;

  m_indicator = true; //Checking for start confirmation
  for(uint32_t i = 0; i < 10; i++) {
    if(m_buffer[i] != m_startResponse[i]) { m_indicator = false; }        
  }
  if(m_indicator) { 
    m_counter = 0;
    m_startConfirmed = true; 
  }

  m_indicator = true; //Checking for output header
  for(uint32_t i = 0; i < 7; i++) {
    if(m_buffer[i] != m_measurementHeader[i]) { m_indicator = false; }
  }
  if(m_indicator) {
    std::cout << "Här kan man va" << std::endl;
    if(!m_firstHeader) {
      ConvertToDistances();
    } 
    m_counter = 0; 
    m_firstHeader = false; 
  }

}

void Lidar::ConvertToDistances()
{
  uint32_t byte1;
  uint32_t byte2;
  uint32_t distance;
  double cartesian[2];
  double PI = 3.14159265;

  m_freshCoordinates.clear();

  for(uint32_t i = 0; i < 361; i++) {

    byte1 = (int)m_measurements[i*2];
    byte2 = (int)m_measurements[i*2+1];
    distance = byte1*32 + byte2/8; //Integer millimeters in local polar coordinates
    cartesian[0] = distance * sin(PI * i /360) / 1000; //Local cartesian coordinates in meters
    cartesian[1] = distance * (-cos(PI * i /360)) / 1000;
    cartesian[0] += m_position[0]; //Truck cartesian coordinates
    cartesian[1] += m_position[1];

    float p[3];
    p[0] = sqrt(pow(cartesian[0],2) + pow(cartesian[1],2));
    p[1] = atan(cartesian[1]/cartesian[0]);
    p[2] = 0;
    opendlv::coordinate::Spherical3 spherical(p);
    m_freshCoordinates.push_back(spherical);

    m_distances[i] = p[0];
    if(i == 0) {
      std::cout << "Dist:" << distance << " ";
    }

  }

  m_latestReading.setListOfPoints(m_freshCoordinates);

  WriteToFile();

}

void Lidar::SendData()
{
  std::cout << "In SendData" << std::endl;
  

  odcore::data::Container c(m_latestReading);
  getConference().send(c);
}

void Lidar::WriteToFile()
{
  std::ofstream write;
  write.open("Test.txt",fstream::out);


  for(uint32_t i = 0; i < 361; i++)
  {
    write << m_distances[i] << ' ';
  }
  write << std::endl;


  write.close();
}

void Lidar::tearDown()
{
  
  StopStream(); //Sends a message to top streaming data
  m_sick->stop();
  m_sick->setStringListener(NULL);
}

} // lidar
} // proxy
} // opendlv
