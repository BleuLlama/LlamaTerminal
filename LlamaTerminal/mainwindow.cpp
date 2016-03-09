/* 
 * mainwindow
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
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pfb( NULL )
    , tb( NULL )
    , si( NULL )
    , fnt( NULL )
    , runMode( kRunMode_Serial )
    , delayRender( false )
    , localEcho( false )
    , hidePrompt( false )
    , timerTickCount( 0 )

{
    ui->setupUi(this);

    this->pfb = new PalettedFrameBuffer( this );
    this->connect( this->pfb, SIGNAL(ScreenIsRendered()), this, SLOT( ScreenHasBeenUpdated() ) );
    this->connect( this, SIGNAL(Load()), this->pfb, SLOT(LoadSettings()) );
    this->connect( this, SIGNAL(Save()), this->pfb, SLOT(SaveSettings()) );

    this->tb = new TextBuffer( );
    this->connect( this->tb, SIGNAL(TextHasChanged()), this, SLOT( TextBufferHasChanged() ) );
    this->connect( this, SIGNAL(Load()), this->tb, SLOT(LoadSettings()) );
    this->connect( this, SIGNAL(Save()), this->tb, SLOT(SaveSettings()) );

    this->si = new SerialInterface();
    this->connect( this->si, SIGNAL( SerialPortConnected() ), this, SLOT( SerialPortHasConnected() ) );
    this->connect( this->si, SIGNAL( SerialPortDisconnected() ), this, SLOT( SerialPortHasDisconnected() ) );
    this->connect( this->si, SIGNAL( SerialReceivedData() ), this, SLOT( SerialDidReceiveData() ) );
    this->connect( this, SIGNAL(Load()), this->si, SLOT(LoadSettings()) );
    this->connect( this, SIGNAL(Save()), this->si, SLOT(SaveSettings()) );

    this->fnt = new Font();
    this->connect( this->fnt, SIGNAL( NewFontLoaded() ), this, SLOT( FontHasLoaded() ));
    this->connect( this, SIGNAL(Load()), this->fnt, SLOT(LoadSettings()) );
    this->connect( this, SIGNAL(Save()), this->fnt, SLOT(SaveSettings()) );
    this->fnt->Setup();
    this->pfb->SetFont( this->fnt->GetLFont() );

    this->LoadRequested();
    this->SaveRequested();

    // get it set up for use
    this->pfb->FillWithPattern( kDisplayPatternDiagonals );

    unsigned char pens[] = { 0x10, 0x01, 0x23, 0x34, 0x45, 0x46, 0x99 };
    this->pfb->DrawText( 30, 30, pens, "Hello!" );
    this->pfb->RenderScreen();


    this->ShowDebug( kDebugItem_Colors );
    this->ShowReady();

    this->connect( &this->timer, SIGNAL(timeout()),  this, SLOT( timerTick() ) );
    this->timer.start( 30 ); /* 30ish ms */
}


/* when we shut down, this gets called */
MainWindow::~MainWindow()
{
    //delete this->pfb;
    //delete this->tb;
    //delete this->si;
    delete ui;
}


void MainWindow::SerialPortHasConnected()
{
}

void MainWindow::SerialPortHasDisconnected()
{
}

void MainWindow::SerialDidReceiveData()
{
    this->delayRender = true;

    QByteArray data = this->si->GetSerialPort()->readAll();

    for( int j=0 ; j<data.length() ; j++ )
    {
        this->tb->AddCharacter( data.at( j ));
    }
    this->delayRender = false;
    this->TextBufferHasChanged();
}

/* this gets called when the framebuffer is ready to be redrawn to the screen */
void MainWindow::ScreenHasBeenUpdated( )
{
    /* get the generated QImage */
    QImage * theImage = this->pfb->getQImage();

    /* determine the destination size */
    QSize sz( this->ui->graphicsQLabel->size().width(),
              this->ui->graphicsQLabel->size().height() );

    /* and scale it to fit! */
    this->ui->graphicsQLabel->setPixmap( QPixmap::fromImage( *theImage ).scaled( sz,
                                                                   Qt::IgnoreAspectRatio,
                                                                   Qt::FastTransformation ) );
}

