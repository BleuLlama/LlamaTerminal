#include <QDebug>
#include "serialinterface.h"

SerialInterface::SerialInterface(QObject *parent)
    : QObject(parent)
    , deviceString( "tty.usbmodem1421")
    , isConnected( false )
    , baud( 19200 )
    , bits( 8 )
    , parity( 'N' )
    , stop( 10 )
{

}

SerialInterface::~SerialInterface()
{

}

/* I know this is horrible, but this is a quick implementation. I'm sure
 * i'll come up with something better later.
 */
void SerialInterface::ToggleBaud()
{
         if( this->baud == 110 ) this->baud = 300;
    else if( this->baud == 300 ) this->baud = 1200;
    else if( this->baud == 1200 ) this->baud = 2400;
    else if( this->baud == 2400 ) this->baud = 4800;
    else if( this->baud == 4800 ) this->baud = 9600;
    else if( this->baud == 9600 ) this->baud = 19200;
    else if( this->baud == 19200 ) this->baud = 38400;
    else if( this->baud == 38400 ) this->baud = 57600;
    else if( this->baud == 57600 ) this->baud = 115200;
    else if( this->baud == 115200 ) this->baud = 230400;
    else if( this->baud == 230400 ) this->baud = 110;
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
          if( this->parity == 'N' ) this->parity = 'O';
     else if( this->parity == 'O' ) this->parity = 'E';
     else if( this->parity == 'E' ) this->parity = 'M';
     else if( this->parity == 'M' ) this->parity = 'S';
     else if( this->parity == 'S' ) this->parity = 'N';
     else this->parity = 'N';
}

void SerialInterface::ToggleStop()
{
         if( this->stop == 10 ) this->stop = 15;
    else if( this->stop == 15 ) this->stop = 20;
    else if( this->stop == 20 ) this->stop = 10;
    else                        this->stop = 10;
}


QString SerialInterface::GetParityString()
{
    if( this->parity == 'N' ) return "No";
    if( this->parity == 'O' ) return "Odd";
    if( this->parity == 'E' ) return "Even";
    if( this->parity == 'M' ) return "Mark";
    if( this->parity == 'S' ) return "Space";
    return "?";
}

QString SerialInterface::GetStopString()
{
    if( this->stop == 10 ) return "1";
    if( this->stop == 15 ) return "1.5";
    if( this->stop == 20 ) return "2";
    return "?";
}

void SerialInterface::ToggleDevice()
{
}

void SerialInterface::ToggleConnect()
{
}
