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

#include "captureinterface.h"
#include <string>
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
    CaptureRosTopic(VarList * _settings) : CaptureInterface(_settings) { }
#else
    CaptureRosTopic(VarList * _settings, QObject * parent=NULL) : QObject(parent), CaptureInterface(_settings) { }
#endif
    ~CaptureRosTopic() { }

  protected:
    virtual RawImage getFrame();
    virtual bool     isCapturing();
    virtual void     releaseFrame();
    virtual bool     startCapture();
    virtual bool     stopCapture();
    virtual bool     resetBus();
    virtual void     readAllParameterValues();
    virtual bool     copyAndConvertFrame(const RawImage & src, RawImage & target);
    virtual string   getCaptureMethodName() const;
};
#endif