/* this is called when the text buffer changes */
void MainWindow::TextBufferHasChanged( )
{
    if( this->delayRender ) return;

    /* the text buffer has changed, we'll just refresh the graphicstext render */
    this->pfb->RenderTextColorBuffer( this->tb->GetColor(),
                                      this->tb->GetText(),
                                      this->hidePrompt? -10 : this->tb->GetCursorX() );
}

/* this is called when a new font is loaded */
void MainWindow::FontHasLoaded( )
{
    this->pfb->SetFont( this->fnt->GetLFont() );
    this->TextBufferHasChanged();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::ShowReady( void )
{
    this->delayRender = true;

    // bars with version text
    tb->SetPen( 0x09, 0x09 ); tb->AddCharacters( ' ', 40 );
    tb->SetPen( 0x01, 0x00 ); tb->AddText( "\n             LlamaTerm " kLlamaTermVersion "\n" );
    tb->SetPen( 0x09, 0x09 ); tb->AddCharacters( ' ', 40 );

    // helpful
    tb->SetPen( 0x0e, 0x00 ); tb->AddText( "\n\nPress [alt] for menu.\n" );

    // ready!
    tb->SetPen( 0x05, 0x00 ); tb->AddText( "\nReady!\n\n" );

    tb->SetPen( 0x0d, 0x00 );

    this->delayRender = false;
    this->TextBufferHasChanged();
}


void MainWindow::ShowDebug( int items )
{
    this->delayRender = true;

    tb->AddText( "\n\n" );

    if( items & kDebugItem_Colors )
    {
        for( int m=0 ; m < 1 ; m++ ) {
            for( int k=0 ; k<=0x0f ; k++ ) {
                tb->SetPen( 1, k );
                char buf[5];
                snprintf( buf, 5, " %x", k );
                tb->AddText( buf );
            }
            tb->AddCharacter( '\n' );
        }
        tb->AddCharacter( '\n' );
    }

    this->delayRender = false;
    this->TextBufferHasChanged();
}


#define kMenu_Main     (0)
#define kMenu_Serial   (1)
#define kMenu_Font     (2)
#define kMenu_Video    (3)
#define kMenu_Terminal (4)
#define kMenu_Quit     (5)

void MainWindow::DisplayMenu( void )
{
    this->delayRender = true;
    this->tb->SetPen( 0x07, 0x00 );
    this->tb->AddText( "\n" );

    switch( this->menuID ) {
    case( kMenu_Main ):
        this->tb->AddText( "Main Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  d: debug\n");
        this->tb->AddText( "  s: serial\n");
        this->tb->AddText( "  f: font\n");
        this->tb->AddText( "  v: video\n");
        this->tb->AddText( "  t: terminal\n");
        this->tb->AddText( "  x: exit menu (or alt)\n");
        this->tb->AddText( "  q: quit LlamaTerminal\n");
        break;

    case( kMenu_Quit ):
        this->tb->SetPen( 0x02, 0x00 );
        this->tb->AddText( "\nReally quit?\n");
        this->tb->SetPen( 0x07, 0x00 );
        this->tb->AddText( "  y: yes, quit\n");
        this->tb->AddText( "  n: no, return\n");
        break;

    case( kMenu_Terminal ):
        this->tb->AddText( "Terminal Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  c: " + this->pfb->GetPromptString() + " cursor\n" );
        this->tb->AddText( "  e: ");
            this->tb->AddText( this->localEcho? "Local echo\n" : "No echo\n" );
        this->tb->AddText( "  w: wrap lines: " + this->tb->GetWrapString() + "\n" );

        this->tb->AddText( "  x: exit menu\n");
        break;

    case( kMenu_Serial ):
        this->tb->AddText( "Serial Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  d: " + this->si->GetDeviceNameString() + "\n");
        if( this->si->GetDeviceInUse() == true ) {
            this->tb->AddText( "     (unavailable)\n");
        } else {
            //this->tb->AddText( "     " + this->si->GetDeviceDescriptionString() + "\n");
            this->tb->AddText( "  c: ");
                this->tb->AddText( this->si->GetConnected()? "Connected\n" : "Not connected\n" );

            if( !this->si->GetConnected() ) {
                this->tb->AddText( "  r: " + QString::number( si->GetBaud() ) + " baud\n");
                this->tb->AddText( "  b: " + QString::number( si->GetBits() ) + " bits\n");
                this->tb->AddText( "  p: " + si->GetParityString() + " parity\n");
                this->tb->AddText( "  s: " + si->GetStopString() + " stop\n");
                this->tb->AddText( "  f: " + si->GetFlowControlString() + " flow ctrl\n");
            }
        }

        this->tb->AddText( "  x: exit menu\n");
        break;

    case( kMenu_Font ):
        this->tb->AddText( "Font Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  d: Pick fonts directory\n" );
        this->tb->AddText( "  f: " + this->fnt->GetFontName() + "\n" );
        this->tb->AddText( "  h: " );
            this->tb->AddText( this->pfb->GetDoubleHoriz() ? "Horiz Double\n" : "Horiz Single\n" );
        this->tb->AddText( "  v: ");
            this->tb->AddText( this->pfb->GetDoubleVert() ? "Vert Double\n" : "Vert Single\n" );
        this->tb->AddText( "  s: ");
            this->tb->AddText( this->pfb->GetScanLines() ? "Scanlines\n" : "No Scanlines\n" );
        this->tb->AddText( "  1: " + QString::number( this->pfb->GetHSpacing() ) + " px H Space\n" );
        this->tb->AddText( "  2: " + QString::number( this->pfb->GetVSpacing() ) + " px V Space\n" );
        this->tb->AddText( "  x: exit menu\n");
        break;

    case( kMenu_Video ):
        this->tb->AddText( "Video Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  k: clear\n" );
        this->tb->AddText( "  p: " + this->pfb->GetPaletteString() + "\n" );
        this->tb->AddText( "  x: exit menu\n");
        break;

    default:
        break;
    }
    this->delayRender = false;
    this->TextBufferHasChanged();
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    qDebug() << e;
}

void MainWindow::mouseMoveEvent( QMouseEvent * e )
{
    qDebug() << e;
}

void MainWindow::mousePressEvent( QMouseEvent * e )
{
    qDebug() << e;
}

void MainWindow::mouseReleaseEvent( QMouseEvent * e )
{
    qDebug() << e;
}



/* this is called when a key is pressed
 *  No need to Connect() to this, it just magically happens
 */
void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if( e->modifiers() & Qt::AltModifier){
        if( this->runMode == kRunMode_Menu ) {
            this->runMode = kRunMode_Serial;

            this->tb->SetPen( 0x01, 0x00 );
            this->tb->AddText( "\nText mode.\n");
        } else {
            this->runMode = kRunMode_Menu;
            this->menuID = kMenu_Main;

            this->DisplayMenu();
            return;
        }
    }

    if( e->modifiers() & Qt::ControlModifier){
        qDebug() << "ctrl " << e->text().toUtf8() << "pressed";
        return;
    }

    if( this->runMode == kRunMode_Menu ) {
        int ch = e->text().toUtf8()[0];

        this->tb->SetPen( 0x0A, 0x00 );
        this->tb->AddText( "<" + e->text() + ">\n");

        switch( this->menuID ) {
        case( kMenu_Main ): /* ******** Main Menu ******** */
            switch( ch ) {
            case( 'd' ): this->ShowDebug( kDebugItem_EVERYTHING ); break;
            case( 's' ): this->menuID = kMenu_Serial; break;
            case( 't' ): this->menuID = kMenu_Terminal; break;
            case( 'v' ): this->menuID = kMenu_Video; break;
            case( 'f' ): this->menuID = kMenu_Font; break;
            case( 'q' ): this->menuID = kMenu_Quit; break;

            case( 'x' ):
                this->runMode = kRunMode_Serial;
                this->tb->SetPen( 0x01, 0x00 );
                this->tb->AddText( "Text mode.\n");
                break;

            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        case( kMenu_Quit ):
            switch( ch ) {
            case( 'y' ): exit( 0 );
            default:
                break;
            }
            this->menuID = kMenu_Main;
            break;

        case( kMenu_Serial ): /* ******** Serial Menu ******** */

            if( this->si->GetDeviceInUse() == true ) {
                /* if the device is in use, we can only go to the next one */
                if( ch == 'd' ) this->si->ToggleDevice();

            } else
            if( this->si->GetConnected() == true ) {
                /* if the device is connected, we can only disconnect */
                if( ch == 'c' ) this->si->ToggleConnect();

            } else {
                /* otherwise we can change settings */
                switch( ch ) {
                case( 'c' ): this->si->ToggleConnect(); break;      /* connect/disconnect */
                case( 'd' ): this->si->ToggleDevice(); break;       /* select next device */
                case( 'r' ): this->si->ToggleBaud(); break;         /* baud rate */
                case( 'b' ): this->si->ToggleBits(); break;         /* data bits */
                case( 'p' ): this->si->ToggleParity(); break;       /* parity */
                case( 's' ): this->si->ToggleStop(); break;         /* stop bits */
                case( 'f' ): this->si->ToggleFlowControl(); break;  /* flow control */
                default: break;
                }
            }

            switch( ch ) {
            case( 'c' ): case( 'd' ): case( 'r' ): case( 'b' ): case( 'p' ): case( 's' ):
                // ignore, and no error printed
                break;

            case( 'x' ): this->menuID = kMenu_Main; break;
            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        case( kMenu_Terminal ): /* ******** Terminal Menu ******** */
            switch( ch ) {
            case( 'c' ): this->pfb->TogglePromptType(); break;
            case( 'e' ): this->localEcho = (this->localEcho)?0:1; break;
            case( 'w' ): this->tb->ToggleWrap(); break;

            case( 'x' ): this->menuID = kMenu_Main; break;
            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        case( kMenu_Video ): /* ******** Video Menu ******** */
            switch( ch ) {
            case( 'k' ): this->tb->Clear(); break;
            case( 'p' ):     /* change palette */
                this->pfb->TogglePalette();
                this->ShowDebug( kDebugItem_Colors );
                break;

            case( 'x' ): this->menuID = kMenu_Main; break;
            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        case( kMenu_Font ): /* ******** Font Menu ******** */
            switch( ch ) {
            case( 'd' ): this->fnt->RequestFontDirectory(); break; /* pick font directory */
            case( 'f' ): this->fnt->ToggleFont(); break; /* font */
            case( 'h' ): this->pfb->ToggleDoubleHoriz(); break; /* double horizontal pixels */
            case( 'v' ): this->pfb->ToggleDoubleVert(); break;  /* double vertical pixels */
            case( 's' ): this->pfb->ToggleScanLines(); break;   /* show scanlines */
            case( '1' ): this->pfb->ToggleHSpacing(); break;
            case( '2' ): this->pfb->ToggleVSpacing(); break;
            case( 'x' ): this->menuID = kMenu_Main; break;
            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        default:
            break;
        }

        /* and save out any changes */
        this->SaveRequested();

        /* and redisplay the menu... */
        if( this->runMode == kRunMode_Menu ) {
            this->DisplayMenu();
        }
    } else {
        /* serial mode */
        if( this->localEcho ) {
            for( int j=0 ; j<e->text().length() ; j++ ) {
                tb->AddCharacter( e->text().toUtf8().at( j ) );
            }
        }
        this->si->Send( e->text().toUtf8() );
    }
}


void MainWindow::timerTick( void )
{
    this->timerTickCount++;

    if( this->timerTickCount & 0x08 ) {
        this->hidePrompt = true;
    } else {
        this->hidePrompt = false;
    }
    this->TextBufferHasChanged();
}

///////////////////////////////////////////////////////////

void MainWindow::SaveRequested( void )
{
    SETUPSETTINGS();
    s.setValue( kSettings_AppVersion, kLlamaTermVersion );
    s.setValue( kSettings_TermLocalEcho, this->localEcho );
    emit Save();
}

void MainWindow::LoadRequested( void )
{
    SETUPSETTINGS();
    this->localEcho = s.value( kSettings_TermLocalEcho, false ).toBool();
    emit Load();
}
