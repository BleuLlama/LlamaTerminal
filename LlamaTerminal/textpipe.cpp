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

#include "textpipe.h"
#include "globals.h"

#include <qdebug.h>
#include <math.h>

TextPipe::TextPipe(QObject *parent)
    : QObject(parent)
    , pfb( NULL )
    , textCursorX( 0 )
    , textCursorY( 0 )
    , textCols( 2 )
    , textRows( 2 )
{
}


TextPipe::~TextPipe()
{
}

// this class will:
//  - handle injecting newlines for word/character wrap
//  - forwarding on the characters and newlines (separate) to pfb
//  -


////////////////////////////////////////////////////////////////

void TextPipe::setPfb( PalettedFrameBuffer * p )
{
    this->pfb = p;
    if( !this->pfb ) return;

    // make sure it has the right values for the window.
    this->pfb->RecomputeRenderWindow();

    // generate the number of columns and rows we can work with
    this->textCols = (int) floor( (double) this->pfb->windowW / (double) this->pfb->GetGlyphW() );
    qDebug() << "cols " << this->textCols <<    "  " << this->pfb->windowW << "  " << this->pfb->GetGlyphW();
    this->textRows = (int) floor( (double) this->pfb->windowH / (double) this->pfb->GetGlyphH() );
}


// clear the text area
void TextPipe::TextClear()
{
    if( this->pfb ) {
        this->pfb->Fill( this->pen >> 4 );
    }
    this->textCursorX = 0;
    this->textCursorY = 0;
}

// scroll the text area by one text line
void TextPipe::TextScroll()
{
    if( this->pfb ) {
        //int r;
        //int glyphW = this->GetGlyphW();
        int glyphH = this->pfb->GetGlyphH();

        /* copy the pixels up */
        this->pfb->BlitWindowUp( glyphH );

        /* then fill the new space */
        int screenX = this->pfb->borderW;
        int screenY = this->pfb->borderN + ( this->pfb->GetGlyphH() * this->textCursorY );

        this->pfb->DrawFilledBox( screenX, screenY,
                                  this->pfb->windowW,
                                  this->pfb->windowH + this->pfb->borderS - screenY,
                                  this->pen >> 4 );

        this->pfb->DrawBorder(); // NO. SHOULD BE HANDLED BY PFB
    }
}


void TextPipe::AddNewline()
{
    if( this->pfb ) {

        this->textCursorX = 0;
        this->textCursorY++;

        // check for scrolly
        if( this->textCursorY >= (this->textRows-1) )
        {
            this->textCursorY--;
            this->TextScroll();
        }
    }
}

void TextPipe::AdvanceCursor()
{
    // todo: add word wrap in here according to user settings
    this->textCursorX++;

    if( this->textCursorX > this->textCols )
    {
        this->AddNewline();
    }
}

void TextPipe::DrawCharacter( unsigned char pen, const unsigned char ch )
{
    if( !this->pfb ) return;

    int screenX = ( this->pfb->GetGlyphW() * this->textCursorX ) + this->pfb->borderW;
    int screenY = ( this->pfb->GetGlyphH() * this->textCursorY ) + this->pfb->borderN;

    this->pfb->DrawCharacter( screenX, screenY, pen, ch );
}

// add characters to the text area
void TextPipe::AddCharacter( const unsigned char ch )
{
    int fgfgPen = ((this->pen & 0x0f) << 4) | (this->pen & 0x0f);

    this->DrawCharacter( this->pen, ' ' );

    switch( ch ) {
    case( '\r' ):
        //break; /* absorb for now... */

    case( '\n' ):
        /* advance the vertical */
        this->AddNewline();
        this->textCursorX = 0;
        break;

    default:
        /* draw it */
        this->DrawCharacter( this->pen, ch );

        /* and advance the cursor */
        this->AdvanceCursor();

        break;
    }

    /* Draw the prompt */
    this->DrawCharacter( fgfgPen, '#' );
}


////////////////////////////////////////////////////////////////

void TextPipe::SaveSettings()
{
    SETUPSETTINGS();
    s.setValue( kSettings_TextPen, this->pen );
    s.setValue( kSettings_TextWrap, this->wrap );
}

void TextPipe::LoadSettings()
{
    SETUPSETTINGS();

    this->pen = (unsigned char) s.value( kSettings_TextPen, 0x10 ).toUInt();
    this->wrap = (unsigned char) s.value( kSettings_TextWrap, 1 ).toUInt();
}
