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
CaptureRosTopic::CaptureRosTopic(VarList * _settings) : CaptureInterface(_settings), spinner(1) { 
#else
CaptureRosTopic::CaptureRosTopic(VarList * _settings, QObject * parent) : QObject(parent), CaptureInterface(_settings), spinner(1) {
#endif
  is_capturing = false;
  image_ready = false;
  settings->addChild(v_base_topic = new VarString("BaseTopic", "/center_camera"));
  current_image.allocate(COLOR_RGB8, 640, 480);
}

RawImage CaptureRosTopic::getFrame() {
#ifndef VDATA_NO_QT
  QMutexLocker lock(&mutex);
#endif
  //TODO: Use condition variables
  while(!image_ready);
  return current_image;
}

bool CaptureRosTopic::isCapturing() {
  return is_capturing;
}

void CaptureRosTopic::releaseFrame() {
  //TODO: Clear unused current_image memory
  last_image_message.reset();
  image_ready = false;
}

bool CaptureRosTopic::startCapture() {
  image_topic = v_base_topic->getString() + "/image_raw/decompressed";
  subscriber = node.subscribe(image_topic, 1, &CaptureRosTopic::receiveImage, this);
  spinner.start();
  is_capturing = true;
  return true;
}

void CaptureRosTopic::receiveImage(sensor_msgs::ImageConstPtr image) {
  last_image_message = image;
  
  // Ensure the right sizing
  current_image.ensure_allocation(COLOR_RGB8, last_image_message->width, last_image_message->height);

  // Update the timestamp
  current_image.setTime(last_image_message->header.stamp.toSec());

  // Copy the data
  //TODO: Clear unused current_image memory
  int size = current_image.computeImageSize(COLOR_RGB8, last_image_message->width * last_image_message->height);
  unsigned char* data = new unsigned char[size];
  memcpy(data, last_image_message->data.data(), size);
  current_image.setData(data);
  image_ready = true;
}

bool CaptureRosTopic::stopCapture() {
  last_image_message.reset();
  is_capturing = false;
  spinner.stop();
  subscriber.shutdown();
  return true;
}

string CaptureRosTopic::getCaptureMethodName() const {
  return "ROS Topic";
}
