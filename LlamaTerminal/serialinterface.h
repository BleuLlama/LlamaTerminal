/* 
 * serial interface header
 *
 *  serial port handler, scanner, and settings support
 */

/*
 * The MIT License (MIT)
 * Copyright (c) 2016 Scott Lawrence
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort.h>
#include "globals.h"

class SerialInterface : public QObject
{
    Q_OBJECT
public:
    explicit SerialInterface(QObject *parent = 0);
    ~SerialInterface( void );

private:
    QString deviceName;
    QString deviceLocation;
    QString deviceDescription;
    bool deviceInUse;

private:
    bool isConnected;
    unsigned long baud; /* 110..57600 */
    int bits;  /* 5,6,7,8 */
    char parity; /* N O E M S */
    int flowcontrol;
    int stop;   /* 1 1.5 2 */

public:
    unsigned long GetBaud()   { return this->baud; }
    int GetBits()    { return this->bits; }
    char GetParity() { return this->parity; }
    int GetStop()    { return this->stop; }
    QString GetParityString();
    QString GetStopString();
    QString GetFlowControlString();
    bool GetConnected() { return this->isConnected; }
    QString GetDeviceNameString() { return this->deviceName; }
    QString GetDeviceLocationString() { return this->deviceLocation; }
    QString GetDeviceDescriptionString() { return this->deviceDescription; }
    bool GetDeviceInUse() { return this->deviceInUse; }

private:
    QSerialPort * serial;
public:
    QSerialPort * GetSerialPort() { return this->serial; }

public:
    void ToggleBaud();
    void ToggleBits();
    void ToggleParity();
    void ToggleFlowControl();
    void ToggleStop();
    void ToggleConnect();
    void ToggleDevice();

private: /* devices */
    int GetNumPortsAvailable();
    QString NameForPortAt( int which );
    QString DescriptionForPortAt( int which );
    QString LocationForPortAt( int which );
    int IndexOfLocation( QString loc );
    bool InUseForPortAt( int which );

public: /* xmit */
    void Send( QByteArray qba );

signals:
    void SerialPortConnected();
    void SerialPortDisconnected();
    void SerialReceivedData();

public slots:
    void readData();

    void SaveSettings( void );
    void LoadSettings( void );
};

#endif // SERIALINTERFACE_H
