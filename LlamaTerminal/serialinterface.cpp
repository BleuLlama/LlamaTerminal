/* 
 * serial interface
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
#include <QDebug>
#include <QSerialPort>
#include "serialinterface.h"


SerialInterface::SerialInterface(QObject *parent)
    : QObject(parent)
    , deviceName( "" )
    , deviceLocation( "" )
    , deviceDescription( "" )
    , isConnected( false )
    , baud( 19200 )
    , bits( 8 )
    , parity( QSerialPort::NoParity )
    , stop( QSerialPort::OneStop )
    , serial( NULL )
{
    this->ToggleDevice();
}

SerialInterface::~SerialInterface()
{

}


/* I know this is horrible, but this is a quick implementation. I'm sure
 * i'll come up with something better later.
 */
void SerialInterface::ToggleBaud()
{
         if( this->baud == 1200 ) this->baud = 2400;
    else if( this->baud == 2400 ) this->baud = 4800;
    else if( this->baud == 4800 ) this->baud = 9600;
    else if( this->baud == 9600 ) this->baud = 19200;
    else if( this->baud == 19200 ) this->baud = 38400;
    else if( this->baud == 38400 ) this->baud = 57600;
    else if( this->baud == 57600 ) this->baud = 115200;
    else if( this->baud == 115200 ) this->baud = 1200;
    else this->baud = 9600;
}

void SerialInterface::ToggleBits()
{
    this->bits++;
    if( this->bits > 8 ) this->bits = 5;
    if( this->bits < 5 ) this->bits = 5;
}


void SerialInterface::ToggleParity()
{
          if( this->parity == QSerialPort::NoParity )    this->parity = QSerialPort::EvenParity;
     else if( this->parity == QSerialPort::EvenParity )  this->parity = QSerialPort::OddParity;
     else if( this->parity == QSerialPort::OddParity )   this->parity = QSerialPort::SpaceParity;
     else if( this->parity == QSerialPort::SpaceParity ) this->parity = QSerialPort::MarkParity;
     else if( this->parity == QSerialPort::MarkParity )  this->parity = QSerialPort::NoParity;
     else this->parity = QSerialPort::NoParity;
}

QString SerialInterface::GetParityString()
{
    if( this->parity == QSerialPort::NoParity )    return "No";
    if( this->parity == QSerialPort::OddParity )   return "Odd";
    if( this->parity == QSerialPort::EvenParity )  return "Even";
    if( this->parity == QSerialPort::MarkParity )  return "Mark";
    if( this->parity == QSerialPort::SpaceParity ) return "Space";
    return "?";
}


void SerialInterface::ToggleStop()
{
         if( this->stop == QSerialPort::OneStop )        this->stop = QSerialPort::OneAndHalfStop;
    else if( this->stop == QSerialPort::OneAndHalfStop ) this->stop = QSerialPort::TwoStop;
    else if( this->stop == QSerialPort::TwoStop )        this->stop = QSerialPort::OneStop;
    else                                                 this->stop = QSerialPort::OneStop;
}

QString SerialInterface::GetStopString()
{
    if( this->stop == QSerialPort::OneStop )        return "1";
    if( this->stop == QSerialPort::OneAndHalfStop ) return "1.5";
    if( this->stop == QSerialPort::TwoStop )        return "2";
    return "?";
}

void SerialInterface::ToggleFlowControl()
{
         if( this->flowcontrol == QSerialPort::NoFlowControl )   this->flowcontrol = QSerialPort::HardwareControl;
    else if( this->flowcontrol == QSerialPort::HardwareControl ) this->flowcontrol = QSerialPort::SoftwareControl;
    else if( this->flowcontrol == QSerialPort::SoftwareControl ) this->flowcontrol = QSerialPort::NoFlowControl;
    else                                                         this->flowcontrol = QSerialPort::NoFlowControl;
}

QString SerialInterface::GetFlowControlString()
{
    if( this->flowcontrol == QSerialPort::NoFlowControl )   return "No";
    if( this->flowcontrol == QSerialPort::HardwareControl ) return "Hardware";
    if( this->flowcontrol == QSerialPort::SoftwareControl ) return "Software";
    return "?";
}

/////////////////////////////////////////////////////////////////////////////////
// this isnt the best way to do this, but it'll work for the first attempt
int SerialInterface::GetNumPortsAvailable()
{
    return QSerialPortInfo::availablePorts().count();
}

QString SerialInterface::NameForPortAt( int which )
{
    int c = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if( c == which ) {
            return info.portName();
        }
        c++;
    }
    return "";
}

