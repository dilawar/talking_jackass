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

#define WINDOW_SIZE 20

#define SOUND_VCC           5
#define SOUND_GND           6
#define SOUND_INPUT         7

#define OUTPUT_PINS_START   2
#define GROUND_PIN          8
#define RECORD_PIN          9
#define PLAY_PIN            11  // play by level.

/**
 * @brief Keep the running values of signal.
 */
int signal_[WINDOW_SIZE];

/*  Running mean of signal. */
float running_mean_ = 0.0;
int pp1 = 10;

unsigned int time = 0;

// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin( 38400 );

    pinMode( SOUND_INPUT, OUTPUT );
    pinMode( SOUND_VCC, OUTPUT );
    pinMode( SOUND_INPUT, OUTPUT );

    digitalWrite( SOUND_VCC, HIGH );
    digitalWrite( SOUND_GND, LOW );

    pinMode( GROUND_PIN, OUTPUT );

    pinMode( RECORD_PIN, OUTPUT );
    pinMode( PLAY_PIN, OUTPUT );

    digitalWrite( GROUND_PIN, LOW );
    digitalWrite( RECORD_PIN, LOW );
}

// the loop routine runs over and over again forever:
void loop() 
{
    // read the input on analog pin 0:
    time = millis( );

    int sig = digitalRead( SOUND_INPUT );
    Serial.println( sig );

    if( time % 1000 == 0 )
    {
        Serial.println( "1 sec over" );

        // Start recording 
        digitalWrite( RECORD_PIN, LOW );

        // Delay for 100 ms.
        delay( 100 );
    }
    else
    {
        if( digitalRead( RECORD_PIN ) == LOW )
            digitalWrite( RECORD_PIN, HIGH );
    }
}
