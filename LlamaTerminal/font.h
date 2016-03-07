#ifndef FONT_H
#define FONT_H

#include <QObject>

typedef struct lfont {
    int startchar;
    int maxchar;
    int w;
    int h;
    unsigned char * data;
} LFONT;

////////////////////////////////////////////////////////
class Font : public QObject
{
    Q_OBJECT
public:
    explicit Font(QObject *parent = 0);
    ~Font( void );

public:
    void Setup( void );
    void UpSet( void );

private:
    QString fontDirectory;
    LFONT theFont;

public:
    void SetFontDirectory( QString d ) { this->fontDirectory = d; }
    QString GetFontDirectory( ) { return this->fontDirectory; }

private:
    void LoadCurrentSelection();

public:
    LFONT * GetLFont() { return &this->theFont; }

private:
    int GetNumFontsAvailable();
    QString PathForFontAt( int which );
    QString NameFromPath( QString pth );
    int IndexOfPath( QString pth );

    QString currentFontPath;
    QString currentFontName;

public:
    void ToggleFont();
    QString GetFontName() {return this->currentFontName; }

signals:
    void NewFontLoaded( void );

public slots:
};

#endif // FONT_H
