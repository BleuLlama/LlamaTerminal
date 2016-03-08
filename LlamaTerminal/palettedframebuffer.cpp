/* 
 * paletted frame buffer
 *
 *  an indexed frame buffer
 *  - with multiple palette support
 *  - hooks for settings
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

#include <Qdebug>
#include "palettedframebuffer.h"
#include "font.h"


////////////////////////////////////////////////////////////

// ref: https://en.wikipedia.org/wiki/List_of_8-bit_computer_hardware_palettes

const PALENT colorPalette [] =
{
    // C=64 Palette
    {   0,   0,   0 },  // 0x00: black
    { 255, 255, 255 },  // 0x01: white
    { 136,   0,   0 },  // 0x02: red
    { 170, 255, 238 },  // 0x03: cyan
    { 204,  68, 204 },  // 0x04: violet
    {   0, 204,  85 },  // 0x05: green
    {   0,   0, 170 },  // 0x06: blue
    { 238, 238, 119 },  // 0x07: yellow
    { 221, 136,  85 },  // 0x08: orange
    { 102,  68,   0 },  // 0x09: brown
    { 225, 119, 119 },  // 0x0A: lt red
    {  51,  51,  51 },  // 0x0B: dk gray
    { 119, 119, 119 },  // 0x0C: gray
    { 170, 255, 102 },  // 0x0D: green
    {   0, 136, 255 },  // 0x0E: blue
    { 187, 187, 187 },  // 0x0F: gray

    // DeluxePaint Palette
    // My 16 color version of the Deluxe Paint palette - NTSC safe ;)
    {   0,   0,   0 },  // 0x10: black
    {  80,  80,  80 },  // 0x11: dk gray
    { 176, 176, 176 },  // 0x12: lt gray
    { 240, 240, 240 },  // 0x13: white
    { 208,  60,  40 },  // 0x14: red
    { 149,  40,  26 },  // 0x15: dk red
    { 152,  86,  43 },  // 0x16: brown
    { 200, 131,  38 },  // 0x17: orange
    { 240, 219,  48 },  // 0x18: yellow
    { 158, 228,  28 },  // 0x19: green
    {  56, 120,   5 },  // 0x1A: dk green
    {  87, 186,  96 },  // 0x1B: blue green
    {  87, 202, 205 },  // 0x1C: cyan
    {   0,  67, 236 },  // 0x1D: blue
    { 174,  80, 222 },  // 0x1E: violet
    { 183,  62, 130 },   // 0x1F: red violet

    // VGA (ANSI layout)
    {   0,   0,   0 }, // 0: black
    { 170,   0,   0 }, // 1: red
    {   0, 170,   0 }, // 2: green
    { 170,  85,   0 }, // 3: brown
    {   0,   0, 170 }, // 4: blue
    { 170,   0, 170 }, // 5: magenta
    {   0, 170, 170 }, // 6: cyan
    { 170, 170, 170 }, // 7: gray
    {  85,  85,  85 }, // 8: light black
    { 255,  85,  85 }, // 9: light red
    {  85, 255,  85 }, // a: light green
    { 255, 255,  85 }, // b: light yellow
    {  85,  85, 255 }, // c: light blue
    { 255,  85, 255 }, // d: light magenta
    {  85, 255, 255 }, // e: light cyan
    { 255, 255, 255 }, // f: white

    // xterm (ANSI layout)
    {   0,   0,   0 }, // 0: black
    { 205,   0,   0 }, // 1: red
    {   0, 205,   0 }, // 2: green
    { 205, 205,   0 }, // 3: brown
    {   0,   0, 238 }, // 4: blue
    { 205,   0, 205 }, // 5: magenta
    {   0, 205, 205 }, // 6: cyan
    { 229, 229, 229 }, // 7: gray
    { 127, 127, 127 }, // 8: light black
    { 255,   0,   0 }, // 9: light red
    {   0, 255,   0 }, // a: light green
    { 255, 255,   0 }, // b: light yellow
    {  92,  92, 255 }, // c: light blue
    { 255,   0, 255 }, // d: light magenta
    {   0, 255, 255 }, // e: light cyan
    { 255, 255, 255 }, // f: white

    // Amiga 1.x
    {  14,  65, 140 }, // 0: blue
    { 236, 236, 236 }, // 1: white
    {   0,   0,   0 }, // 2: black
    { 220, 115,  35 }, // 3: orange
    {  80,  80,  80 }, // 4: gray 80  // from here down are ideas
    { 197, 197, 197 }, // 5: gray 197
    {  10,  20,  55 }, // 6: dk blue
    { 150, 150, 150 }, // 7: gray 150
    { 161,  44,  28 }, // 8: tomato red
    { 255, 255, 255 }, // 9: pure white
    {  71,  16,  38 }, // a: deep burgundy
    { 235, 161,  78 }, // b: light orange/creamsicle
    {  50,  95,  36 }, // c: green
    { 103, 164,  50 }, // d: light green
    {  24,  45, 125 }, // e: blue (**)
    {  76, 117, 215 }, // f: light blue

    // Amiga 2.x
    { 154, 154, 154 }, // 0: gray
    {   0,   0,   0 }, // 1: black
    { 255, 255, 255 }, // 2: white
    {  84, 115, 173 }, // 3: lt blue
    {  25,  42, 250 }, // 4: bright blue
    { 225,  64, 254 }, // 5: bright pink
    {  95, 248, 251 }, // 6: cyan
    { 255, 255, 255 }, // 7: white (*)
    {  95, 248, 251 }, // 8: cyan (*)
    {  83,  83,  83 }, // 9; Gray-83
    { 135, 135, 135 }, // A: Gray-135
    { 154, 154, 154 }, // B: Gray (*)
    { 230,  47,  53 }, // C: Red
    {  93, 219,  69 }, // D: green
    {   2,  40, 212 }, // E: Blue
    { 231, 135,  37 }, // F: Orange

};

typedef struct PALETTE {
    const char * name;
    const PALENT * colors;
    unsigned char defaultFG;
    unsigned char defaultBG;
    unsigned char promptColor;
} PALETTE;

PALETTE pals[] = {
    { "Commodore 64", &colorPalette[16 * 0], 0x03, 0x06, 0x0e },
    { "Deluxe Paint", &colorPalette[16 * 1], 0x03, 0x00, 0x08 },
    { "PC-DOS VGA",   &colorPalette[16 * 2], 0x0f, 0x00, 0x0f },
    { "VT220-G",      &colorPalette[16 * 3], 0x0a, 0x00, 0x02 },
    { "xterm",        &colorPalette[16 * 3], 0x07, 0x00, 0x02 },
    { "Amiga 1.x",    &colorPalette[16 * 4], 0x01, 0x00, 0x03 },
    { "Amiga 2.x",    &colorPalette[16 * 5], 0x01, 0x00, 0x03 },
    { NULL, NULL, 0, 0, 0 }
};


////////////////////////////////////////////////////////////

PalettedFrameBuffer::PalettedFrameBuffer( QObject *parent )
    : QObject(parent)
    , width( kDefaultWidth )
    , height( kDefaultHeight )
    , indexedBuffer( NULL )
    , rgbBuffer( NULL )
    , imageGfx( NULL )
    , palId( 0 )
    , fnt( NULL )
    , defaultFGColor( 1 )
    , defaultBGColor( 0 )
    , defaultPromptColor( 2 )
    , promptType( kPrompt_Block )
    , hSpacing( 0 )
    , vSpacing( 1 )
    , doublevert( 1 )
    , doublehoriz( 1 )
    , scanlines( 0 )
{
    this->Setup( this->width, this->height );
}

PalettedFrameBuffer::~PalettedFrameBuffer()
{
    this->UpSet();
}


////////////////////////////////////////////////////////////

/* Setup
 * Allocate and configure the buffer
 */
