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

#define kLlamaTermVersion "v0.02"
/*
 * v0.02 - 2016/3/6 Initial functionality
 *
 * v0.01 - 2016/3/x Building it up, importing bits from GfxTest, refactoring
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

#define kSettings_AppVersion        "Main_AppVersion"

#define kSettings_TermLocalEcho     "Terminal_LocalEcho"
#define kSettings_TermWrap          "Terminal_Wrap"
#define kSettings_TermPrompt        "Terminal_Prompt"

#define kSettings_TextPen           "Text_Pen"

#define kSettings_SerialName        "Serial_Name"
#define kSettings_SerialLocation    "Serial_Location"
#define kSettings_SerialDescription "Serial_Description"
#define kSettings_SerialBaud        "Serial_Baud"
#define kSettings_SerialBits        "Serial_Bits"
#define kSettings_SerialParity      "Serial_Parity"
#define kSettings_SerialFlow        "Serial_Flow"
#define kSettings_SerialStop        "Serial_Stop"

#define kSettings_VisualPalId       "Visual_PalId"
#define kSettings_VisualHSpacing    "Visual_HSpacing"
#define kSettings_VisualVSpacing    "Visual_VSpacing"
#define kSettings_VisualDoubleHoriz "Visual_DoubleHoriz"
#define kSettings_VisualDoubleVert  "Visual_DoubleVert"
#define kSettings_VisualScanLines   "Visual_ScanLines"

#define kSettings_FontPath          "Font_Path"
#define kSettings_FontName          "Font_Name"
#define kSettings_FontDirectory     "Font_Directory"


#endif // GLOBALS_H

