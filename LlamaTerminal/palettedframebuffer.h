/* 
 * paletted frame buffer header
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
#ifndef PALETTEDFRAMEBUFFER_H
#define PALETTEDFRAMEBUFFER_H

#include <QObject>
#include <QImage>
#include <QLabel>
#include "globals.h"
#include "font.h"


typedef struct PALENT {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} PALENT;


class PalettedFrameBuffer : public QObject
{
    Q_OBJECT
public:
    explicit PalettedFrameBuffer(QObject *parent = 0);
    ~PalettedFrameBuffer();

private:
    int width;
    int height;
    unsigned char * indexedBuffer;
    unsigned char * rgbBuffer;
    QImage * imageGfx;
    int palId;
    LFONT * fnt;

public:
    void Setup( int w, int h );
    void UpSet( void );

    /* buffer/image accessors */
public:
    unsigned char * GetBuffer() { return this->indexedBuffer; }
    unsigned char * GetRGBBuffer() { return this->rgbBuffer; }
    QImage * getQImage() { return this->imageGfx; }

    int Width() { return this->width; }
    int Height() { return this->height; }

public:
    bool ImageExists( QString filepath );
    void SavePng( QString filepath = "" );

public:
    void SetFont( LFONT * fnt ) { this->fnt = fnt; }

    /* palette selection */
public:
    void SelectPalette( int p ) { this->palId = p; }
    void TogglePalette( void );
    QString GetPaletteString();

    /* pixels */
public:
    void Set( int x, int y, unsigned char color );
    unsigned char Get( int x, int y );

    /* fills */
public:
    void Fill( unsigned char color );

    void Clear( void ) {
        this->Fill( 0 );
    }


    void FillWithPattern( int patno );

#define kDisplayPatternBlank     (0)
#define kDisplayPatternStart     (1)
#define kDisplayPatternDiagonals (2)
#define kDisplayPatternStripes   (3)
#define kDisplayPatternRandom    (4)

/* NOTE:
 *  - do fg/bg color belong in here?
 *  - prompt probably doesn't
 */
private:
    unsigned char defaultFGColor;
    unsigned char defaultBGColor;
    unsigned char defaultPromptColor;
public:
    unsigned char GetDefaultFGColor() { return this->defaultFGColor; }
    void SetDefaultFGColor( unsigned char x ) { this->defaultFGColor = x; }
    unsigned char GetDefaultBGColor() { return this->defaultBGColor; }
    void SetDefaultBGColor( unsigned char x ) { this->defaultBGColor = x; }
    unsigned char GetDefaultPromptColor() { return this->defaultPromptColor; }
    void SetDefaultPromptColor( unsigned char x ) { this->defaultPromptColor = x; }

private:
    int promptType;
#define kPrompt_None       (0)
#define kPrompt_Underscore (1)
#define kPrompt_Block      (2)
public:
    void SetPromptType( int p ) { this->promptType = p; }
    int GetPromptType() { return this->promptType; }
    void TogglePromptType() { this->promptType++; if (this->promptType > 2) this->promptType = 1; }
    QString GetPromptString() { if( this->promptType == 1 ) return "Line"; return "Block"; }

    /* poly primitives */
public:
    void DrawHLine( int sx, int sy, int w, int color );
    void DrawVLine( int sx, int sy, int h, int color );
    void DrawHDiagLine( int xMin, int yMin, int xMax, int yMax, int xDelta, int yDelta, int color );
    void DrawVDiagLine( int xMin, int yMin, int xMax, int yMax, int xDelta, int yDelta, int color );

    void DrawFilledBox( int sx, int sy, int w, int h, int color );

    void DrawLine( int sx, int sy, int dx, int dy, int color );


public: /* decoration/border */
    int windowW;
    int windowH;

public:
    int borderN;
    int borderS;
    int borderE;
    int borderW;

public:
    void DrawBorder();
    void DrawBorder( int sz, int color );
    void DrawNoBorder( void ) { this->DrawBorder( 0, 0 ); }
    void DrawAmiga1Border( void );
    void DrawAmiga2Border( void );

    void RecomputeRenderWindow( void );


private:  /* FONT rendering stuff */
    int hSpacing; /* space between characters */
    int vSpacing; /* space between rows */
    int doublevert;
    int doublehoriz;
    int scanlines;

    /* font rendering attributes accessors */
public:
    void SetHSpacing(    int z  ) { this->hSpacing = z; }
    int  GetHSpacing(    void   ) { return this->hSpacing; }
    void ToggleHSpacing( void   ) { this->hSpacing++; if( this->hSpacing > 5 ) this->hSpacing = 0; }

    void SetVSpacing(    int z  ) { this->vSpacing = z; }
    int  GetVSpacing(    void   ) { return this->vSpacing; }
    void ToggleVSpacing( void   ) { this->vSpacing++; if( this->vSpacing > 5 ) this->vSpacing = 0; }

    void SetDoubleVert(  bool z ) { if( z ) this->doublevert = 1; else this->doublevert = 0; }
    bool GetDoubleVert(  void   ) { return (this->doublevert == 0) ? false : true; }
    bool ToggleDoubleVert( void ) { this->doublevert = this->doublevert ? 0:1; return this->GetDoubleVert(); }

    void SetDoubleHoriz( bool z ) { if( z ) this->doublehoriz= 1; else this->doublehoriz = 0; }
    bool GetDoubleHoriz( void   ) { return (this->doublehoriz == 0) ? false : true; }
    bool ToggleDoubleHoriz( void ) { this->doublehoriz = this->doublehoriz ? 0:1; return this->GetDoubleHoriz(); }

    void SetScanLines(   bool z ) { if( z ) this->scanlines= 1; else this->scanlines = 0; }
    bool GetScanLines(   void   ) { return (this->scanlines == 0) ? false : true; }
    bool ToggleScanLines( void ) { this->scanlines = this->scanlines ? 0:1; return this->GetScanLines(); }

    // these two will return the space in pixels per glyph needed to properly render
    // according to settings (width and height separate)
    int GetGlyphW( void );
    int GetGlyphH( void );

    // and actually draw the character to the screen
    void DrawCharacter( int x, int y, const unsigned char pen, const unsigned char ch );

    // screen copy
    void BlitWindowUp( int npix );
    // and rendering options
signals:
    void ScreenIsRendered();

public slots:
    void RenderScreen( void );

    void SaveSettings( void );
    void LoadSettings( void );
};

#endif // PALETTEDFRAMEBUFFER_H
