/* 
 * mainwindow header
 *
 *  the code for the main window user interface (the core of the engine)
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>

#include "globals.h"
#include "palettedframebuffer.h"
#include "serialinterface.h"
#include "textbuffer.h"
#include "font.h"

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
    Font * fnt;

private:
    void ShowReady( void );

private:
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
    void Save(); /* When we want our children to save */
    void Load(); /* When we want our children to load */

public slots:
    void ScreenHasBeenUpdated();
    void TextBufferHasChanged();    
    void timerTick();

    void SerialPortHasConnected();
    void SerialPortHasDisconnected();
    void SerialDidReceiveData();

    void FontHasLoaded();

    void SaveRequested(); /* a child has requested a save */
    void LoadRequested(); /* a child has requested a load */

private:
    QTimer timer;
    long timerTickCount;
    void StartTimer();
};

#endif // MAINWINDOW_H
