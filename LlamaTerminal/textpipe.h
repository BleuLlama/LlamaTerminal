/*
 * TextPipe
 *
 *  Handles sending the text received to the buffers for display and logging
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

#ifndef TEXTPIPE_H
#define TEXTPIPE_H

#include <QObject>
#include "palettedframebuffer.h"

class TextPipe : public QObject
{
    Q_OBJECT
public:
    explicit TextPipe(QObject *parent = 0);
    ~TextPipe();

private:
    int wrap;
#define kWrap_Off   (0)
#define kWrap_On    (1)

public:
    int GetWrap() { return this->wrap; }
    void SetWrap( int w ) { this->wrap = w; }
    void ToggleWrap() { this->wrap = (this->wrap)?0:1; }
    QString GetWrapString() { if( this->wrap ) return "On"; else return "Off"; }

private:
    PalettedFrameBuffer * pfb;
public:
    void setPfb( PalettedFrameBuffer * p );

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


private: /* text positioning */
    int textCursorX;
    int textCursorY;
    int textCols;
    int textRows;

public:
    // move cursor to 0,0
    void CursorHome() { this->textCursorX = 0; this->textCursorY = 0; }

    // clear the text area
    void TextClear();

    // scroll the text area by one text line
    void TextScroll();

    // handle new line
    void AddNewline();
    // handle new character
    void AdvanceCursor();


    // add characters to the text area
    void AddCharacter( const unsigned char ch );

    // render the character down into the PFB
    void DrawCharacter( unsigned char pen, const unsigned char ch );

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
signals:
    void TextHasChanged();

public slots:
    void SaveSettings( void );
    void LoadSettings( void );

};

#endif // TEXTPIPE_H
