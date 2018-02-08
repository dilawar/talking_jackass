/***
 *       Filename:  jackass.ino
 *
 *    Description:  Project file.
 *
 *        Version:  0.0.1
 *        Created:  2016-07-10

 *       Revision:  none
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *
 *        License:  GNU GPL2
 */

#define WINDOW_SIZE  200

#define SOUND_INPUT_A       A0
#define SOUND_INPUT_B       A1

#define RECORD_PIN          4
#define PLAY_PIN            6  // play by level

#define RECORD_TIME         5000

/**
 * @brief Keep the running values of signal.
 */
unsigned int signal_[WINDOW_SIZE];

/*  Running mean of signal. */
float running_mean_ = 0.0;
int pp1 = 10;

unsigned long time = 0;

// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin( 38400 );

    pinMode( SOUND_INPUT_A, INPUT );
    pinMode( SOUND_INPUT_B, INPUT );

    pinMode( RECORD_PIN, OUTPUT );
    pinMode( PLAY_PIN, OUTPUT );

    digitalWrite( RECORD_PIN, LOW );
}

// the loop routine runs over and over again forever:
void loop() 
{
    // read the input on analog pin 0:
    unsigned long time0 = millis( );

    time = millis( ) - time0;
    unsigned stamp = millis( ) - time0;

    digitalWrite( RECORD_PIN, HIGH );
    delay( RECORD_TIME );

    Serial.println( "Done recording. ..." );
    digitalWrite( RECORD_PIN, LOW );
    digitalWrite( PLAY_PIN, HIGH );
    stamp = millis( ) - time0;

    long sum = 0;
    int a, b;
    while( (millis( ) - stamp - time0) <= (RECORD_TIME + 100) )
    {
        b = analogRead( SOUND_INPUT_B );
        a = analogRead( SOUND_INPUT_A );
        //Serial.print( b ); Serial.print( ' ' ); Serial.println( a );
        if( a < 1024 && b < 1024 )
            sum += abs( b - a );
    }

    stamp = millis( ) - time0;

    if( sum > 500 )
    {
        Serial.println( "Over the top. Annoy" );
    }

    Serial.println( sum );
    digitalWrite( PLAY_PIN, LOW );
    digitalWrite( RECORD_PIN, LOW );

}