void PalettedFrameBuffer::Setup( int w, int h ) {
    this->width = w;
    this->height = h;

    // make sure we're starting fresh
    this->UpSet();
    // allocate our internal graphics buffer (l-r t-b, RGB)
    this->indexedBuffer = new unsigned char[ this->width * this->height ];
    this->rgbBuffer = new unsigned char[ this->width * this->height * 3 ];

    // create the QImage wrapper
    this->imageGfx = new QImage( this->rgbBuffer, this->width, this->height, QImage::Format_RGB888 );

    this->SelectPalette( 0 );

    // Start with a pattern on the screen
    this->FillWithPattern( kDisplayPatternStart );
}

void PalettedFrameBuffer::UpSet( void )
{
    if( this->indexedBuffer ) delete this->indexedBuffer;
    this->indexedBuffer = NULL;

    if( this->rgbBuffer ) delete this->rgbBuffer;
    this->rgbBuffer = NULL;

    if( this->imageGfx ) delete this->imageGfx;
    this->imageGfx = NULL;
}

////////////////////////////////////////////////////////////

void PalettedFrameBuffer::RenderScreen( void )
{
    const PALENT * colors = pals[ this->palId ].colors;

    if( !this->rgbBuffer ) return;

    // 1. convert the indexed buffer to the RGB buffer
    for( int i=0 ; i < (this->width * this->height) ; i++ )
    {
        int paletteIndex = (this->indexedBuffer[i] & 0x0F);
        this->rgbBuffer[ (i*3)+0 ] = colors[ paletteIndex ].r; // r
        this->rgbBuffer[ (i*3)+1 ] = colors[ paletteIndex ].g; // g
        this->rgbBuffer[ (i*3)+2 ] = colors[ paletteIndex ].b; // b
    }

    emit this->ScreenIsRendered();
}

