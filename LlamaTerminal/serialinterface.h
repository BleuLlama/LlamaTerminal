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

public: /* devices */
    int ScanForDevices();
    QString DeviceAt( int dno );
    bool DeviceAvailable( int dno );

    int Open( int device );
    void Close( void );
    bool IsOpen( void );

public: /* xmit */
    void Send( char ch );
    void Send( char * str );
    void Send( QString str );

public: /* text buffer */
    void TextClear();
    void TextSetWrapWidth( int w );

signals:

public slots:
};

#endif // SERIALINTERFACE_H
