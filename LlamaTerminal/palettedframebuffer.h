#ifndef PALETTEDFRAMEBUFFER_H
#define PALETTEDFRAMEBUFFER_H

#include <QObject>
#include <QImage>
#include <QLabel>

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

public:
    void Setup( int w, int h );
    void UpSet( void );

    /* buffer/image accessors */
public:
    unsigned char * GetBuffer() { return this->indexedBuffer; }
    unsigned char * GetRGBBuffer() { return this->rgbBuffer; }
    QImage * getQImage() { return this->imageGfx; }

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

    void SetVSpacing(    int z  ) { this->vSpacing = z; }
    int  GetVSpacing(    void   ) { return this->vSpacing; }

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