////////////////////////////////////////////////////////////
/*
 * typedef struct PALETTE {
    const char * name;
    const PALENT * colors;
} PALETTE;

PALETTE pals[] = {
    { "Commodore 64", &colorPalette[0] },
    { "Deluxe Paint", &colorPalette[16] },
    { "PC-DOS VGA",   &colorPalette[32] },
    { "xterm",        &colorPalette[48] },
    { NULL, NULL }
};
*/

void PalettedFrameBuffer::TogglePalette( void )
{
    if( this->palId < 0 ) {
        this->palId = 0; // just in case
        return;
    }

    this->palId++;
    if( pals[ this->palId ].name == NULL ) {
        this->palId = 0;
    }

    this->defaultBGColor = pals[ this->palId ].defaultBG;
    this->defaultFGColor = pals[ this->palId ].defaultFG;
    this->defaultPromptColor = pals[ this->palId ].promptColor;
}

QString PalettedFrameBuffer::GetPaletteString()
{
    return pals[ this->palId ].name;
}

////////////////////////////////////////////////////////////

void PalettedFrameBuffer::Set( int x, int y, unsigned char color )
{
    if( x < 0 || x > this->width ) return;
    if( y < 0 || y > this->height ) return;

    this->indexedBuffer[ (y * this->width) + x ] = color;
}

unsigned char PalettedFrameBuffer::Get( int x, int y )
{
    if( x < 0 || x > this->width ) return 0;
    if( y < 0 || y > this->height ) return 0;

    return( this->indexedBuffer[ (y * this->width) + x ] );
}

////////////////////////////////////////////////////////////

void PalettedFrameBuffer::Fill( unsigned char color )
{
    memset( this->indexedBuffer, color, this->width * this->height );
    this->RenderScreen();
}

