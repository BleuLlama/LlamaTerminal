/* 
 * font header
 *
 *  Internal font, file font, settings support
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
#ifndef FONT_H
#define FONT_H

#include <QObject>
#include "globals.h"

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
    void RequestFontDirectory();

private:
    void LoadCurrentSelection();
    void SaveStruct();

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
    void SaveSettings( void );
    void LoadSettings( void );
};

#endif // FONT_H
