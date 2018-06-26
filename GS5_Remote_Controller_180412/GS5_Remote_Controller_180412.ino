#define MIN_ZERO 1000
#define MAX_ZERO 1250
#define MIN_ONE 2125
#define MAX_ONE 2375
#define MIN_LEAD 13000
#define MAX_LEAD 14000
#define COUNT_BIT 8


volatile unsigned long preMicros, intervalTime, interruptState ;
char number, numberButton [ 10 ] = { 0x6a, 0x42, 0xc2, 0xf0, 0x78, 0xb8, 0x38, 0x18, 0xa2, 0x32 } ;


// This function is used to measure the time between falling edges ( ISR ) 
// There is no return value 
// THere is no parameters
volatile void RemoteContorlInterrupt ( void )																			
{																													
	volatile unsigned long curMicros = micros ( ) ;
	
	interruptState = HIGH ;																							

	intervalTime = curMicros - preMicros ;

	preMicros = curMicros ;
}


void setup ( void )
{
	Serial.begin ( 9600 ) ;
	pinMode ( 2, INPUT ) ;
	
	attachInterrupt ( 0, RemoteContorlInterrupt, FALLING ) ;
}


void loop ( void )
{
	static unsigned int digitCount = 0 ; 
	static boolean leadCheck = false ;
	unsigned char matchNumber ;
	
	if ( interruptState == HIGH ) {																												
		// Once the lead code is verified
		if ( leadCheck == true ) {		
			// Only the button code is masked
			if ( digitCount > ( COUNT_BIT * 2 - 1 ) && digitCount < ( COUNT_BIT * 3 )  )  {								
				if ( ( intervalTime > MIN_ZERO ) && ( intervalTime < MAX_ZERO ) ) 
					number = number | ( 0 << 23 - digitCount ) ;
				else if ( ( intervalTime > MIN_ONE ) && ( intervalTime < MAX_ONE ) ) 
					number = number | ( 1 << 23 - digitCount ) ;
			}
			
			digitCount = ( digitCount + 1 ) % ( COUNT_BIT * 4 ) ;													
		}
		
		// After verifying the 32-bit code
		if ( digitCount == 0 ) {		
			// Initialize lead code status
			leadCheck = false ;		
			// Output button pressed on remote controller to serial window
			for ( matchNumber = 0; matchNumber < 10; matchNumber++ ) {
				if ( numberButton [ matchNumber ] == number ) {
					Serial.print ( "button " ) ;
					Serial.print ( matchNumber ) ;
					Serial.println ( " is pressed " ) ;
					break ;
				}
			}
			// initialize variable number
			number = 0 ;																							
		}
		
		// Verify lead code
		if ( ( leadCheck == false ) && ( intervalTime > MIN_LEAD ) && ( intervalTime < MAX_LEAD ) ) {								
			leadCheck = true ;
		} 

		interruptState = LOW ;
	}
}
