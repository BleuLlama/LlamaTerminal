#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pfb( NULL )
    , tb( NULL )
    , runMode( kRunMode_Serial )
    , delayRender( false )
{
    ui->setupUi(this);

    this->pfb = new PalettedFrameBuffer( this );
    this->connect( this->pfb, SIGNAL(ScreenIsRendered()), this, SLOT( ScreenHasBeenUpdated() ) );

    this->pfb->FillWithPattern( kDisplayPatternDiagonals );

    unsigned char pens[] = { 0x10, 0x01, 0x23, 0x34, 0x45, 0x46, 0x99 };
    this->pfb->DrawText( 30, 30, pens, "Hello!" );
    this->pfb->RenderScreen();

    this->tb = new TextBuffer( );
    this->connect( this->tb, SIGNAL(TextHasChanged()), this, SLOT( TextBufferHasChanged() ) );

    ////////////////////////////////////////
    for( int m=0 ; m < 3 ; m++ ) {
        for( int k=0 ; k<=0x0f ; k++ ) {
            tb->SetPen( 1, k );
            char buf[5];
            snprintf( buf, 5, " %x", k );
            tb->AddText( buf );
        }
        tb->AddCharacter( '\n' );
    }
    tb->AddCharacter( '\n' );

    tb->SetPen( 0x09, 0x09 ); tb->AddCharacters( ' ', 40 );
    tb->SetPen( 0x01, 0x00 ); tb->AddText( "\n             LlamaTerm v0.01\n" );
    tb->SetPen( 0x09, 0x09 ); tb->AddCharacters( ' ', 40 );

    tb->SetPen( 0x0e, 0x00 ); tb->AddText( "\n\nPress [alt] for menu.\n" );

    tb->SetPen( 0x05, 0x00 ); tb->AddText( "\nReady.\n\n" );

    tb->SetPen( 0x0d, 0x00 );
}

/* when we shut down, this gets called */
MainWindow::~MainWindow()
{
    //delete this->pfb;
    //delete this->tb;
    delete ui;
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
    this->pfb->RenderTextColorBuffer( this->tb->GetColor(), this->tb->GetText(), this->tb->GetCursorX() );
}


#define kMenu_Main   (0)
#define kMenu_Serial (1)
#define kMenu_Video  (2)

void MainWindow::DisplayMenu( void )
{
    this->delayRender = true;
    this->tb->SetPen( 0x07, 0x00 );
    this->tb->AddText( "\n" );

    switch( this->menuID ) {
    case( kMenu_Main ):
        this->tb->AddText( "Main Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  s: serial\n");
        this->tb->AddText( "  v: video settings\n");
        this->tb->AddText( "  x: exit menu (or alt)\n");
        this->tb->AddText( "  q: quit LlamaTerminal\n");
        break;

    case( kMenu_Serial ):
        this->tb->AddText( "Serial Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  c: Not connected\n");
        this->tb->AddText( "  d: <device>\n");

        this->tb->AddText( "  e: ");
            this->tb->AddText( this->localEcho? "Local echo\n" : "No echo\n" );
        this->tb->AddText( "  r: 9600\n");
        this->tb->AddText( "  b: 8\n");
        this->tb->AddText( "  p: n\n");
        this->tb->AddText( "  s: 1\n");

        this->tb->AddText( "  x: exit menu\n");
        break;

    case( kMenu_Video ):
        this->tb->AddText( "Video Menu:\n");
        this->tb->SetPen( 0x01, 0x00 );

        this->tb->AddText( "  f: font: internal\n");
        this->tb->AddText( "  c: clear\n" );
        this->tb->AddText( "  h: " );
            this->tb->AddText( this->pfb->GetDoubleHoriz() ? "Horiz Double\n" : "Horiz Single\n" );
        this->tb->AddText( "  v: ");
            this->tb->AddText( this->pfb->GetDoubleVert() ? "Vert Double\n" : "Vert Single\n" );
        this->tb->AddText( "  s: ");
            this->tb->AddText( this->pfb->GetScanLines() ? "Scanlines\n" : "No Scanlines\n" );
        this->tb->AddText( "  x: exit menu\n");
        break;
    }
    this->delayRender = false;
    this->TextBufferHasChanged();
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
        }
    }

    if( e->modifiers() & Qt::ControlModifier){
        qDebug() << "ctrl " << e->text().toUtf8() << "pressed";
        return;
    }

    if( this->runMode == kRunMode_Menu ) {
        int ch = e->text().toUtf8()[0];

        switch( this->menuID ) {
        case( kMenu_Main ):
            switch( ch ) {
            case( 's' ): this->menuID = kMenu_Serial; break;
            case( 'v' ): this->menuID = kMenu_Video; break;

            case( 'x' ):
                this->runMode = kRunMode_Serial;
                this->tb->SetPen( 0x01, 0x00 );
                this->tb->AddText( "Text mode.\n");
                break;

            case( 'q' ): exit( 0 );

            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        case( kMenu_Serial ):
            switch( ch ) {
            case( 'c' ): break; /* connect/disconnect */
            case( 'd' ): break; /* select next device */
            case( 'r' ): break; /* baud rate */
            case( 'b' ): break; /* data bits */
            case( 'p' ): break; /* parity */
            case( 's' ): break; /* stop bits */

            case( 'e' ): this->localEcho = (this->localEcho)?0:1; break;

            case( 'x' ): this->menuID = kMenu_Main; break;
            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        case( kMenu_Video ):
            switch( ch ) {
            case( 'f' ): break; /* font */
            case( 'c' ): this->tb->Clear(); break;
            case( 'h' ): this->pfb->ToggleDoubleHoriz(); break;
            case( 'v' ): this->pfb->ToggleDoubleVert(); break;
            case( 's' ): this->pfb->ToggleScanLines(); break;

            case( 'x' ): this->menuID = kMenu_Main; break;
            default:
                this->tb->AddText( "?\n" );
                break;
            }
            break;

        default:
            break;
        }

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
        //serial->write( e->text().toUtf8() );
    }
}
