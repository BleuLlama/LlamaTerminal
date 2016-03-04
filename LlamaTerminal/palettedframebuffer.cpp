
#include <Qdebug>
#include "palettedframebuffer.h"
#include "font.h"


////////////////////////////////////////////////////////////


const PALENT colorPalette [] =
{
    // C=64 Palette
    {   0,   0,   0 },  // 0x00: black
    { 255, 255, 255 },  // 0x01: white
    { 255,   0,   0 },  // 0x02: red /* was 136 */
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
    { 183,  62, 130 }   // 0x1F: red violet
};

////////////////////////////////////////////////////////////

PalettedFrameBuffer::PalettedFrameBuffer( QObject *parent )
    : QObject(parent)
    , width( kDefaultWidth )
    , height( kDefaultHeight )
    , indexedBuffer( NULL )
    , rgbBuffer( NULL )
    , imageGfx( NULL )
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
    int poffs = 0;
    if( this->palId == kPaletteAmiga )  poffs = 16;

    if( !this->rgbBuffer ) return;

    // 1. convert the indexed buffer to the RGB buffer
    for( int i=0 ; i < (this->width * this->height) ; i++ )
    {
        int paletteIndex = (this->indexedBuffer[i] & 0x0F) + poffs;
        this->rgbBuffer[ (i*3)+0 ] = colorPalette[ paletteIndex ].r; // r
        this->rgbBuffer[ (i*3)+1 ] = colorPalette[ paletteIndex ].g; // g
        this->rgbBuffer[ (i*3)+2 ] = colorPalette[ paletteIndex ].b; // b
    }

    emit this->ScreenIsRendered();

#ifdef NEVER
    // 3. Blit the QImage into the label

#endif
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
    LFONT * fnt = &internalFont; /* for now, hardcoded */
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
    if( cursorX >= 0 ) {
        unsigned char cx[] = { 0x0F };
        this->DrawText(  hPad + (cursorX * 16), height - this->VSpacePerCharacter(), cx, "#" );
    }

    /* and flush it to the screen */
    this->RenderScreen();
}
