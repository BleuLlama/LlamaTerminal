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
    , tp( NULL )
    , si( NULL )
    , fnt( NULL )
    , theme( NULL )
    , decorationStyle( 0 )
    , decorationColor( 1 )
    , decorationSize( 0 )
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

    this->tp = new TextPipe( );
    this->tp->setPfb( this->pfb );
    //this->connect( this->tp, SIGNAL(TextHasChanged()), this, SLOT( TextBufferHasChanged() ) );
    this->connect( this, SIGNAL(Load()), this->tp, SLOT(LoadSettings()) );
    this->connect( this, SIGNAL(Save()), this->tp, SLOT(SaveSettings()) );

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

    this->theme = new Theme();
    this->connect( this->theme, SIGNAL( NewThemeLoaded() ), this, SLOT( ThemeHasLoaded() ));
    //this->connect( this, SIGNAL(Load()), this->theme, SLOT(LoadSettings()) );
    this->connect( this, SIGNAL(Save()), this->theme, SLOT(SaveSettings()) );
    this->theme->Setup();

    this->LoadRequested();
    this->SaveRequested();

    // get it set up for use
    this->pfb->FillWithPattern( kDisplayPatternBlank );

    /*
    unsigned char pens[] = { 0x10, 0x01, 0x23, 0x34, 0x45, 0x46, 0x99 };
    this->pfb->DrawText( 30, 30, pens, "Hello!" );
    */
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
    //delete this->tp;
    //delete this->si;
    //delete this->theme;
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
        this->tp->AddCharacter( data.at( j ));
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


#define kDecoration_None    0
#define kDecoration_C64     1
#define kDecoration_Amiga1  2
#define kDecoration_Amiga2  3
#define kMaxDecorations     3

/* this is called when the text buffer changes */
void MainWindow::TextBufferHasChanged( )
{
    this->pfb->RecomputeRenderWindow();

    if( this->delayRender ) return;

    switch( this->decorationStyle ) {
    case( kDecoration_C64 ):
        this->pfb->DrawBorder( this->decorationSize, this->decorationColor );
        this->pfb->DrawHLine( 10, 10, 16, 4 );
        this->pfb->DrawVLine( 5, 5, 50, 5 );
        this->pfb->DrawFilledBox( 1, 1, 10, 10, 1 );
        break;

    case( kDecoration_Amiga1 ):
        this->pfb->DrawAmiga1Border();
        break;

    case( kDecoration_Amiga2 ):
        this->pfb->DrawAmiga2Border();

        break;

    default:
        this->pfb->DrawBorder( -1, 0 );
        break;
    }

    /* the text buffer has changed, we'll just refresh the graphicstext render */
    /* XXXYYY
    this->pfb->RenderTextColorBuffer( this->tb->GetColor(),
                                      this->tb->GetText(),
                                      this->hidePrompt? -10 : this->tb->GetCursorX() );
                                      */
    this->pfb->RenderScreen();
}

