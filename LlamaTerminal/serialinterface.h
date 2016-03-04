#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
//#include <QSerialPort.h>

class SerialInterface : public QObject
{
    Q_OBJECT
public:
    explicit SerialInterface(QObject *parent = 0);
    ~SerialInterface( void );

private:
    QString deviceString;
    bool isConnected;
    unsigned long baud; /* 110..57600 */
    int bits;  /* 5,6,7,8 */
    char parity; /* N O E M S */
    int stop;   /* 1 1.5 2 */


public:
    unsigned long GetBaud()   { return this->baud; }
    int GetBits()    { return this->bits; }
    char GetParity() { return this->parity; }
    int GetStop()    { return this->stop; }
    QString GetParityString();
    QString GetStopString();
    bool GetConnected() { return this->isConnected; }
    QString GetDeviceString() { return this->deviceString; }

public:
    void ToggleBaud();
    void ToggleBits();
    void ToggleParity();
    void ToggleStop();
    void ToggleConnect();
    void ToggleDevice();

public: /* devices */
    int ScanForDevices();
    QString DeviceAt( int dno );
    bool DeviceAvailable( int dno );

    int Open( void );
    void Close( void );
    bool IsOpen( void );

public: /* xmit */
    void Send( char ch );
    void Send( char * str );
    void Send( QString str );

signals:

public slots:
};

#endif // SERIALINTERFACE_H
