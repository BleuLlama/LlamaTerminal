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
#include <QFileInfo>
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

    , windowW( 0 ) /* usable window size */
    , windowH( 0 )

    /* border sizes */
    , borderN( 0 )
    , borderS( 0 )
    , borderE( 0 )
    , borderW( 0 )

    , hSpacing( 0 )
    , vSpacing( 1 )
    , doublevert( 1 )
    , doublehoriz( 1 )
    , scanlines( 0 )
{
    this->Setup( this->width, this->height );
    RecomputeRenderWindow();
    this->DrawBorder();
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
// Render Screen
//  This takes the paletted buffer and shoves it out to the RGB buffer to be displayed
void PalettedFrameBuffer::RenderScreen( void )
{
    const PALENT * colors = pals[ this->palId ].colors;

    this->RecomputeRenderWindow();

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

bool PalettedFrameBuffer::ImageExists( QString filepath )
{
    QFileInfo check_file( filepath );

    // check if file exists
    if (check_file.exists() ) {
        return true;
    } else {
        return false;
    }
}

void PalettedFrameBuffer::SavePng( QString filepath )
{
    char buf[128];
    int idx = 0;

    if( !filepath.compare( "" ) )
    {
        // filepath gets next item in list
        snprintf( buf, 128, "/tmp/llt_%04d.png", idx );
        filepath = buf;
        while( this->ImageExists( buf ) && idx < 9999 ) {
            idx++;
            snprintf( buf, 128, "/tmp/llt_%04d.png", idx );
            filepath = buf;
        }
    }

    this->imageGfx->save( filepath );

    QFileInfo qfi( filepath );

    qDebug() << "Image saved to " << qfi.absoluteFilePath();
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
    // xthis->RenderScreen();
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

    // xthis->RenderScreen();
}

////////////////////////////////////////////////////////////

#define QSET( X, Y, C ) \
    this->indexedBuffer[ ((Y) * this->width) + (X) ] = (C)

////////////////////////////////////////////////////////////


// need primitives:
//  Hline ( memset )
void PalettedFrameBuffer::DrawHLine( int sx, int sy, int w, int color )
{
    unsigned char * ptr = &this->indexedBuffer[ (sy * this->width) + sx ];
    memset( ptr, color&0x0f, w );
}

void PalettedFrameBuffer::DrawVLine( int sx, int sy, int h, int color )
{
    unsigned char * ptr = &this->indexedBuffer[ (sy * this->width) + sx ];

    while( h > 0 ) {
        *ptr = color;
        ptr += this->width;
        h--;
    }
}

void PalettedFrameBuffer::DrawHDiagLine( int x1, int y1, int x2, int y2, int xDelta, int yDelta, int color )
{
    int avdy = std::abs( yDelta );
    int d = (avdy<<1) - xDelta;
    int dy_x_2 = avdy<<1;
    int dy_m_dx_x_2= (avdy-xDelta)<<1;

    if ( yDelta > 0) {  // this creates more code but is faster
        while (x1 <= x2){
            this->Set( x1, y1, color );
            this->Set( x2, y2, color );
            if (d < 0){
                d += dy_x_2;
            } else {
                d += dy_m_dx_x_2;
                y1++; y2--;
            }
            x1++; x2--;
        }
    } else {
        while (x1 <= x2){
            this->Set( x1, y1, color );
            this->Set( x2, y2, color );
            if (d < 0){
                d += dy_x_2;
            } else {
                d += dy_m_dx_x_2;
                y1--; y2++;
            }
            x1++; x2--;
        }
    }
}

void PalettedFrameBuffer::DrawVDiagLine( int x1, int y1, int x2, int y2, int xDelta, int yDelta, int color )
{
    int avdx = std::abs( xDelta );
    int d = (avdx<<1) - yDelta;
    int dx_x_2 = avdx<<1;
    int dx_m_dy_x_2= (avdx-yDelta)<<1;

    if ( xDelta > 0 )
    {  // this creates more code but is faster
        while (y1 <= y2){
            this->Set( x1, y1, color );
            this->Set( x2, y2, color );
            if (d < 0){
                d += dx_x_2;
            } else {
                d += dx_m_dy_x_2;
                x1++; x2--;
            }
            y1++; y2--;
        }
    } else {
        while (y1 <= y2){
            this->Set( x1, y1, color );
            this->Set( x2, y2, color );
            if (d < 0){
                d += dx_x_2;
            } else {
                d += dx_m_dy_x_2;
                x1--; x2++;
            }
            y1++; y2--;
        }
    }
}


/* Note: this DrawLine() is a port from a paint program I worked on, UberPaint, (c)2000 */
void PalettedFrameBuffer::DrawLine( int x1, int y1, int x2, int y2, int color )
{
    int xDelta = x2 - x1;
    int yDelta = y2 - y1;

    /* optimize for h and vlines */
    if( xDelta == 0 ) {
        this->DrawVLine( std::min( x1, x2 ), std::min( y1, y2 ),
                         std::max( y1, y2 ) - std::min( y1, y2 ),
                         color );
        return;
    }

    if( yDelta == 0 ) {
        this->DrawHLine( std::min( x1, x2 ), std::min( y1, y2 ),
                         std::max( x1, x2 ) - std::min( x1, x2 ),
                         color );
        return;
    }


    /*
    **  1) find out which sets of angles to use
    **     (set 1 or set 2)
    **     compare the absolute values of the slopes
    **
    **  2) figure out which quadrant it's in
    **     (Quadrant A, B, C, or D)
    **     compare dx and dy with 0 for this result
    **
    **    \        |        /
    **      \  2B  |  2A  /
    **        \    |    /                       set 1:  |dx| > |dy|
    **          \  |  /   1A
    **      1B    \|/               dy < 0      set 2:  |dx| < |dy|
    **    ---------*---------       ------
    **      1C    /|\     1D        dy > 0
    **          /  |  \
    **        /    |    \
    **      /  2C  | 2D   \
    **    /        |        \
    **
    **     dx < 0  |  dx > 0
    */

    if ( std::abs( xDelta ) > std::abs( yDelta ))
    {
        // it's an angle (315,45) or (135,225)   slow angle (Set 1)
        if ( xDelta > 0 ){ // 1A or 1D
            this->DrawHDiagLine( x1, y1, x2, y2,  xDelta, yDelta, color );
            return;
        } else { // 1B or 1C
            //dx *= -1; dy *= -1;
            this->DrawHDiagLine( x2, y2, x1, y1, -xDelta, -yDelta, color );
            return;
        }
    }
    else
    {
        // it's an angle (45,135) or (225,315)   steep angle (set 2)
        if ( yDelta > 0 ){ // 2A or 2B (2B or not 2B)
            this->DrawVDiagLine( x1, y1, x2, y2,  xDelta, yDelta, color );
            return;
        } else { // 2C or 2D
            //dx *= -1; dy *= -1;
            this->DrawVDiagLine( x2, y2, x1, y1, -xDelta, -yDelta, color );
            return;
        }
    }

}


void PalettedFrameBuffer::DrawFilledBox( int sx, int sy, int w, int h, int color )
{
    int r;
    for( r=0 ; r<h ; r++ )
    {
        this->DrawHLine( sx, sy, w, color );
        sy++;
    }
}


void PalettedFrameBuffer::RecomputeRenderWindow( void )
{
    // render area
    this->windowW = this->width - (this->borderE + this->borderW);
    this->windowH = this->height - (this->borderN + this->borderS);
}

//  vline ( +width loop )
//  box ( memsets in a loop )


//////////////////////////////////////////////////////////////////////////////
// Borders

void PalettedFrameBuffer::DrawBorder()
{
    this->DrawBorder( 10, 0x34 );
}


void PalettedFrameBuffer::DrawBorder( int sz, int color )
{
    this->borderN = sz;
    this->borderS = sz;
    this->borderE = sz;
    this->borderW = sz;

    this->RecomputeRenderWindow();

    if( sz <= 0 ) return;

    // top
    unsigned char * ptr = &this->indexedBuffer[0];
    memset( ptr, color&0x0f, this->width * sz );

    // bottom
    ptr = &this->indexedBuffer[ this->width * (this->height - sz) ];
    memset( ptr, color&0x0f, this->width * sz );

    // left, right
    this->DrawFilledBox(                           0, this->borderN,  sz, this->height - (sz*2), color );
    this->DrawFilledBox( this->width - this->borderE, this->borderN,  sz, this->height - (sz*2), color );
}

void PalettedFrameBuffer::DrawAmiga1Border( void )
{

}

void PalettedFrameBuffer::DrawAmiga2Border( void )
{

}

////////////////////////////////////////////////////////////
// Font stuff

// these two will return the space in pixels per glyph needed to properly render
// according to settings (width and height separate)
int PalettedFrameBuffer::GetGlyphW( void )
{
    if( !fnt ) return 16;
    int hSpacePerCharacter = fnt->w;

    if( this->doublehoriz ) hSpacePerCharacter = fnt->w * 2;
    hSpacePerCharacter += this->hSpacing;

    return hSpacePerCharacter;
}

int PalettedFrameBuffer::GetGlyphH( void )
{
    if( !fnt ) return 16;

    int vSpacePerCharacter = fnt->h;

    if( this->doublevert ) vSpacePerCharacter = fnt->h * 2;
    vSpacePerCharacter += this->vSpacing;

    return vSpacePerCharacter;
}


void PalettedFrameBuffer::DrawCharacter( int x, int y, const unsigned char pen, const unsigned char ch )
{
    int c = 0;
    int yp;
    int mask = 0x80;
    int xp;
    int vSpacePerCharacter = fnt->h;

    if( this->doublevert ) vSpacePerCharacter = fnt->h * 2;
    vSpacePerCharacter += vSpacing;

    /* if it's renderable */
    if( ch >= fnt->startchar && ch <= fnt->maxchar ) {

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

    /* and flush it to the screen */
    // xthis->RenderScreen();
}


//////////////////////////////////////////////////////////////////////////////
void PalettedFrameBuffer::BlitWindowUp( int npix )
{
    int a;

    for( a = this->borderN ; a <= (this->windowH - this->borderS) ; a++ )
    {
        unsigned char * dst = indexedBuffer + (  a         * this->Width() );
        unsigned char * src = indexedBuffer + ( (a + npix) * this->Width() );

        memcpy( dst, src, this->windowW );
    }
}


void PalettedFrameBuffer::SaveSettings()
{
    SETUPSETTINGS();
    s.setValue( kSettings_PaletteId, this->palId );
    s.setValue( kSettings_PromptStyle, this->promptType );

    s.setValue( kSettings_FontHSpacing, this->hSpacing );
    s.setValue( kSettings_FontVSpacing, this->vSpacing );
    s.setValue( kSettings_FontDoubleHoriz, this->doublehoriz );
    s.setValue( kSettings_FontDoubleVert, this->doublevert );
    s.setValue( kSettings_FontScanLines, this->scanlines );
}

void PalettedFrameBuffer::LoadSettings()
{
    SETUPSETTINGS();

    this->palId = s.value( kSettings_PaletteId, "" ).toInt();
    this->promptType = s.value( kSettings_PromptStyle, kPrompt_Block ).toInt();

    this->hSpacing = s.value( kSettings_FontHSpacing, 0 ).toInt();
    this->vSpacing = s.value( kSettings_FontVSpacing, 1 ).toInt();
    this->doublehoriz = s.value( kSettings_FontDoubleHoriz, 1 ).toInt();
    this->doublevert = s.value( kSettings_FontDoubleVert, 1 ).toInt();
    this->scanlines = s.value( kSettings_FontScanLines, 1 ).toInt();
}
