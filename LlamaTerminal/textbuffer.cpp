/* 
 * text buffer
 *
 *  maintains the text buffer and settings
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

////////////////////////////////////////////////////////////////


void TextBuffer::SaveSettings()
{
    SETUPSETTINGS();
    s.setValue( "Text.Pen", this->pen );
}

void TextBuffer::LoadSettings()
{
    SETUPSETTINGS();

    this->pen = (unsigned char) s.value( "Text.pen", 0x10 ).toUInt();
}
