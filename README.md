# LlamaTerminal
A retro-looking, classic-computer themed terminal emulator.

This is intended to turn your computer into a retro-themable terminal.
It will not have fancy menus, nor nifty GL effects... It will emulate
a terminal (VT100 or ANSI) but look like your classic computer
system via color palette, font and graphics capabilities.

The core of it is built against its own framebuffer, which can be
ported to other display systems, but it is running on a QT engine
for the serial interface and graphics display for the initial
version.

This version is based on the QT interfaces, so you will need QT
Creator, as well as your local dev tools.  LlamaTerminal has been
built under the following configurations:

- Mac OS X 10.11 + Xcode 7.2 + QT Creator 3.5.1 + QT 5.5.1
- Windows 7 Professional + MSVC 2013 + QT Creator 3.5.0 + QT 5.5.0
- Linux - Ubuntu - 3.13 + gcc 4.9.1 + QT Creator 3.5.0 + QT 4.9.1

It should be easy to port to other QT platforms, and somewhat
easy-ish to port to a system without QT, replacing the Serial
interface class, and plugging the indexed buffer into whatever is
native for the system.  A LibSDL port should be reasinably possible.


Features:

* 640x480 graphics mode as the base
* All text is rendered as graphics
* fonts/graphics can be rendered through "scanlines" for realism
* Theme presets store color palette, border and font settings.
* (will contain) Theme presets for:
** Commodore PET
** Commodore Vic 20
** Commodore 64
** Atari 400/800
** Commodore Amiga (1.3 and 2.0)
** ADM 3A
** Digital VT-220

* All theme settings are also user-adjustable
** eg. use the C64 font with the Amiga colors, with no border

* Border: on/off
* Font height: 1x (8px), 2x (16px), 2x w/Scanlines (16px)
* Font width: 1x (8px), 2x (16px)  -- or 20/40/80 column
* Scroll type: Smooth, Jump
* Serial port switching  (select any available port)
* No mouse support. Keyboard only.

* Keyboard options for configuration menus

* VT100 or ANSI emulation (probably)
* VT240 graphics Emulation (maybe)
* GiGi Graphics Emulation (maybe)
* Shell/telnet/ssh support (maybe)
