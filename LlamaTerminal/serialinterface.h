#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort.h>

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
};

#endif // SERIALINTERFACE_H
