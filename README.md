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

Features:

* 640x480 graphics mode as the base
* All text is rendered as graphics
* 80x25 default resolution, variable
* fonts/graphics can be rendered through "scanlines" for realism
* Theme presets store color palette, border and font settings.
* Theme presets for:
** Commodore PET
** Commodore Vic 20
** Commodore 64
** Atari 400/800
** Commodore Amiga

* All theme settings are also user-adjustable
** eg. use the C64 font with the Amiga colors, with no border

* Border: on/off
* Font height: 1x (8px), 2x (16px), 2x w/Scanlines (16px)
* Font width: 1x (8px), 2x (16px)  -- or 20/40/80 column
* Scroll type: Smooth, Jump
* Serial port switching  (select any available port)
* No mouse support. Keyboard only.

* Keyboard options for configuration menus

* VT100 emulation
* VT240 graphics Emulation (maybe)
* GiGi Graphics Emulation (maybe)
* Shell/telnet/ssh support (maybe)
