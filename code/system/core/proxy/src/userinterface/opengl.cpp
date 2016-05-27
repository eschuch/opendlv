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

#include <iostream>

#include <GL/glut.h>

#include "userinterface/opengl.hpp"

opendlv::proxy::userinterface::OpenGl* g_openGlInstance;

extern "C"
void KeyboardCallback(unsigned char a_key, int a_x, int a_y)
{
  g_openGlInstance->OnKeyboard(a_key, a_x, a_y);
}

extern "C"
void MouseCallback(int a_button, int a_state, int a_x, int a_y)
{
  g_openGlInstance->OnMouse(a_key, a_x, a_y);
}

extern "C"
void ShowCallback()
{
  g_openGlInstance->OnShow();
}

namespace opendlv {
namespace proxy {
namespace userinterface {

/**
 * Constructor.
 *
 */
OpenGl::OpenGl():
  m_isRunning(false)
{
}

OpenGl::~OpenGl()
{
}

bool OpenGl::IsRunning() const
{
  return m_isRunning;
}

void OpenGl::Release()
{
  m_isRunning = false;
}

void OpenGl::OnKeyboard(unsigned char a_key, int, int)
{
  if (a_key == 'q' || a_key == 'Q') {
    exit(EXIT_SUCCESS);
  }
}

void OpenGl::OnMouse(int a_button, int a_state, int a_x, int a_y)
{
  std::cout << a_button << "  " << a_state << "  " << a_x << "  " << a_y << std::endl;
}

void OpenGl::OnShow()
{
  if (!m_isRunning) {
    exit(EXIT_SUCCESS);
  }

  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.5, 0.5, 0.0);       
  glBegin(GL_POLYGON);

  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5,  0.5);
  glVertex2f( 0.5,  0.5);
  glVertex2f( 0.5, -0.5);
  
  glEnd();

  glFlush(); 
}

void OpenGl::Start()
{
  m_isRunning = true;
  ::g_openGlInstance = this;
 
  int argc = 1;
  char *argv[] = {NULL};

  ::glutInit(&argc, argv);
  ::glutCreateWindow("OpenDLV Eye");
  ::glutKeyboardFunc(::KeyboardCallback);
  ::glutMouseFunc(::MouseCallback);
  ::glutDisplayFunc(::ShowCallback);
  ::glutMainLoop();
}

} // userinterface
} // proxy
} // opendlv
