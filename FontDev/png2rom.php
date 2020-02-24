<?php
function LoadPNG($imgname)
{
    /* Attempt to open */
    $im = @imagecreatefrompng($imgname);

    /* See if it failed */
    if(!$im)
    {
        /* Create a blank image */
        $im  = imagecreatetruecolor(150, 30);
        $bgc = imagecolorallocate($im, 255, 255, 255);
        $tc  = imagecolorallocate($im, 0, 0, 0);

        imagefilledrectangle($im, 0, 0, 150, 30, $bgc);

        /* Output an error message */
        imagestring($im, 1, 5, 5, 'Error loading ' . $imgname, $tc);
    }

    return $im;
}

header('Content-Type: text/plain');
print_r( $argv );


$img = LoadPNG( $argv[1] ); //'bogus.image');

/*
    images are 32 glyphs aross,
    each character is 8x8 pixels, with a red border of 1 pixel, shared.
    */


function glyphAt( $img, $glyphX, $glyphY )
{
    $bytes = [];
    $startX = ($glyphX * 9)+1;
    $startY = ($glyphY * 9)+1;

    $value = 0;
    for( $y=0 ; $y<8 ; $y++ ) {
        $mask = 0x80;
        for( $x=0 ; $x<8 ; $x++ ) {

            $xpos = $startX + $x;
            $ypos = $startY + $y;
            $c = imagecolorat( $img, $xpos, $ypos );
            if( $c == 0 ) {
                $c = '.';

            } else {
                $c = '#';
                $value |= $mask;
            }

            $mask = $mask / 2;

        }
        $bytes[] = $value;
        $value = 0;
    }
    return $bytes; 
}


$fp = fopen($argv[2], 'w');

for( $y = 0 ; $y < 8 ; $y++ )
{
    for( $x=0 ; $x < 32 ; $x++ )
    {
        $bindata = glyphAt( $img, $x, $y );
        foreach( $bindata as $val ) {

            fwrite( $fp, pack( "C*", $val ) );
        }
    }
}

fclose($fp);

//imagepng($img);
imagedestroy($img);



?>