#ifndef PALETTEDFRAMEBUFFER_H
#define PALETTEDFRAMEBUFFER_H

#include <QObject>
#include <QImage>
#include <QLabel>
#include "font.h"

#define kDefaultWidth (640)
#define kDefaultHeight (480)

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

    /* FONT rendering stuff */
private:
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

public:
    int DrawText( int x, int y, const unsigned char * pens, const unsigned char * txt );
    int DrawText( int x, int y, const unsigned char * pens, const char * txt ) {
        return this->DrawText( x, y, pens, (const unsigned char *) txt );
    }

    int VSpacePerCharacter( )
    {
        return( this->DrawText( 0, 0, NULL, (const unsigned char *)NULL ));
    }

    void RenderTextColorBuffer( const unsigned char * color, const unsigned char * text, int cursorX = -1 );

signals:
    void ScreenIsRendered();

public slots:
    void RenderScreen( void );
};

#endif // PALETTEDFRAMEBUFFER_H
