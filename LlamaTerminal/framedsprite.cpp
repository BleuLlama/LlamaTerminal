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

#include <QDir>
#include <QDebug>
#include <QImage>
#include <QFileInfo>
#include "framedsprite.h"



FramedSprite::FramedSprite(QObject *parent, QString _imagePath)
    : QObject(parent)
    , imagePath( _imagePath )
    , theImage( NULL )
    , okay( false )
    , currentPos( QPoint( 0, 0 ) )
    , currentSize( QSize( 0, 0 ) )
{
    this->LoadImage();
    this->Rewind();
}

FramedSprite::~FramedSprite()
{
    delete this->theImage;
    this->theImage = NULL;
    this->okay = false;
}

//////////////////////////////////////////////////////////////////////////////

QString toHex( long val )
{
    return QString("0x%1").arg(val , 0, 16);
}

QString toColor( long val )
{
    return QString( "R:0x%1  G:0x%2  B:0x%3  A:0x%4" )
            .arg( (val >> 24 ) & 0x0FF, 2, 16, QChar('0'))
            .arg( (val >> 16 ) & 0x0FF, 2, 16, QChar('0'))
            .arg( (val >> 8  ) & 0x0FF, 2, 16, QChar('0'))
            .arg( (val >> 0  ) & 0x0FF, 2, 16, QChar('0'))
            ;
}

void FramedSprite::Dump( void )
{
    qDebug() << "Framed Sprite dump: ";
    qDebug() << "  ImagePath: " << this->imagePath;
    qDebug() << "       Okay: " << this->Okay();
    if( this->Okay() ) {
        qDebug() << "        res: " << this->Width() << "x" << this->Height();
        qDebug() << "     border: " << toColor(this->BorderColor() );
        for( int i=0 ; i < 8 ; i++ ) {
            qDebug() << "      color: " << i << ":" << toColor( this->PaletteColor( i ));
        }
        int i=0;

        this->Rewind();
        while( this->SpriteOkay() ) {

            while( this->SpriteOkay() ) {
                qDebug() << "  sprite" << i++ << "@" << this->SpritePos().x() << "," << this->SpritePos().y() << "  " << this->SpriteSize().width() << "x" << this->SpriteSize().height();
                this->SeekNext();
            }
            this->SeekNextRow();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
bool FramedSprite::FileExists(QString path)
{
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////

bool FramedSprite::WithinImage( int x, int y )
{
    if( !this->okay ) return false;
    if( x < 0 || y < 0 ) return false;
    if( x >= this->Width() ) return false;
    if( y >= this->Height() ) return false;
    return true;
}

QRgb FramedSprite::ColorAt( int x, int y )
{
    // if the image failed, doesn't exist, or coords are outside, return border
    if( !this->WithinImage( x, y )) return  0; //this->borderColor;

    return this->theImage->pixel( x, y );
}

//////////////////////////////////////////////////////////////////////////////

bool FramedSprite::SpriteOkay( void )
{
    if( this->currentPos.x() < 1 || this->currentPos.y() < 1 ) return false;
    if( this->currentSize.width() < 1 || this->currentSize.height() < 1 ) return false;
    return true;
}

void FramedSprite::MeasureCurrent( void )
{
    int x = this->currentPos.x();
    int y = this->currentPos.y();

    this->currentSize.setHeight( 0 );
    this->currentSize.setWidth( 0 );

    // determine width
    while( this->WithinImage( x, y) && !BorderAt( x, y ) ) x++;
    if( this->WithinImage( x,y )) {
        this->currentSize.setWidth( x - this->currentPos.x() );
    }

    // determine height
    x = this->currentPos.x();
    y = this->currentPos.y();

    while( this->WithinImage( x, y ) && !BorderAt( x, y ) ) y++;
    if( this->WithinImage( x, y )) {
        this->currentSize.setHeight( y - this->currentPos.y() );
    }

    if( this->currentSize.width() == 0  || this->currentSize.height() == 0 ){
        this->currentSize.setHeight( 0 );
        this->currentSize.setWidth( 0 );
    }
}

void FramedSprite::Rewind( void )
{
    this->currentPos.setX( 1 );
    this->currentPos.setY( 1 );
    this->MeasureCurrent();
}

/*
 * 1,1  21x22  |  23,1  1x22  |  25,1  48x22
 * 1,24  5x1   |  7,24  1x1   |  9x24   3x1
 * 2,26  5x5   |  7,26  1x5
 */

void FramedSprite::SeekNext( void )
{
    int x = this->currentPos.x() + this->currentSize.width() + 1;
    int y = this->currentPos.y();

    while( this->WithinImage( x, y ) && BorderAt( x, y ) ) x++;
    this->currentPos.setX( x );
    this->MeasureCurrent();
}

void FramedSprite::SeekNextRow( void )
{
    this->currentPos.setX( 1 );
    this->MeasureCurrent();

    int x = 1;
    int y = this->currentPos.y()  + this->currentSize.height() + 1;

    while( this->WithinImage( x, y ) && BorderAt( x, y ) ) y++;
    this->currentPos.setY( y );
    this->MeasureCurrent();
}


void FramedSprite::LoadImage()
{
    if( !this->FileExists( this->imagePath ) ){
        qDebug() << this->imagePath << ": File not found.";
        return;
    }

    this->theImage = new QImage( this->imagePath );
    if( !this->theImage ) {
        qDebug() << this->imagePath << ": Can't load image.";
    }
    this->okay = true;
}
