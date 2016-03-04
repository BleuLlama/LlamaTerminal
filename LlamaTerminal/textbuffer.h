#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include <QObject>

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

};

#endif // TEXTBUFFER_H