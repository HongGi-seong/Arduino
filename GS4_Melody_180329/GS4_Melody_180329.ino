#include "pitches.h"

#define MICROS 1
// #define DELAY 1
#define PIN A4


// Music to play with the buzzer
int melody [ ] = {
	NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
	NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
	NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
	NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
} ;


// Implement the Tone function as a millisecond structure
#if MICROS 

unsigned long SEC = 1000000 ;
unsigned long preMicros, preMicrosPitches, noteDuration = 4  ;
unsigned int loopFactor;
boolean pitchState = LOW ;


// The function is used to play notes
// There is no return value
// Use pitch and note length as parameter
void Tone ( int factorPitches, unsigned long factorLength ) 
{
	// Change pitch frequency to time
	int pitchesTime = ( SEC / ( factorPitches * 2 ) ) ;
	
	// If it is a comma
	if ( factorPitches == 0 )
		delayMicroseconds ( factorLength ) ;
	
	else {
		
		while ( 1 ) {
			
			unsigned long curMicrosPitches = micros ( ) ;
			
			 // Create a waveform to match the pitch
			if ( ( curMicrosPitches - preMicrosPitches ) > pitchesTime ) {
				preMicrosPitches = curMicrosPitches ;
				
				if ( pitchState == LOW ) {
					pitchState = HIGH ;
					digitalWrite ( PIN, HIGH ) ;
				} else if ( pitchState == HIGH ) {
					pitchState = LOW ;
					digitalWrite ( PIN, LOW ) ;
				}
			}
			
			// Play as long as the note length
			if ( ( curMicrosPitches - preMicros ) > factorLength ) {
				preMicros = curMicrosPitches ;
				break ;
			}
		}
	}
	
	digitalWrite ( PIN, LOW ) ;
}


void setup ( void ) 
{
	pinMode ( PIN, OUTPUT ) ;
	Serial.begin ( 9600 ) ;
}


void loop ( void )
{
	for ( loopFactor = 0; loopFactor < ( sizeof ( melody ) / sizeof ( int ) ); loopFactor++ ) { 
		unsigned long noteLength = ( SEC / noteDuration ) ;
		// The rest time between notes is the note length * 1.3
		unsigned long noteDelay = noteLength * 1.3 ;
		Tone ( melody [ loopFactor ], noteLength ) ;
		delayMicroseconds ( noteDelay ) ;
	}

	while ( 1 ) ;
}

//////////////////////////////////////END MICROS////////////////////////////////////////////////////////



// Implement tone function with delay structure
#elif DELAY
#define SEC 1000


boolean pitchState = LOW ;
unsigned int noteDuration = 4, count ;


// The function is used to play notes
// There is no return value
// Use pitch and note length as parameter
void Tone ( int factorPitches, int factorLength ) 
{
	
	if ( factorPitches == 0 ) 
		delay ( factorLength ) ; 
	
	else {
		while ( 1 ) {
		
			// Create a waveform to match the pitch
			if ( ( ( count % ( ( SEC * 5 ) / factorPitches ) ) == 0 ) ) {
				if ( pitchState == LOW ) {
					digitalWrite ( PIN, HIGH ) ;
					pitchState = HIGH ;
				} else if ( pitchState == HIGH ) {
					digitalWrite ( PIN, LOW ) ;
					pitchState = LOW ;
				}
			}
				
			count++ ;
			
			// Play as long as the note length
			if ( count >= factorLength * 10 ) {
				digitalWrite ( PIN, LOW ) ;
				pitchState = LOW ;
				count = 0 ;
				break ;
			}
	
			delayMicroseconds ( 100 ) ; 
		}
	
	}
}


void setup ( void )
{
	pinMode ( PIN, OUTPUT ) ;
}


void loop ( void )
{
	int loopFactor ;
	
	for ( loopFactor = 0; loopFactor < ( sizeof ( melody ) / sizeof ( int ) ); loopFactor++ ){ 
		int noteLength = SEC / noteDuration ;
		// The rest time between notes is the note length * 1.3
		int noteDelay = noteLength * 1.3 ;
		Tone ( melody [ loopFactor ], noteLength ) ;
		delay( noteDelay ) ;
	}
	while ( 1 ) ;
}


#endif
