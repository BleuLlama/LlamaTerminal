/*
 * theme
 *
 *  mechanism for loading/saving settings groups
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
#include <QSettings>

#include "theme.h"
#include "globals.h"

#define kInternalThemePath "(internal)"


Theme::Theme(QObject *parent)
    : QObject(parent)
    , themeDirectory( "/" )
    , currentThemeName( "" )
    , currentThemePath( kInternalThemePath )
{
}

Theme::~Theme( void )
{
}


//////////////////////////////////////////////////////////////////////////////

void Theme::Setup( void )
{
    this->LoadSettings();
}

void Theme::UpSet( void )
{
}


//////////////////////////////////////////////////////////////////////////////
int Theme::GetNumThemesAvailable()
{
    QDir themedir( this->themeDirectory );
    themedir.setNameFilters( QStringList() << "*.ini");
    QStringList fileList = themedir.entryList();

    return fileList.count();
}

QString Theme::PathForThemeAt( int which )
{
    if( which < 0 ) return kInternalThemePath;

    QDir themedir( this->themeDirectory );
    themedir.setNameFilters( QStringList() << "*.ini");
    QStringList fileList = themedir.entryList();

    if( which > fileList.count() )
        return kInternalThemePath;

    return( this->themeDirectory + "/" + fileList.at( which ));
}

QString Theme::NameFromPath( QString pth )
{
    if( !pth.compare( kInternalThemePath )) return "user theme";

    QString tmp = pth.left( pth.lastIndexOf(".") );

    return tmp.right( tmp.length() - tmp.lastIndexOf( ("/")) -1 );
}

int Theme::IndexOfPath( QString pth )
{
    for( int i=0 ; i<this->GetNumThemesAvailable() ; i++ )
    {
        if( !pth.compare( this->PathForThemeAt(i)))
            return i;
    }

    // didn't find it, go to the internal one
    return -1;
}


//////////////////////////////////////////////////////////////////////////////
void Theme::ToggleTheme()
{
    int i = this->IndexOfPath( this->currentThemePath );

    i++;
    if( i >= this->GetNumThemesAvailable() ) {
        i = -1;
    }

    this->currentThemePath = this->PathForThemeAt( i );
    this->currentThemeName = this->NameFromPath( this->currentThemePath );

    this->LoadCurrentSelection();
}

//////////////////////////////////////////////////////////////////////////////

#include <QFileDialog>

void Theme::RequestThemeDirectory()
{
    QString str = QFileDialog::getExistingDirectory( NULL,
                                                     "Choose your theme directory",
                                                     this->themeDirectory,
                                                     QFileDialog::DontResolveSymlinks );
    this->SetThemeDirectory( str );
    this->SaveSettings();
}

//////////////////////////////////////////////////////////////////////////////////////////
void Theme::LoadCurrentSelection()
{
    SETUPSETTINGS();

    QSettings th( this->currentThemePath, QSettings::IniFormat );
    th.beginGroup( "Theme" );

    const QStringList childKeys = th.childKeys();
    foreach( const QString &childKey, childKeys ) {
//        qDebug() << "  " << childKey << " --> " << th.value( childKey );

        if( !childKey.compare( kSettings_FontName ) ) s.setValue( kSettings_FontName, th.value( childKey ).toString() );
        else if( !childKey.compare( kSettings_PaletteId ) ) s.setValue( kSettings_PaletteId, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_FontHSpacing ) ) s.setValue( kSettings_FontHSpacing, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_FontVSpacing ) ) s.setValue( kSettings_FontVSpacing, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_FontDoubleHoriz ) ) s.setValue( kSettings_FontDoubleHoriz, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_FontDoubleVert ) ) s.setValue( kSettings_FontDoubleVert, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_FontScanLines ) ) s.setValue( kSettings_FontScanLines, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_DecorationStyle ) ) s.setValue( kSettings_DecorationStyle, th.value( childKey ).toString() );
        else if( !childKey.compare( kSettings_DecorationSize ) ) s.setValue( kSettings_DecorationSize, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_DecorationColor ) ) s.setValue( kSettings_DecorationColor, th.value( childKey ).toInt() );
        else if( !childKey.compare( kSettings_BackgroundColor ) ) s.setValue( kSettings_BackgroundColor, th.value( childKey ).toInt() );
    }
    th.endGroup();

    emit this->NewThemeLoaded();
}

void Theme::SaveSettings()
{
    SETUPSETTINGS();

    s.setValue( kSettings_ThemePath, this->currentThemePath );
    s.setValue( kSettings_ThemeName, this->currentThemeName );
    s.setValue( kSettings_ThemeDirectory, this->themeDirectory );
}

void Theme::LoadSettings()
{
    SETUPSETTINGS();

    this->currentThemePath = s.value( kSettings_ThemePath, "" ).toString();
    this->currentThemeName = s.value( kSettings_ThemeName, "" ).toString();
    this->themeDirectory = s.value( kSettings_ThemeDirectory, "" ).toString();

    this->SetThemeDirectory( this->themeDirectory );
    this->LoadCurrentSelection();
}
