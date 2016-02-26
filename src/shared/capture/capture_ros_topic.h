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

#ifndef CAPTUREROSTOPIC_H
#define CAPTUREROSTOPIC_H

#include <string>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "captureinterface.h"
#include "VarTypes.h"
#ifndef VDATA_NO_QT
  #include <QMutex>
  #include <QMutexLocker>
#else
  #include <pthread.h>
#endif

// If not using QT
#ifdef VDATA_NO_QT
class CaptureRosTopic : public CaptureInterface {
#else
class CaptureRosTopic : public QObject, public CaptureInterface {
  Q_OBJECT
  protected:
    QMutex mutex;
#endif

  public:
#ifdef VDATA_NO_QT
    CaptureRosTopic(VarList * _settings);
#else
    CaptureRosTopic(VarList * _settings, QObject * parent=NULL);
#endif
    ~CaptureRosTopic() { }

  protected:
    virtual RawImage getFrame();
    virtual bool     isCapturing();
    virtual void     releaseFrame();
    virtual bool     startCapture();
    virtual bool     stopCapture();
    virtual string   getCaptureMethodName() const;

    void receiveImage(sensor_msgs::ImageConstPtr image);

  private:
    bool is_capturing;
    VarString * v_base_topic;
    std::string image_topic;
    ros::NodeHandle node;
    ros::Subscriber subscriber;
    sensor_msgs::ImageConstPtr last_image;
};
#endif
