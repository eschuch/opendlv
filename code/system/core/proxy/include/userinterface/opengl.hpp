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

#ifndef USERINTERFACE_OPENGL_HPP_
#define USERINTERFACE_OPENGL_HPP_

#include <vector>

extern "C"
void KeyboardCallback(unsigned char, int, int);
extern "C"
void MouseCallback(int, int, int, int);
extern "C"
void ShowCallback();

namespace opendlv {
namespace proxy {
namespace userinterface {

class OpenGl {
 public:
  OpenGl();
  OpenGl(OpenGl const &) = delete;
  OpenGl &operator=(OpenGl const &) = delete;
  virtual ~OpenGl();
  bool IsRunning() const;
  void OnKeyboard(unsigned char, int, int);
  void OnMouse(int, int, int, int);
  void OnShow();
  void Release();
  void Start();

 private:
  bool m_isRunning;
};

} // userinterface
} // proxy
} // opendlv

#endif
