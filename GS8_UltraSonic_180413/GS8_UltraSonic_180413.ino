

#define POLLING 1
// #define INTERRUPT 1
// #define SRF05 1

#define PERIOD 100000


const unsigned char pinTrig = 4, pinEcho = 3 ;
unsigned long preMicrosTrigger, preMicrosEcho, highTime, lowTime, intervalTime ;
unsigned char preState = LOW, curState = LOW ;


// Measure distance with ultrasonic sensor using polling method 
// Use ultra_sound_SRF04_SRF05.ini file 
#if POLLING 

void setup ( void )
{
	Serial.begin ( 9600 ) ;

	pinMode ( pinTrig, OUTPUT) ;																
	pinMode ( pinEcho, INPUT) ;																	
}


void loop ( void )
{
	unsigned long curMicros = micros ( ) ;

	// Triggers every 100 ms
	if( curMicros - preMicrosTrigger >= PERIOD ) {
		preMicrosTrigger = curMicros ;
		
		digitalWrite ( pinTrig, HIGH );
		delayMicroseconds( 10 ) ;
		digitalWrite ( pinTrig, LOW ) ;
	}
	
	if ( curMicros - preMicrosEcho > 5 ) {
		// Check edge's level
		if ( digitalRead ( pinEcho ) != 0 ) 
			curState = HIGH ;
		else
			curState = LOW ;
		
		
		// measure the time between two edges
		if ( preState != curState ) {
			preState = curState ;
		
			if ( curState == HIGH ) 
				highTime = micros ( ) ;
			else if ( curState == LOW ) {
				lowTime = micros ( ) ;
				intervalTime = lowTime - highTime ;
				Serial.print ( intervalTime / 58 ) ;
				Serial.println ( " cm" ) ;
			}
		}
	}
}
/////////////////////////////// end of polling /////////////////////////



// Measure distance with ultrasonic sensor using interrupt method
#elif INTERRUPT 

volatile int count ;


// This function is used to measure the distance between triggers
// There is no return value
// There is no parameter
volatile void UltraSoundInterrupt ( void ) 
{
	if ( count == 0 ) 
		highTime = micros ( ) ;
	
	else {
		lowTime = micros ( ) ;
		intervalTime = lowTime - highTime ;
		Serial.println ( intervalTime / 58 ) ;
	}
	
	// count zero is rising edge and count one is falling edge
	count = ( count + 1 ) % 2 ;
}


void setup ( void )
{
	Serial.begin ( 9600 ) ;

	pinMode ( pinTrig, OUTPUT ) ;
	pinMode ( pinEcho, INPUT ) ;

	attachInterrupt ( 0, UltraSoundInterrupt, CHANGE ) ;
}

 
void loop ( void )
{
	unsigned long curMicros = micros ( ) ;
	
	// triggers every 100 ms 
	if ( curMicros - preMicrosTrigger >= PERIOD ) {
		preMicrosTrigger = curMicros ;
		
		digitalWrite ( pinTrig, HIGH ) ;
		delayMicroseconds ( 10 ) ;
		digitalWrite ( pinTrig, LOW ) ;
	}
}
/////////////////////////////// end of INTERRUPT /////////////////////////


// Use the srf05 module
// This section is for simulation ( Keil )
#elif SRF05


void setup ( void )
{
	pinMode ( GPIO_C, 10, OUTPUT ) ;																
}


void loop ( void )
{
	unsigned long curMicros = CheckMicros ( ) ;
	
	
	// triggers every 100 ms
	if( curMicros - preMicrosTrigger >= PERIOD ) {
		preMicrosTrigger = curMicros ;
		
		pinMode ( GPIO_C, 10, OUTPUT ) ;
		digitalWriteBit( GPIO_C, 10, HIGH );
		Delay_us ( 10 ) ;
		digitalWriteBit( GPIO_C, 10, LOW ) ;
	}
	
	if ( curMicros - preMicrosEcho > 5 ) {
		pinMode ( GPIO_C, 10, INPUT ) ;
		// check edge's level
		if ( digitalReadBit ( GPIO_C, 10 ) != 0 ) 
			curState = HIGH ;
		else
			curState = LOW ;
		
		
		// measure the time between two edges
		if ( preState != curState ) {
			preState = curState ;
		
			if ( curState == HIGH ) 
				highTime = CheckMicros ( ) ;
			else if ( curState == LOW ) {
				lowTime = CheckMicros ( ) ;
				intervalTime = lowTime - highTime ;
				printf ( "%ld cm\n", intervalTime / 58 ) ;
			}
		}
	}
}
/////////////////////////////// end of SRF05 /////////////////////////
#endif