QString SerialInterface::DescriptionForPortAt( int which )
{
    int c = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if( c == which ) {
            return info.description();
        }
        c++;
    }
    return "";
}

QString SerialInterface::LocationForPortAt( int which )
{
    int c = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if( c == which ) {
            return info.systemLocation();
        }
        c++;
    }
    return "";
}

int SerialInterface::IndexOfLocation( QString loc )
{
    int c = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if( !info.systemLocation().compare( loc )) return c;
        c++;
    }
    return -1;
}

bool SerialInterface::InUseForPortAt( int which )
{
    int c = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if( c == which ) {
            return info.isBusy();
        }
        c++;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

void SerialInterface::ToggleDevice()
{
    /* if nothing is available just clear it all out */
    if( this->GetNumPortsAvailable() == 0 )
    {
        this->deviceName = "";
        this->deviceDescription = "";
        this->deviceLocation = "";
        this->deviceInUse = false;
        return;
    }

    /* if current is "", return the first one */
    if( !this->deviceLocation.compare( "" ) )
    {
        this->deviceName = this->NameForPortAt( 0 );
        this->deviceDescription = this->DescriptionForPortAt( 0 );
        this->deviceLocation = this->LocationForPortAt( 0 );
        this->deviceInUse = this->InUseForPortAt( 0 );
        return;
    }

    /* find the current, pick the next one */
    int curidx = this->IndexOfLocation( this->deviceLocation );
    curidx++;
    /* check for wraps */
    if( curidx >= this->GetNumPortsAvailable() ) curidx = 0;

    /* and set it */
    this->deviceName = this->NameForPortAt( curidx );
    this->deviceDescription = this->DescriptionForPortAt( curidx );
    this->deviceLocation = this->LocationForPortAt( curidx );
    this->deviceInUse = this->InUseForPortAt( curidx );

}

/////////////////////////////////////////////////////////////////////////////////


void SerialInterface::readData()
{
    emit this->SerialReceivedData();
}


void SerialInterface::ToggleConnect()
{
    if( this->isConnected ) {
        /* disconnect */
        this->serial->close();
        delete this->serial;
        this->serial = NULL;

        this->isConnected = false;
        emit this->SerialPortDisconnected();
    } else {
        /* connect! */
        this->serial = new QSerialPort( this );
        connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

        serial->setPortName( this->deviceLocation );  // eg "COM3" "/dev/cu.usbmodem1411"
        serial->setBaudRate( this->baud );

        serial->setDataBits( (QSerialPort::DataBits) this->bits );
        serial->setParity( (QSerialPort::Parity) this->parity );
        serial->setStopBits( (QSerialPort::StopBits) this->stop );
        serial->setFlowControl( (QSerialPort::FlowControl) this->flowcontrol );

        if (serial->open(QIODevice::ReadWrite)) {
            this->isConnected = true;
            qDebug() << "Connected.";
            /*
            showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                              .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
                              */
            emit this->SerialPortConnected();

        } else {
            this->isConnected = false;
            qDebug() << tr("Error") << "  " << serial->errorString();
            this->serial->close();
            delete this->serial;
            this->serial = NULL;
            emit this->SerialPortDisconnected(); /* ? */
        }
    }
}

void SerialInterface::Send( QByteArray qba )
{
    if( this->isConnected && this->serial ) {
        this->serial->write( qba );
    }
}

////////////////////////////////////////////////////////////////

void SerialInterface::SaveSettings()
{
    SETUPSETTINGS();
    s.setValue( "Serial.Name", this->deviceName );
    s.setValue( "Serial.Location", this->deviceLocation );
    s.setValue( "Serial.Description", this->deviceDescription );

    s.setValue( "Serial.Baud", (long long) this->baud );
    s.setValue( "Serial.Bits", this->bits );
    s.setValue( "Serial.Parity", this->parity );
    s.setValue( "Serial.Flow", this->flowcontrol );
    s.setValue( "Serial.Stop", this->stop );
}

void SerialInterface::LoadSettings()
{
    SETUPSETTINGS();

    this->deviceName = s.value( "Serial.Name", "" ).toString();
    this->deviceLocation = s.value( "Serial.Location", "" ).toString();
    this->deviceDescription = s.value( "Serial.Description", "" ).toString();

    this->baud = (long) s.value( "Serial.Baud", 9600 ).toLongLong();
    this->bits = s.value( "Serial.Bits", 8 ).toInt();
    this->parity = s.value( "Serial.Parity", QSerialPort::NoParity ).toInt();
    this->flowcontrol = s.value( "Serial.Flow", QSerialPort::NoFlowControl ).toInt();
    this->stop = s.value( "Serial.Stop", QSerialPort::OneStop ).toInt();
}