void PalettedFrameBuffer::FillWithPattern( int patno )
{
    long count = 0;

    switch( patno ) // mmm. i could go for a Switchwich right now.
    {
    case( kDisplayPatternBlank ):
        this->Fill( 0 );
        break;

    case( kDisplayPatternStart ):
        this->Fill( 0x0b );
        break;

    case( kDisplayPatternDiagonals ):
        for( int y=0 ; y < this->height ; y++ )
        {
            for( int x=0 ; x < this->width ; x++ )
            {
                this->Set( x, y, x+y );
            }
        }
        break;

    case( kDisplayPatternStripes ):
        for( int y=0 ; y < this->height ; y++ )
        {
            for( int x=0 ; x < this->width ; x++ )
            {
                this->Set( x, y,  (count + x) &0x0F );
            }
        }
        break;

    case( kDisplayPatternRandom ):
        for( int y=0 ; y < this->height ; y++ )
        {
            for( int x=0 ; x < this->width ; x++ )
            {
                this->Set( x, y, qrand() & 0x0F );
            }
        }
        break;

    default:
        this->Fill( 2 ); // Red
    }

    this->RenderScreen();
}


#define QSET( X, Y, C ) \
    this->indexedBuffer[ ((Y) * this->width) + (X) ] = (C)

int PalettedFrameBuffer::DrawText( int x, int y, const unsigned char * pens, const unsigned char * txt )
{
    int c = 0;
    int yp;
    int mask = 0x80;
    int xp;
    int vSpacePerCharacter = fnt->h;

    if( this->doublevert ) vSpacePerCharacter = fnt->h * 2;
    vSpacePerCharacter += vSpacing;

    if( !txt ) return vSpacePerCharacter;

    /* for each character in the string */
    while( *txt != '\0' ) {

        /* if it's renderable */
        if( *txt >= fnt->startchar && *txt <= fnt->maxchar ) {
            unsigned char ch = (*txt) - fnt->startchar;
            unsigned char pen = *pens;

            /* for each scan line */
            for( int fy=0 ; fy<fnt->h ; fy++ ) {
                unsigned char pixels = fnt->data[ (ch * fnt->h) + fy ];

                yp = y + fy;
                if( this->doublevert ) yp += fy;

                /* for each column */
                xp = x;
                for( mask = 0x80 ; mask > 0 ; mask >>=1 )
                {
                    if( pixels & mask ) {
                        c = pen & 0x0F;
                    } else {
                        c = (pen & 0xF0) >> 4;
                    }

                    if( c >= 0 ) {
                        QSET( xp, yp, c );
                        if( this->doublehoriz ) QSET( xp+1, yp, c );

                        if( this->doublevert && !this->scanlines ) {
                            QSET( xp, yp+1, c );
                            if( this->doublehoriz ) QSET( xp+1, yp+1, c );
                        }
                    }
                    xp++;
                    if( this->doublehoriz ) xp++;
                }
            }
        }
        /* next character */
        txt++;
        pens++;

        /* next position */
        x = x + fnt->w + (fnt->w*this->doublehoriz ) + this->hSpacing;
    }

    return vSpacePerCharacter;
}


void PalettedFrameBuffer::RenderTextColorBuffer( const unsigned char * color,
                                                 const unsigned char * text,
                                                 int cursorX )
{
    if( !text || !color ) return;

    int height = 480;
    int hPad = 0;

    int starty = height - this->VSpacePerCharacter();

    /* clear the display */
    this->Fill( 0 );

    /* display the current text */
    for( int j=0 ; starty > 0 ; j++ )
    {
        starty -= this->DrawText( hPad, starty, &color[ j * 256 ], &text[ j * 256 ] );
    }

    /* draw the cursor */
    if( cursorX >= 0 && (this->promptType != kPrompt_None) )  {
        /* block cursor:     color 0xFF, character '_'
         * underline cursor: color 0x0F, character '_'
         */
        unsigned char cx = this->defaultPromptColor;

        switch( this->promptType ) {
        case( kPrompt_Underscore ):
            cx = this->defaultPromptColor | ( this->defaultBGColor << 4 );
            break;

        case( kPrompt_Block ):
            cx = this->defaultPromptColor | (this->defaultPromptColor << 4 );
            break;
        }



        if( this->doublehoriz )
            cursorX *= (16 + this->hSpacing);
        else
            cursorX *= (8 + this->hSpacing);

        this->DrawText(  hPad + cursorX, height - this->VSpacePerCharacter(), &cx, "_" );
    }

    /* and flush it to the screen */
    this->RenderScreen();
}
