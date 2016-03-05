#include "textbuffer.h"
#include <QDebug>

#define kMaxCols (256)
#define kMaxRows (256)


TextBuffer::TextBuffer( QObject *parent )
    : QObject( parent )
    , rows( kMaxRows )
    , cols( kMaxCols )
    , text( NULL )
    , color( NULL )
    , textX( 0 )
    , pen( 0 )
{
    this->Setup( this->rows, this->cols );
}

TextBuffer::~TextBuffer()
{
    this->UpSet();
}


/* ************************************************************* */

void TextBuffer::Setup( int rows, int cols )
{
    this->UpSet();

    this->text = new unsigned char[ rows * cols ];
    this->color = new unsigned char[ rows * cols ];
    this->Clear();
}

void TextBuffer::UpSet()
{
    if( this->text )
        delete( this->text );
    this->text = NULL;

    if( this->color )
            delete( this->color );
    this->color = NULL;
}

void TextBuffer::Clear( void )
{
    this->textX = 0;
    for( int j=0 ; j< (rows * cols) ; j++ )
    {
        this->text[ j ] = '\0';
        this->color[ j ] = 0x00;
    }
}

void TextBuffer::Dump( void )
{
    char buf[ 256 ];

    for( int j=0 ; j<10 ; j++ )
    {
        unsigned char * str = (text+(256 * j));
        snprintf( buf, 256, "%d %lu: %s", j, strlen( (const char *)str ), str );
        qDebug() << buf;
    }
}

/* ************************************************************* */

void TextBuffer::ScrollOneRow( bool suppressSignal )
{
    int y=0;
    int x=0;

    // NOTE: fixed at 256 max width for now.
    for( y=255 ; y>0 ; y-- )
    {
        int yp = y * 256;

        for( x=0 ; x<256 ; x++ )
        {
            this->text[ yp + x ] = this->text[ (yp-256) + x ];
            this->color[ yp + x ] = this->color[ (yp-256) + x ];
        }
    }

    for( int x=0 ; x<255 ; x++ ) {
        this->text[ x ] = '\0';
        this->color[ x ] = this->pen;
    }

    if( !suppressSignal ) {
        emit this->TextHasChanged();
    }
}

void TextBuffer::AddCharacter( char ch )
{
    if( ch == 0x0d ) { /* CR */
        //this->ScrollOneRow( true /* we'll emit the signal here */ );
        this->textX = 0;

    } else if( ch == 0x0a ) { /* LF */
        this->ScrollOneRow( true /* we'll emit the signal here */ );
        this->textX = 0;

    } else {
        this->text[ this->textX  ] = ch;
        this->color[ this->textX ] = this->pen;

        this->textX++;
    }

    emit this->TextHasChanged();
}
