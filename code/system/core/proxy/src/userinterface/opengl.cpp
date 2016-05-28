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
  g_openGlInstance->OnMouse(a_button, a_state, a_x, a_y);
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
  m_texture(),
  m_isRunning(false)
{
  std::cout << "adsa" << std::endl;
  cv::Mat image = cv::imread("/home/ola/opendlv_logo/opendlv_logo.png",
      cv::IMREAD_UNCHANGED);
  std::cout << "adsa" << std::endl;
//  m_texture = MatToTexture(image);
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

GLuint OpenGl::MatToTexture(cv::Mat &a_mat)
{
  GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_mat.cols, a_mat.rows, 0, GL_BGRA,
	  GL_UNSIGNED_BYTE, a_mat.ptr());

	//glGenerateMipmap(GL_TEXTURE_2D);

  return texture;
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
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex2f(0, 0);
  glTexCoord2f(1, 0);
  glVertex2f(500, 0);
  glTexCoord2f(1, 1);
  glVertex2f(500, 500);
  glTexCoord2f(0, 1);
  glVertex2f(0, 500);
  glEnd();
  glFlush();


  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.5, 0.5, 0.0);       
  glBegin(GL_POLYGON);

  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5,  0.5);
  glVertex2f( 0.5,  0.5);
  glVertex2f( 0.5, -0.5);
  
  glEnd();

  glFlush(); 

  glutSwapBuffers();
  glutPostRedisplay();
}

void OpenGl::Start()
{
  m_isRunning = true;
  ::g_openGlInstance = this;
 
  int argc = 1;
  char *argv[] = {NULL};

  ::glutInit(&argc, argv);
  ::glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  ::glutInitWindowSize(600, 600);
  ::glutCreateWindow("OpenDLV Eye");
  ::glutFullScreen();
  ::glutKeyboardFunc(::KeyboardCallback);
  ::glutMouseFunc(::MouseCallback);
  ::glutDisplayFunc(::ShowCallback);

  glClearColor(0.0,0.0,0.0,0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0,800,600,1.0,-1.0,1.0);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  ::glutMainLoop();
}

} // userinterface
} // proxy
} // opendlv
