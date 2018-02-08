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

#define INPUT_PIN   A0
#define INPUT_GND   A1
#define INPUT_VCC   A2

#define OUTPUT_PINS_START   2
#define GROUND_PIN          8

/**
 * @brief Keep the running values of signal.
 */
int signal_[WINDOW_SIZE];

/*  Running mean of signal. */
float running_mean_ = 0.0;
int pp1 = 10;


double computeMean(  )
{
    double sum = 0.0;
    for (size_t i = 0; i < WINDOW_SIZE; i++) 
        sum += signal_[i];
    running_mean_ = sum / WINDOW_SIZE;
    return running_mean_;
}

int switchStatus( )
{
    int sum = 0;
    for (size_t i = OUTPUT_PINS_START; i < OUTPUT_PINS_START + 6; i++) 
        sum += digitalRead( i );
    return sum;
}


void appendToSignal( int sensorValue )
{
    for (size_t i = 0; i < WINDOW_SIZE - 1; i++) 
        signal_[i] = signal_[i+1];
    signal_[WINDOW_SIZE-1] = sensorValue;
}

int slow( int ca )
{
    float kh1 = 150.0;
    float frac = pow( ca / kh1, 3.0 );
    return 1024 * pow(frac, 2) / pow(( 1 + frac), 2 );
}

int fast( int ca )
{
    float kh1 = 150.0;
    float frac = pow( ca / kh1, 3.0 );
    return 1024 * frac / ( 1 + frac ); 
}

float activePP1( int ca )
{
    float kh2 = 70.0;
    float frac = pow( ca / kh2, 3.0 );
    return pp1 * ( 1 + frac ) / frac;
}

bool isClockWiseNeighbourOn( size_t pin )
{
    int pinIndex = (pin - 1 + 6 ) % 6;
    return (HIGH == digitalRead( pinIndex ));
}

int compute_phospho_rate( int ca )
{
    int state = switchStatus( );
    return (state * fast( ca ) + ( 6 - state ) * slow( ca ))/6;
}

int compute_dephospho_rate( int ca )
{
    int avgPhosphoRate = compute_phospho_rate( ca );
    int state = switchStatus( );
    if( state >= 4 )
        return pow( avgPhosphoRate , 0.4 );

    else if( state <= 2 )
        return pow( avgPhosphoRate, 2.0);
    else 
        return 2 * avgPhosphoRate;
}

void printSignal( )
{
    for (size_t i = 0; i < WINDOW_SIZE - 1; i++) 
    {
        Serial.print( signal_[i] );
        Serial.print( ' ' );
    }

    Serial.println( ' ' );
}

void system( int sensorValue )
{
    // The probability of turning any OUTPUT ON.
    for( size_t pin = OUTPUT_PINS_START; pin < OUTPUT_PINS_START + 6; pin++ )
    {
        int rateOn = 0;
        Serial.print( digitalRead( pin ) );
        Serial.print( ':' );

        if( isClockWiseNeighbourOn( pin ) )
            rateOn = fast( sensorValue );
        else
            rateOn = slow( sensorValue );

        if( random( 0, 1024 ) < rateOn )
        {
            digitalWrite( pin, HIGH );
            delay( 10 );
        }

        char msg[4];
        sprintf( msg, "%3d,", rateOn );
        Serial.print( msg );
    }

    // int rateOff = activePP1( sensorValue );
    int rateOff = compute_dephospho_rate( sensorValue );
    for( size_t pin = OUTPUT_PINS_START; pin < OUTPUT_PINS_START + 6; pin++ )
    {
        if( random(0, 1024) < rateOff )
        {
            digitalWrite( pin, LOW );
            delay( 10 );
        }
    }

    char msg[100];
#if 0
    sprintf( msg, "ca=%4d, slow=%3d, fast=%3d", sensorValue, slow( sensorValue )
            , fast( sensorValue )
           );
#else
    sprintf( msg, "ca=%4d,pp1=%4d,rOFF=%4d", sensorValue, pp1, rateOff );
#endif
    Serial.print( msg );

    for( size_t i = OUTPUT_PINS_START; i < OUTPUT_PINS_START + 6; i++)
    {
        Serial.print( ' ' );
        Serial.print( digitalRead( i ) );
    }
    Serial.print( " #On " );
    Serial.println( switchStatus( ) );
}

// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin( 38400 );

    pinMode( GROUND_PIN, OUTPUT );
    digitalWrite( GROUND_PIN, LOW );

    pinMode( INPUT_VCC, OUTPUT );
    digitalWrite( INPUT_VCC, HIGH );

    pinMode( INPUT_GND, OUTPUT );
    digitalWrite( INPUT_GND, LOW );

    // Read sensor input from here. Calcium
    pinMode( INPUT_PIN, INPUT );

    /* Initialize signal  */
    for (size_t i = 0; i < WINDOW_SIZE; i++) 
        signal_[i] = 10;

}

// the loop routine runs over and over again forever:
void loop() 
{
    // read the input on analog pin 0:
    int sensorValue = analogRead(INPUT_PIN);
    appendToSignal( sensorValue );
    printSignal( );
}
