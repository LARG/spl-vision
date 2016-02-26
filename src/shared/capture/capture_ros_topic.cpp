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
    
#ifdef VDATA_NO_QT
CaptureRosTopic::CaptureRosTopic(VarList * _settings) : CaptureInterface(_settings) { 
#else
CaptureRosTopic::CaptureRosTopic(VarList * _settings, QObject * parent) : QObject(parent), CaptureInterface(_settings) {
#endif
  is_capturing = false;
  settings->addChild(v_base_topic = new VarString("BaseTopic", "/center_camera"));
}

RawImage CaptureRosTopic::getFrame() {
#ifndef VDATA_NO_QT
  QMutexLocker lock(&mutex);
#endif
  //TODO: Return the last-constructed RawImage if there hasn't been an update from the topic
  RawImage image;
  if(last_image != NULL) {
    image.setWidth(last_image->width);
    image.setHeight(last_image->height);
    image.setColorFormat(COLOR_RGB8);
    unsigned char* data = new unsigned char[
      image.computeImageSize(COLOR_RGB8, last_image->width * last_image->height)
    ];
    image.setData(data);
  }
  return image;
}

bool CaptureRosTopic::isCapturing() {
  return is_capturing;
}

void CaptureRosTopic::releaseFrame() {
  last_image.reset();
}

bool CaptureRosTopic::startCapture() {
  image_topic = v_base_topic->getString() + "/image_raw/decompressed";
  subscriber = node.subscribe(image_topic, 1, &CaptureRosTopic::receiveImage, this);
  return true;
}

void CaptureRosTopic::receiveImage(sensor_msgs::ImageConstPtr image) {
  last_image = image;
}

bool CaptureRosTopic::stopCapture() {
  subscriber.shutdown();
  last_image.reset();
  is_capturing = false;
  return true;
}

string CaptureRosTopic::getCaptureMethodName() const {
  return "ROS Topic";
}
