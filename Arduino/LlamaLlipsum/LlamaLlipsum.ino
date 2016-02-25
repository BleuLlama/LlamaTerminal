/*
 * LlamaLlipsum
 *
 *  A simple thing to appear as a serial device, and allow 
 *  for various amounts of lorem ipsum to be generated.
 *  Originally made as a testing device for LlamaTerm.
 *  
 *  v1.0 2016-02-24  yorgle@gmail.com
 */

////////////////////////////////////////////////////////////////////////////////
// Autobaud

long bauds[] = { /*115200,*/ 9600, 19200, 4800, 57600, // common
                        2400, 1200, 38400, 28800, 14400, 1200, 0 }; // less common
// doesn't seem to work for 300, 2400, 115200 baud
// well, it detects 115200, but it doesn't work

long baud = 0;

void serial_connected( void )
{
  Serial.flush();
  Serial.print( "CONNECT " );
  Serial.println( baud, DEC );
}

void autobaud_begin( void )
{
  baud = 0;
  do {
    Serial.begin( bauds[baud] );
    while( !Serial ); // leonardo fix
    
    // wait for a character
    while( !Serial.available() ) { 
      #ifdef NEVER
      // check for break-out
      if( digitalRead( kButton ) == LOW ) {
        baud = kDefaultBaud;
        Serial.end();
        Serial.begin( baud );
        while( !Serial ); // leonardo!
        serial_connected();
        return;
      }
      #endif
    }
    
    int ch = Serial.read();
    if( ch == 0x0d || ch == 0x0a) {
      baud = bauds[baud];
      serial_connected( );
      return;
    }
    Serial.end();
    
    baud++;
  } while( bauds[baud] != 0 );
  baud = 0;
}


//////////////////////////////////////////
// other configuration

int cols = 40;
int rows = 5;

/////////////////////////////////////////
// row, col, baud settings

void newCols( int w )
{
  if( w <= 0 ) {
    Serial.println( F( "Bad number." ));
    return;
  }
  
  cols = w;
  Serial.print( w, DEC );
  Serial.println( " cols." );
  
  for( int w=0 ; w<cols ; w++ ) {
    Serial.print( "-" );
  }
  Serial.println( "" );
}

void newRows( int h )
{
  if( h <= 0 ) {
    Serial.println( F( "Bad number." ));
    return;
  }

  rows = h;
  Serial.print( h, DEC );
  Serial.println( " rows." );
}

void newBaud( long b )
{
  if( b < 0 ) {
    Serial.println( F( "Bad number." ));
    return;
  }


  baud = b;
  if( b <= 10 ) {
    Serial.print( bauds[baud] );
  } else {
    Serial.print( b, DEC );
  }
  Serial.println( " baud." );

  Serial.end();
  delay( 100 );
  Serial.begin( baud );
  while( !Serial );   // leonardo fix
}


/////////////////////////////////////////
// arduino setup
void setup()
{
  // set up the serial
  //Serial.begin( 9600 );
  autobaud_begin();

  Serial.println( "" );
  Serial.println( "LlamaLlipsum" );
  Serial.println( "yorgle@gmail.com" );
  Serial.println( "Ready." );
  Serial.println( "" );

  newCols( 80 );
  clearLine();
}

/////////////////////////////////////////
// lipsum printer

// the proper way to do this is to put all of the text strings into 
// PROGMEM, but this fits for now. so we'll just go with it.

