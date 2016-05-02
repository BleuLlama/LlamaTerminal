/* 
 * globals
 *
 *  various global defines and version information
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
#include <QDebug>
#include <QSettings>

#ifndef GLOBALS_H
#define GLOBALS_H

/* Version History */

#define kLlamaTermVersion "v0.07"
/*
 * v0.07 - 2016/04/19 New character based rendering in progress
 * v0.06 - 2016/03/21 Restructuring setup/settings, theme loads font (by name)
 * v0.05 - 2016/03/20 preliminary theme support
 * v0.04 - 2016/03/09 Fixes to cleanly build on Windows (snprintf)
 * v0.03 - 2016/03/08 Settings, Framebuffer, Load/Save, Font support
 * v0.02 - 2016/03/06 Initial functionality
 * v0.01 - 2016/03/xx Building it up, importing bits from GfxTest, refactoring
 */


/* Framebuffer size */
#define kDefaultWidth  (640)
#define kDefaultHeight (480)

QString GetSettingsFile();
#define SETUPSETTINGS() \
    QSettings s( "UmlautLlama.com", "LlamaTerminal" );
#define SETUPSETTINGS_WIN() \
    QSettings s( GetSettingsFile() );

//////////////////////////////////
// Settings keys

#define kSettings_AppVersion        "AppVersion"

#define kSettings_PromptStyle       "PromptStyle"
#define kSettings_DecorationStyle   "DecorationStyle"
#define kSettings_DecorationSize    "DecorationSize"
#define kSettings_DecorationColor   "DecorationColor"
#define kSettings_BackgroundColor   "BackgroundColor"
#define kSettings_LocalEcho         "LocalEcho"
#define kSettings_TextWrap          "TextWrap"
#define kSettings_TextPen           "TextPen"

#define kSettings_PortName          "PortName"
#define kSettings_PortLocation      "PortLocation"
#define kSettings_PortDescription   "PortDescription"

#define kSettings_CommBaud          "CommBaud"
#define kSettings_CommBits          "CommBits"
#define kSettings_CommParity        "CommParity"
#define kSettings_CommFlow          "CommFlow"
#define kSettings_CommStop          "CommStop"

#define kSettings_PaletteId         "PaletteId"          /* themeable */
#define kSettings_FontHSpacing      "FontHSpacing"       /* themeable */
#define kSettings_FontVSpacing      "FontVSpacing"       /* themeable */
#define kSettings_FontDoubleHoriz   "FontDoubleHoriz"    /* themeable */
#define kSettings_FontDoubleVert    "FontDoubleVert"     /* themeable */
#define kSettings_FontScanLines     "FontScanLines"      /* themeable */

#define kSettings_FontPath          "FontPath"           /* themeable */
#define kSettings_FontName          "FontName"           /* themeable */
#define kSettings_FontDirectory     "FontDirectory"

#define kSettings_ThemePath         "ThemePath"
#define kSettings_ThemeName         "ThemeName"
#define kSettings_ThemeDirectory    "ThemeDirectory"


#ifdef Q_OS_WIN32

#ifndef snprintf
int snprintf(char * str, size_t size, const char * format, ...);
#endif

#endif // Q_OS_WIN32

#endif // GLOBALS_H

