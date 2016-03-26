----------------------------------------
                                Overview

The fonts in this directory were collected from various sources.
This document will list all of the sources used and what changes
were made where applicable.

All of the fonts here are 32 characters across, 8 rows, with one
pixel overlapping, surrounding each glyph.  The grid is red, the
pixels to be rendered are white, on a black background (whcih can
be removed at render time.

All fonts are in the MS-DOS extended character style, with ASCII
characters at the low range, and accented characters, ANSI graphics,
and then math graphics, in an attempt to standardize the font format.

- Scott Lawrence
  yorgle@gmail.com
  March 2016

----------------------------------------
                                   Fonts

dos.blocksansf.png
dos.flat.png
dos.inverted.png
dos.italics.png
dos.sans.png
dos.serif.png
dos.super.png
dos.thin.png
dos.vga.png

	Snagged from GIFs here: 
		http://umlautllama.com/projects/turacocl/starters/#s8
	Which I originally got from the VGA Font Editor (VGA FED)
	by Simon Griffiths. (download link on the above page)

	These were run through an ImageMagick filter to add in the
	red grid spacings:

	convert font.gif -background Red -crop 8x0 +repage \
		-splice 1x0 +append -crop 0x8 +repage \
		-splice 0x1 -append -gravity south \
		-splice 0x1 -gravity east -splice 1x0 outfont.png

amiga.topaz.1.serif.png
amiga.topaz.2.sans.png
	Both of these were made by myself using PikoPixel on OS X.
	They started from "dos.vga.png" above, and the glyphs have been
	converte to be Topaz 1.x and Topaz 2.x style, fitting in with
	the DOS layout.

	Looks best doubled-vertical, single horizontal.

vt220.png
	Created by myself in PikoPixel.  Pixel-based on GlassTTY
	    	http://www.spacerogue.net/wordpress/?p=356

	Looks best doubled-vertical, single horizontal, scanlines.

TI994A.png
	Created by myself.  The main glyphs were transposed from
	"TI99 Basic" TTF available here:
		http://www.dreamcodex.com/fonts.php

	Looks best square.


Commodore.128.png
Commodore.64.png
Commodore.Pet.png
	Created by myself in Pikopixel, based on font glyphs from here:
	        http://www.kreativekorp.com/software/fonts/c64.shtml

	The "Pet" version of the font should be used for PET, CBM
	and VIC/VC-20 targets.  There are only minor differences
	to lowercase letters between the 64 and 128 versions.

	Look best double vertical and horizontal, depending on target.


Apple.II.png
	Created by myself in Pikopixel, based on font glyphs from here:
		http://www.kreativekorp.com/software/fonts/apple2.shtml

	Look best square.

MSX.png
	Converted from the imagery here:
		http://uzebox.org/wiki/index.php?title=Font_Bitmaps

ZX.Spectrum.png
	Converted from the imagery here:
		http://uzebox.org/wiki/index.php?title=Font_Bitmaps

Atari.800.png
	Partial font, made by me, based on a few Atari 800 screenshots.
	Most of it is the Commodore 64 font, but the digits, upper
	and lowercase glyphs are correct.

reference:

https://damieng.com/blog/2011/02/20/typography-in-8-bits-system-fonts