/* this is called when a new font is loaded */
void MainWindow::FontHasLoaded( )
{
    this->pfb->SetFont( this->fnt->GetLFont() );
    this->TextBufferHasChanged();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::ThemeHasLoaded()
{
    this->LoadRequested();  /* force everyone to reload their settings */
    this->TextBufferHasChanged();
}


///////////////////////////////////////////////////////////
/// Slots for load/save

void MainWindow::SaveRequested( void )
{
    SETUPSETTINGS();
    s.setValue( kSettings_AppVersion, kLlamaTermVersion );
    s.setValue( kSettings_LocalEcho, this->localEcho );
    s.setValue( kSettings_DecorationStyle, this->decorationStyle );
    s.setValue( kSettings_DecorationSize, this->decorationSize);
    s.setValue( kSettings_DecorationColor, this->decorationColor );
    emit Save();
}

void MainWindow::LoadRequested( void )
{
    SETUPSETTINGS();
    this->localEcho = s.value( kSettings_LocalEcho, false ).toBool();
    this->decorationStyle = s.value( kSettings_DecorationStyle, "0" ).toInt();
    this->decorationSize = s.value( kSettings_DecorationSize, "none" ).toInt();
    this->decorationColor = s.value( kSettings_DecorationColor, "none" ).toInt();
    emit Load();
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::ShowReady( void )
{
    this->delayRender = true;

    // bars with version text
    tp->SetPen( 0x09, 0x09 ); tp->AddCharacters( ' ', 40 );
    tp->SetPen( 0x01, 0x00 ); tp->AddText( "\n             LlamaTerm " kLlamaTermVersion "\n" );
    tp->SetPen( 0x09, 0x09 ); tp->AddCharacters( ' ', 40 );

    // helpful
    tp->SetPen( 0x0e, 0x00 ); tp->AddText( "\n\nPress [alt] for menu.\n" );

    // ready!
    tp->SetPen( 0x05, 0x00 ); tp->AddText( "\nReady!\n\n" );

    tp->SetPen( 0x0d, 0x00 );

    this->delayRender = false;
    this->TextBufferHasChanged();
}


void MainWindow::ShowDebug( int items )
{
    this->delayRender = true;

    tp->AddText( "\n\n" );

    if( items & kDebugItem_Colors )
    {
        for( int m=0 ; m < 1 ; m++ ) {
            for( int k=0 ; k<=0x0f ; k++ ) {
                tp->SetPen( 1, k );
                char buf[5];
                snprintf( buf, 5, " %x", k );
                tp->AddText( buf );
            }
            tp->AddCharacter( '\n' );
        }
        tp->AddCharacter( '\n' );
    }

    this->delayRender = false;
    this->TextBufferHasChanged();
}



QString MainWindow::DecorationName( void )
{
    if( this->decorationStyle == kDecoration_None )   return "None";
    if( this->decorationStyle == kDecoration_C64 )    return "C= 64";
    if( this->decorationStyle == kDecoration_Amiga1 ) return "Amiga 1.x";
    if( this->decorationStyle == kDecoration_Amiga2 ) return "Amiga 2.x";
    return "Unknown";
}

void MainWindow::ToggleDecoration( void )
{
    this->decorationStyle++;
    if( this->decorationStyle > kMaxDecorations ) this->decorationStyle = 0;
    this->TextBufferHasChanged();
    this->SaveRequested();
}


void MainWindow::ToggleDecorationColor( void )
{
    this->decorationColor++;
    this->decorationColor &= 0x0F;

    this->TextBufferHasChanged();
    this->SaveRequested();
}

QString MainWindow::DecorationColorAsString( void )
{
    QString str = "Dec Color: " + QString::number( this->decorationColor, 16 );
    return str;
}


void MainWindow::ToggleDecorationSize( void )
{
    this->decorationSize +=2 ;
    this->decorationSize &= 0x01F;

    this->TextBufferHasChanged();
    this->SaveRequested();
}

QString MainWindow::DecorationSizeAsString( void )
{
    QString str = "Dec Size: " + QString::number( this->decorationSize ) + " px";
    return str;
}

///////////////////////////////////////////////////////////////////////////

/*
 VT100/320 menu configurations:

 Set-Up
    Display
        80/132
        (No) Auto Wrap
        (Smooth|Jump) Scroll
        Light Text, Dark Screen
        (No) Cursor
        (Block|Underline) Cursor Style
        (No) Status Display
    General
        VT100|VT300 Mode
        Terminal ID (Vt320 ID, VT100 ID)
    Keyboard
        (No) New Line - Ret sends CR or CRLF
        (Backspace|Delete)
        (No) Keyclick
        (No) Margin Bell
        (No) Warning Bell ^G
    Comm
        Speed = (1200|2400)
        XOFF
        Data Bits / Parity
        _Bits, _Parity
        (1|2) Stop bit(s)
        (No) Local Echo
    On Line (Ready|Not Ready)
    Reset Terminal
    Clear Display
 */

#define kMenu_Main     (0)
#define kMenu_Serial   (1)
#define kMenu_Font     (2)
#define kMenu_Video    (3)
#define kMenu_Terminal (4)
#define kMenu_Theme    (5)
#define kMenu_Quit     (6)

void MainWindow::MenuStart( QString title, bool special )
{
    this->delayRender = true;

    if( special ) this->tp->SetPen( 0x00, 0x07 );
    else  this->tp->SetPen( 0x07, 0x00 );
    this->tp->AddText( "\n== " + title + " ==\n" );
    this->tp->SetPen( 0x01, 0x00 );
}

void MainWindow::MenuOption( char ch, QString text )
{
    QString c( ch );
    this->tp->AddText( "  " + c + ": " + text + "\n" );
}

void MainWindow::MenuEnd()
{
    this->delayRender = false;
    this->TextBufferHasChanged();
}

void MainWindow::DisplayMenu( void )
{

    switch( this->menuID ) {

    case( kMenu_Main ):
        this->MenuStart( "Set-Up" );
        this->MenuOption( 'd', "Debug" );
        this->MenuOption( 's', "Serial" );
        this->MenuOption( 'f', "Font" );
        this->MenuOption( 'v', "Video" );
        this->MenuOption( 't', "Terminal" );
        this->MenuOption( 'm', "Theme" );
        this->MenuOption( 'x' );
        this->MenuOption( 'q', "Quit LlamaTerminal" );
        this->MenuEnd();
        break;

    case( kMenu_Quit ):
        this->MenuStart( "Confirm Exit?", true );
        this->tp->SetPen( 0x07, 0x00 );
        this->MenuOption( 'y', "Yes. Exit" );
        this->MenuOption( 'n', "No. Return." );
        this->MenuEnd();
        break;

    case( kMenu_Terminal ):
        this->MenuStart( "Terminal" );
        this->MenuOption( 'c', this->pfb->GetPromptString() + " cursor" );
        this->MenuOption( 'e', this->localEcho? "Local echo" : "No echo" );
        this->MenuOption( 'w', "wrap lines: " + this->tp->GetWrapString()  );
        this->MenuOption( 'x' ) ;
        this->MenuEnd();
        break;

    case( kMenu_Serial ):
        this->MenuStart( "Serial" );
        this->MenuOption( 'd', this->si->GetDeviceNameString() );
        if( this->si->GetDeviceInUse() == true ) {
            this->MenuOption( ' ', "     (unavailable)");
        } else {
            //this->MenuOption( ' ', this->si->GetDeviceDescriptionString() );
            this->MenuOption( 'c', this->si->GetConnected()? "Connected" : "Not connected" );

            if( !this->si->GetConnected() ) {
                this->MenuOption( 'r', QString::number( si->GetBaud() ) + " baud");
                this->MenuOption( 'b', QString::number( si->GetBits() ) + " bits");
                this->MenuOption( 'p', si->GetParityString() + " parity");
                this->MenuOption( 's', si->GetStopString() + " stop");
                this->MenuOption( 'f', si->GetFlowControlString() + " flow ctrl");
            }
        }
        this->MenuOption( 'x' );
        this->MenuEnd();
        break;

    case( kMenu_Font ):
        this->MenuStart( "Font" );
        this->MenuOption( 'd', "Pick fonts directory" );
        this->MenuOption( 'f', this->fnt->GetFontName() );
        this->MenuOption( 'h', this->pfb->GetDoubleHoriz() ? "Horiz Double" : "Horiz Single" );
        this->MenuOption( 'v', this->pfb->GetDoubleVert() ? "Vert Double" : "Vert Single" );
        this->MenuOption( 's', this->pfb->GetScanLines() ? "Scanlines" : "No Scanlines" );
        this->MenuOption( '1', QString::number( this->pfb->GetHSpacing() ) + " px H Space" );
        this->MenuOption( '2', QString::number( this->pfb->GetVSpacing() ) + " px V Space" );
        this->MenuOption( 'x' );
        this->MenuEnd();
        break;

    case( kMenu_Video ):
        this->MenuStart( "Video" );
        this->MenuOption( 'd', "decoration: " + this->DecorationName() );
        this->MenuOption( 'c', this->DecorationColorAsString() );
        this->MenuOption( 's', this->DecorationSizeAsString() );

        this->MenuOption( 'p', this->pfb->GetPaletteString() );
        this->MenuOption( 'x' );
        this->MenuEnd();
        break;

    case( kMenu_Theme ):
        this->MenuStart( "Theme" );
        this->MenuOption( 'd', "Pick theme directory" );
        this->MenuOption( 't', this->theme->GetThemeName() );
        this->MenuOption( 'x' );
        this->MenuEnd();
        break;

    default:
        break;
    }
}



void MainWindow::MenuModeKeyPress( QKeyEvent * e )
{
    int ch = e->text().toUtf8()[0];

    this->tp->SetPen( 0x0A, 0x00 );
    this->tp->AddText( "<" + e->text() + ">\n");

    switch( this->menuID ) {
    case( kMenu_Main ): /* ******** Main Menu ******** */
        switch( ch ) {
        case( 'd' ): this->ShowDebug( kDebugItem_EVERYTHING ); break;
        case( 's' ): this->menuID = kMenu_Serial; break;
        case( 't' ): this->menuID = kMenu_Terminal; break;
        case( 'v' ): this->menuID = kMenu_Video; break;
        case( 'f' ): this->menuID = kMenu_Font; break;
        case( 'm' ): this->menuID = kMenu_Theme; break;
        case( 'q' ): this->menuID = kMenu_Quit; break;

        case( 'x' ):
            this->runMode = kRunMode_Serial;
            this->tp->SetPen( 0x01, 0x00 );
            this->tp->AddText( "Text mode.\n");
            break;

        default:
            this->tp->AddText( "?\n" );
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
            this->tp->AddText( "?\n" );
            break;
        }
        break;

    case( kMenu_Terminal ): /* ******** Terminal Menu ******** */
        switch( ch ) {
        case( 'c' ): this->pfb->TogglePromptType(); break;
        case( 'e' ): this->localEcho = (this->localEcho)?0:1; break;
        case( 'w' ): this->tp->ToggleWrap(); break;

        case( 'x' ): this->menuID = kMenu_Main; break;
        default:
            this->tp->AddText( "?\n" );
            break;
        }
        break;

    case( kMenu_Video ): /* ******** Video Menu ******** */
        switch( ch ) {
        case( 'd' ): this->ToggleDecoration(); break;
        case( 'c' ): this->ToggleDecorationColor(); break;
        case( 's' ): this->ToggleDecorationSize(); break;

        case( 'p' ):     /* change palette */
            this->pfb->TogglePalette();
            this->ShowDebug( kDebugItem_Colors );
            break;

        case( 'x' ): this->menuID = kMenu_Main; break;
        default:
            this->tp->AddText( "?\n" );
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
            this->tp->AddText( "?\n" );
            break;
        }
        break;

    case( kMenu_Theme ): /* ******** Theme Menu ******** */
        switch( ch ) {
        case( 'd' ): this->theme->RequestThemeDirectory(); break;
        case( 't' ): this->theme->ToggleTheme(); break;
        case( 'x' ): this->menuID = kMenu_Main; break;
        default:
            this->tp->AddText( "?\n" );
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
}

///////////////////////////////////////////// internal lipsum for testing

const char * const lipsum[] = {
  "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur",
  "adipiscing", "elit.", "Pellentesque", "et", "vulputate",
  "velit.", "Pellentesque", "tempor", "ligula", "id", "dolor",
  "gravida", "convallis.", "Duis", "sed", "dignissim", "lectus.",
  "Nulla", "tempus", "nunc", "a", "maximus", "sodales.", "Phasellus",
  "rhoncus", "finibus", "suscipit.", "Donec", "euismod", "volutpat",
  "erat,", "aliquet", "auctor", "ipsum", "vehicula", "vitae.",
  "Sed", "nibh", "turpis,", "volutpat", "in", "volutpat", "nec,",
  "viverra", "non", "purus.", "Proin", "malesuada", "lobortis",
  "elit,", "in", "porta", "nulla", "laoreet", "et.", "Phasellus",
  "ligula", "turpis,", "pellentesque", "sed", "magna", "congue,",
  "efficitur", "ultrices", "tortor.", "Morbi", "leo", "dui,",
  "vestibulum", "ac", "eleifend", "tempus,", "consequat", "eget",
  "felis.", "Cras", "ante", "elit,", "vestibulum", "at", "tortor",
  "non,", "ullamcorper", "rutrum", "odio.", "", "Donec", "ac",
  "urna", "eget", "lacus", "mollis", "gravida", "sed", "sed",
  "neque.", "In", "hac", "habitasse", "platea", "dictumst.",
  "Donec", "id", "posuere", "nibh.", "Donec", "ac", "diam", "nec",
  "augue", "blandit", "rhoncus", "sit", "amet", "sed", "risus.",
  "Praesent", "quis", "ullamcorper", "nibh,", "eu", "rhoncus",
  "tortor.", "Praesent", "pulvinar", "tristique", "ornare.",
  "Curabitur", "non", "lectus", "nec", "turpis", "tincidunt",
  "aliquet", "vitae", "at", "ex.", "Proin", "leo", "nisi,",
  "convallis", "vel", "auctor", "sed,", "imperdiet", "ac",
  "tortor.", "Sed", "dignissim", "nulla", "vel", "tortor",
  "interdum,", "non", "tempus", "orci", "imperdiet.", "Donec",
  "convallis", "nibh", "quis", "ligula", "feugiat,", "eget",
  "dictum", "urna", "vulputate.", "", "Nam", "vehicula,", "leo",
  "a", "dignissim", "tempus,", "purus", "orci", "pellentesque",
  "lectus,", "nec", "porta", "magna", "neque", "at", "augue.",
  "Pellentesque", "lacinia", "blandit", "risus,", "ac", "vestibulum",
  "odio", "scelerisque", "nec.", "Phasellus", "sed", "ultricies",
  "dolor.", "Nunc", "vehicula", "mi", "mi,", "in", "pellentesque",
  "nisi", "dignissim", "eget.", "Proin", "sodales,", "ex", "a",
  "blandit", "finibus,", "arcu", "velit", "mattis", "purus.",
  "vitae,", "suscipit", "ex", "lacus", "ut", "elit.", "Quisque",
  "feugiat", "non", "tellus", "quis", "cursus.", "Nulla", "quis",
  "dolor", "vel", "mi", "faucibus", "egestas.", "", "Vivamus",
  "cursus", "quis", "nulla", "quis", "euismod.", "Fusce", "gravida",
  "bibendum", "dolor", "in", "vehicula.", "Proin", "mauris",
  "felis,", "ultricies", "quis", "rhoncus", "ac,", "fermentum",
  "at", "lacus.", "Sed", "porta", "leo", "magna,", "a", "finibus",
  "odio", "aliquam", "et.", "Ut", "pellentesque", "varius", "est",
  "nec", "lobortis.", "Nam", "id", "massa", "molestie,", "mollis",
  "nisl", "id,", "mattis", "elit.", "Vivamus", "elementum", "ut",
  "massa", "interdum", "feugiat.", "Suspendisse", "elementum",
  "erat", "ac", "odio", "tempus,", "ac", "congue", "nunc",
  "malesuada.", "Vivamus", "ac", "ligula", "condimentum", "amet.",
  NULL
};

void MainWindow::addLipsum( void )
{
    static int idx = 0;
    int cols = 0;
    int rows = 0;

    while( rows < 30 ) {
        this->tp->AddText( lipsum[idx] );
        this->tp->AddText( " " );

        cols += (strlen( lipsum[idx] ) + 1 );

        if( cols > 55 ) {
            this->tp->AddText( "\n" );
            cols = 0;
            rows++;
        }
        idx++;

        if( lipsum[idx] == NULL ) {
            idx = 0;
        }
    }
}

///////////////////////////////////////////// Keyboard events

/* this is called when a key is pressed
 *  No need to Connect() to this, it just magically happens
 */
void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if( e->modifiers() & Qt::AltModifier){
        if( this->runMode == kRunMode_Menu ) {
            this->runMode = kRunMode_Serial;

            this->tp->SetPen( 0x01, 0x00 );
            this->tp->AddText( "\nText mode.\n");
        } else {
            this->runMode = kRunMode_Menu;
            this->menuID = kMenu_Main;

            this->DisplayMenu();
            return;
        }
    }

    if( e->modifiers() & Qt::ShiftModifier){
        if( this->runMode == kRunMode_Serial ) {
            this->addLipsum();
        }
    }


    if( e->modifiers() & Qt::ControlModifier){

        qDebug() << "ctrl " << e->text().toUtf8() << "pressed";

        return;
    }

    if( this->runMode == kRunMode_Menu ) {
        this->MenuModeKeyPress( e );

    } else {
        /* serial mode */
        if( this->localEcho ) {
            for( int j=0 ; j<e->text().length() ; j++ ) {
                tp->AddCharacter( e->text().toUtf8().at( j ) );
            }
        }
        this->si->Send( e->text().toUtf8() );
    }
}

//////////////////////////////////////////
/// Mouse Events (for shagdraw!)
///

static Qt::MouseButtons m_butts;
static int lastmx = -1;
static int lastmy = -1;

void MainWindow::resizeEvent(QResizeEvent * e)
{
    qDebug() << "Resize " << e;
}

void MainWindow::mouseMoveEvent( QMouseEvent * e )
{
    int x = (int)e->localPos().x();
    int y = (int)e->localPos().y();

    if( m_butts & Qt::LeftButton ) {
        if( lastmx >= 0 )
            this->pfb->DrawLine( lastmx, lastmy, x, y, 0x02 );
    }
    if( m_butts & Qt::RightButton ) {
        if( lastmx >= 0 )
            this->pfb->DrawLine( lastmx, lastmy, x, y, 0x03 );
    }
    lastmx = x;
    lastmy = y;
}

void MainWindow::mousePressEvent( QMouseEvent * e )
{
    lastmx = lastmy = -1;
    m_butts = e->buttons();
}

void MainWindow::mouseReleaseEvent( QMouseEvent * e )
{
    m_butts = e->buttons();
    lastmx = lastmy = -1;
}


//////////////////////////////////////////
/// Timer events
///
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
