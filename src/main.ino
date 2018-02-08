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

int speakerPin = 9;

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) 
{
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) 
{
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) 
    {
        if (names[i] == note) 
        {
            Serial.print( "Playing tone " );
            Serial.println( note );
            playTone(tones[i], duration);
        }
    }
}

void setup( )
{
    Serial.begin( 38400 );

    pinMode( speakerPin, OUTPUT );
    pinMode( 8, OUTPUT );
    digitalWrite( 8, LOW );
}

void play( )
{
    char melody[] = "ccggaag";

    for (size_t i = 0; i < 15; i++) 
        playNote( melody[i], 200 );
}


// the loop routine runs over and over again forever:
void loop() 
{
    if( Serial.available( ) > 0)
    {
        int incoming = Serial.read( );
        Serial.println( incoming );

        if( incoming == 80 )
        {
            Serial.println( "Play notes" );
            play( );
        }
    }
}
