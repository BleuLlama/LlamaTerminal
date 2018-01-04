/*
 * framed sprite
 *
 *  2017.12.31
 *  yorgle@gmail.com
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

#ifndef __FRAMEDSPRITE_H__
#define __FRAMEDSPRITE_H__

#include <QObject>
#include <QImage>
#include <QPoint>
#include <QSize>
#include <QColor>

#include "globals.h"

////////////////////////////////////////////////////////
class FramedSprite : public QObject
{
    Q_OBJECT
public:
    explicit FramedSprite(QObject *parent = 0, QString _imagePath = "" );
    ~FramedSprite( void );

    //////////////////////////////////////////////////
public:
    // utility
    bool FileExists( QString path );
    void Dump( void );

    //////////////////////////////////////////////////
    // core image functions
private:
    QString imagePath;
    QImage * theImage;
    void LoadImage( void );

public:
    bool WithinImage( int x, int y );

    //////////////////////////////////////////////////
    // color related functions

public:
    QRgb ColorAt( int x, int y );

public:
    QRgb PaletteColor( int index ) {
        return this->ColorAt( index, 0 );
    }

    //////////////////////////////////////////////////
    // accessors
private:
    bool okay;
public:
    // accessors, etc.
    bool Okay( void ) {
        return this->okay;
    }

    QImage *  Image( void ) {
        return this->theImage;
    }

    int Width( void ){
        if( !this->Okay() ) return 0;
        return this->theImage->width();
    }

    int Height( void ){
        if( !this->Okay() ) return 0;
        return this->theImage->height();
    }

    //////////////////////////////////////////////////
    // border color stuff
public:
    QRgb BorderColor() {
        return this->ColorAt( 0, 1 );
    }
    bool BorderAt( int x, int y  ) {
        return ( this->BorderColor() == this->ColorAt( x, y ) );
    }

    //////////////////////////////////////////////////
private:
    QPoint currentPos;
    QSize  currentSize;

public:
    // measure the sprite under the current pointer
    void MeasureCurrent( void );

    // reset pointer to top left sprite
    void Rewind( void );

    // seek to and measure the next sprite to the right
    void SeekNext( void );

    // seek to and measure the first sprite on the next row
    void SeekNextRow( void );


public:
    // are the current sprite pointers okay?
    bool SpriteOkay( void );

    // get the current position
    QPoint & SpritePos( void ) {
        return this->currentPos;
    }

    // get the current size
    QSize & SpriteSize( void ) {
        return this->currentSize;
    }
};

#endif
