//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    capture_ros_topic.cpp
  \brief   Image capture from a ROS image topic
  \author  Jacob Menashe, (C) 2016
*/
//========================================================================

#include "capture_ros_topic.h"

RawImage CaptureRosTopic::getFrame() {
  return RawImage();
}

bool CaptureRosTopic::isCapturing() {
  return false;
}

void CaptureRosTopic::releaseFrame() {
  return;
}

bool CaptureRosTopic::startCapture() {
  return false;
}

bool CaptureRosTopic::stopCapture() {
  return false;
}

bool CaptureRosTopic::resetBus() {
  return false;
}

void CaptureRosTopic::readAllParameterValues() {
  return;
}

bool CaptureRosTopic::copyAndConvertFrame(const RawImage & src, RawImage & target) {
  return false;
}

string CaptureRosTopic::getCaptureMethodName() const {
  return string();
}