const char * const lipsum[] = {
  "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur",
  "adipiscing", "elit.", "Pellentesque", "et", "vulputate",
  "velit.", "Pellentesque", "tempor", "ligula", "id", "dolor",
  "gravida", "convallis.", "Duis", "sed", "dignissim", "lectus.",
  "Nulla", "tempus", "nunc", "a", "maximus", "sodales.", "Phasellus",
  "rhoncus", "finibus", "suscipit.", "Donec", "euismod", "volutpat",
  "erat,", "aliquet", "auctor", "ipsum", "vehicula", "vitae.",
  "Sed", "nibh", "turpis,", "volutpat", "in", "volutpat", "nec,",
  "viverra", "non", "purus.", "Proin", "malesuada", "lobortis",
  "elit,", "in", "porta", "nulla", "laoreet", "et.", "Phasellus",
  "ligula", "turpis,", "pellentesque", "sed", "magna", "congue,",
  "efficitur", "ultrices", "tortor.", "Morbi", "leo", "dui,",
  "vestibulum", "ac", "eleifend", "tempus,", "consequat", "eget",
  "felis.", "Cras", "ante", "elit,", "vestibulum", "at", "tortor",
  "non,", "ullamcorper", "rutrum", "odio.", "", "Donec", "ac",
  "urna", "eget", "lacus", "mollis", "gravida", "sed", "sed",
  "neque.", "In", "hac", "habitasse", "platea", "dictumst.",
  "Donec", "id", "posuere", "nibh.", "Donec", "ac", "diam", "nec",
  "augue", "blandit", "rhoncus", "sit", "amet", "sed", "risus.",
  "Praesent", "quis", "ullamcorper", "nibh,", "eu", "rhoncus",
  "tortor.", "Praesent", "pulvinar", "tristique", "ornare.",
  "Curabitur", "non", "lectus", "nec", "turpis", "tincidunt",
  "aliquet", "vitae", "at", "ex.", "Proin", "leo", "nisi,",
  "convallis", "vel", "auctor", "sed,", "imperdiet", "ac",
  "tortor.", "Sed", "dignissim", "nulla", "vel", "tortor",
  "interdum,", "non", "tempus", "orci", "imperdiet.", "Donec",
  "convallis", "nibh", "quis", "ligula", "feugiat,", "eget",
  "dictum", "urna", "vulputate.", "", "Nam", "vehicula,", "leo",
  "a", "dignissim", "tempus,", "purus", "orci", "pellentesque",
  "lectus,", "nec", "porta", "magna", "neque", "at", "augue.",
  "Pellentesque", "lacinia", "blandit", "risus,", "ac", "vestibulum",
  "odio", "scelerisque", "nec.", "Phasellus", "sed", "ultricies",
  "dolor.", "Nunc", "vehicula", "mi", "mi,", "in", "pellentesque",
  "nisi", "dignissim", "eget.", "Proin", "sodales,", "ex", "a",
  "blandit", "finibus,", "arcu", "velit", "mattis", "purus.",
#ifdef NEVER
  "vitae,", "suscipit", "ex", "lacus", "ut", "elit.", "Quisque",
  "feugiat", "non", "tellus", "quis", "cursus.", "Nulla", "quis",
  "dolor", "vel", "mi", "faucibus", "egestas.", "", "Vivamus",
  "cursus", "quis", "nulla", "quis", "euismod.", "Fusce", "gravida",
  "bibendum", "dolor", "in", "vehicula.", "Proin", "mauris",
  "felis,", "ultricies", "quis", "rhoncus", "ac,", "fermentum",
  "at", "lacus.", "Sed", "porta", "leo", "magna,", "a", "finibus",
  "odio", "aliquam", "et.", "Ut", "pellentesque", "varius", "est",
  "nec", "lobortis.", "Nam", "id", "massa", "molestie,", "mollis",
  "nisl", "id,", "mattis", "elit.", "Vivamus", "elementum", "ut",
  "massa", "interdum", "feugiat.", "Suspendisse", "elementum",
  "erat", "ac", "odio", "tempus,", "ac", "congue", "nunc",
  "malesuada.", "Vivamus", "ac", "ligula", "condimentum", "amet.",
#endif
  NULL
};


void printText( )
{ 
  int idx = 0;
  int ncols = 0;
  int nrows = 0;
  
  int w = 0;

  // only continue for the number of rows we want
  do {
    // see if it will fit on the line
    w = strlen( lipsum[idx] );
    if( ((ncols + w ) >= cols) || (w==0) ) {
      Serial.println( "" );
      ncols = 0;
      nrows++;
    }

    // see if we're past rows
    if( nrows < rows ) {
      // put it on this row
      if( ncols != 0 ) {
        Serial.print( " " );
        ncols++;
      }
      Serial.print( lipsum[idx] );
      ncols += w;
    }

    idx++;
    if( lipsum[idx] == NULL ) {
      idx = 0;
    }
    
  } while( nrows < rows );
}


/////////////////////////////////////////
// serial port line reading

char line[80];

void clearLine()
{
  line[0] = '\0';
}

void readLine()
{
  char ch = '\0';
  int lp = 0;

  /* read until newline or filled string */
  while( ch != '\n' && lp < (80-1) ) {
    while( !Serial.available() ) delay( 5 );
    ch = Serial.read();
    line[lp] = ch;
    lp++;
  }
  line[lp-1] = '\0';  /* terminate the string */

  // echo if we got something
  if( line[0] != '\0' ) {
    Serial.print( "   << " );
    Serial.print( line );
    Serial.println( " >>" );
  }
}


/////////////////////////////////////////
// main user input loop

void loop()
{
  readLine();
  bool err = false;

  if( !strcmp( line, "h" )) {
    Serial.println( F("       t - text") );
    Serial.println( F("       ? - settings") );
    Serial.println( F("  c<n> - cols") );
    Serial.println( F("  b<n> - baud") );
    Serial.println( F("  r<n> - rows") );
  }

  else if( !strcmp( line, "?" )) {
    newCols( cols );
    newRows( rows );
    newBaud( baud );
  }

  else if( line[0] == 'b' ) {
    newBaud( atol( &line[1] ));
  } else if( line[0] == 'c' ) {
    newCols( atol( &line[1] ));
  } else if( line[0] == 'r' ) {
    newRows( atol( &line[1] ));
  }
  
  
  else if( !strcmp( line, "t" )) printText();

  else err = true;

  if( err == true ) {
    Serial.println( F( "What?" ));
  }
}
