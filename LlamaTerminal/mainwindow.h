#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>

#include "palettedframebuffer.h"
#include "serialinterface.h"
#include "textbuffer.h"
//#include "font.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void keyPressEvent(QKeyEvent* e);
    void DisplayMenu( void );

private:
    Ui::MainWindow *ui;
    PalettedFrameBuffer * pfb;
    TextBuffer * tb;
    SerialInterface * si;

private:
    void ShowReady( void );
    void ShowDebug( int level = 0 );
#define kDebugItem_Colors     ( 0x0001 )
#define kDebugItem_EVERYTHING ( 0xFFFF )


private:
#define kRunMode_Serial (0)
#define kRunMode_Menu (1)
    int runMode;

    int menuID;
    bool delayRender;

    bool localEcho;
    bool hidePrompt;

signals:

public slots:
    void ScreenHasBeenUpdated();
    void TextBufferHasChanged();    
    void timerTick();

    void SerialPortHasConnected();
    void SerialPortHasDisconnected();
    void SerialDidReceiveData();

private:
    QTimer timer;
    long timerTickCount;
    void StartTimer();
};

#endif // MAINWINDOW_H
