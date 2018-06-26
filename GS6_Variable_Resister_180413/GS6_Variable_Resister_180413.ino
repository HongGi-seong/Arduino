#define CORRECTION_VALUE (double)10000
#define DIGITAL_STEPS (double)1023


const unsigned int pinVar = A5 ;
unsigned long varAnalogValue, preValue ;
double result ;


void setup ( void ) 
{
	Serial.begin ( 9600 ) ;
	
	pinMode ( pinVar, INPUT ) ;
}


void loop ( void ) 
{  
	varAnalogValue = analogRead ( pinVar ) ;
	
	// If the analog value changes
	if ( (  ( long )varAnalogValue - ( long )preValue ) != 0 ) { 
		preValue = varAnalogValue ;
		
		result = CORRECTION_VALUE * ( ( double )varAnalogValue / DIGITAL_STEPS ) ;
		
		Serial.println ( result ) ;
	}

	delay ( 300 ) ;
}
