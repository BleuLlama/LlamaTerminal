#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "palettedframebuffer.h"
//#include "serialinterface.h"
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

    int runMode;
#define kRunMode_Serial (0)
#define kRunMode_Menu (1)
    int menuID;
    bool delayRender;

    int localEcho;

signals:

public slots:
    void ScreenHasBeenUpdated();
    void TextBufferHasChanged();
};

#endif // MAINWINDOW_H
