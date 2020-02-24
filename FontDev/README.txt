
Template files for piko, for making your own font
	template.piko
	template_adm.piko

FONT/
	a directory of 2k binary LLF files, generated from the Font images
	provided in the ../Fonts/ directory.

	These files are in a format appropriate for the TMS9918A video
	chip.  The data for each glyph is stored as such:

	255 glyphs
		Each glyph is 8 bytes, 
			each byte is a row of 8 pixels for the glyph
			bytes are in-order from top to bottom
			each byte is stored as you would see it, left to right

	For example, the data for "K" might be the sequence of 
	8 bytes:
	    0x82, 0x84, 0x88, 0xF0,
	    0x88, 0x84, 0x82, 0x00

	As seen here:

    8421 8421
	#... ..#.
    #... .#..
    #... #...
    #### ....
    #... #...
    #... .#..
	#... ..#.
	.... ....

    

png2rom.php
run.sh
	The PHP script that uses GD to load the image, and save it out in the raw 2kbyte LLF format
