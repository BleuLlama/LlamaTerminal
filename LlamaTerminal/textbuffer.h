/* 
 * text buffer header
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
#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include <QObject>
#include "globals.h"

class TextBuffer : public QObject
{
    Q_OBJECT

public:
    TextBuffer(QObject *parent = 0);
    ~TextBuffer();

private:
    int rows;
    int cols;
    unsigned char * text;
    unsigned char * color;

public:
    void Setup( int rows, int cols );
    void UpSet( void );

    int GetRows( void ) { return this->rows; }
    int GetCols( void ) { return this->cols; }
    unsigned char * GetText( void ) { return this->text; }
    unsigned char * GetColor( void ) { return this->color; }


    /* text buffer interactions */
private:
    long textX;

public:
    /* scroll the text by one row */
    void ScrollOneRow( bool suppressSignal = false );

    /* add a character */
    void AddCharacter( char ch );

    void AddCharacters( char ch, long count ) {
        while( count-- ) this->AddCharacter( ch );
    }

    /* add a null-terminated string */
    void AddText( char * str ) {
        if( !str ) return;
        while( *str ) { this->AddCharacter( *str++ ); }
    }

    /* some helpers defined here in the class definition... */
    void AddText( const char * str ) {
        this->AddText( (char *) str );
    }
    void AddText( QString str ) {
        this->AddText( str.toUtf8().data() );
    }

    int GetCursorX( void ) { return this->textX; }

    void Clear( void );

    void Dump( void );

    /* pen is an unsigned char where the bottom nibble is the fg, background is shifted
     * 0xBF
     * All of the pen interaction functions are here in the class definition...
     */
private:
    unsigned char pen;

public:
    void SetPen( unsigned char pen ) {
        this->pen = pen;
    }

    void SetPen( unsigned char fg, unsigned char bg ) {
        this->SetPen( (fg & 0x0f) | ((bg & 0x0f)<<4) );
    }

    unsigned char GetPen() { return this->pen; }
    unsigned char GetFGPen() { return (this->pen & 0x0F); }
    unsigned char GetBGPen() { return ((this->pen >>4) & 0x0F ); }


signals:
    void TextHasChanged();

public slots:
    void SaveSettings( void ) { qDebug() << "Text save"; }
    void LoadSettings( void ) { qDebug() << "Text load"; }

};

#endif // TEXTBUFFER_H
