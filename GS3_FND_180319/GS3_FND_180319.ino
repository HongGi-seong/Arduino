#define CROSS_TIME 2
#define COUNT_TIME 1000
#define FND_A 11
#define FND_B A2
#define FND_C A1
#define FND_D 13
#define FND_E A0
#define FND_F 12
#define FND_G 9
#define FND_H 10
#define FND_V0 7
#define FND_V1 8

// Numbers displayed on the FND
const char segment [ 10 ] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67 } ;			
const unsigned int pin [ 10 ] = { FND_A, FND_B, FND_C, FND_D, FND_E, FND_F, FND_G, FND_H, FND_V0, FND_V1 } ; 	
unsigned long preMillisCross, preMillisNum ;


void setup ( void )
{
	for ( int index = 0; index < 10; index++ )
		pinMode ( pin [ index ], OUTPUT );												
}


void loop ( void )
{

	unsigned long curMillis = millis ( ) ;
	int shift, tempVoltage, tempNumber ;
	static int num = 0, numTen = 0, voltage = 1 ;
	
	// Every 2 milliseconds alternating between different segments
	if ( curMillis - preMillisCross > CROSS_TIME ) {
		preMillisCross = curMillis ;
		
		voltage=!voltage;																
		if ( voltage == 1 ) {																			
			tempVoltage = FND_V1 ;
			tempNumber = numTen ;
		} else {
			tempVoltage = FND_V0 ;
			tempNumber = num ;
		}
		
		// Clear segments
		digitalWrite ( FND_V1, LOW ) ;
		digitalWrite ( FND_V0, LOW ) ;
		
		// Show number in segment
		for ( shift = 0; shift < 8; shift++ ) {															
			if ( ( segment [ tempNumber ] & ( 0x01 << shift ) ) != 0 ) 
				// Active low
				digitalWrite ( pin [ shift ] , LOW ) ;	
			else 
				digitalWrite ( pin [ shift ], HIGH ) ;
		}
			
		// Turns power on the segment alternately	
		for ( int i = FND_V0; i <= FND_V1; i++ ) {											
			if ( i == tempVoltage ) 
				digitalWrite( i, HIGH ) ;
			else 
				digitalWrite ( i, LOW ) ;
		}
	}
	
	// Increase the number displayed on the FND by 1 every second.
	if ( curMillis - preMillisNum >= COUNT_TIME ) {
		preMillisNum = curMillis ;

		num = ( num + 1 ) % 10;
		if ( num == 0 ) 
			numTen++ ;
		if ( numTen == 10 )
			numTen = 0 ;
	}
}