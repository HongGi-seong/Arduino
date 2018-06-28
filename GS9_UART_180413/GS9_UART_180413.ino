
#define PIN_TX 1
#define PIN_RX 0
#define SIGNAL_HIGH digitalWrite ( PIN_TX, HIGH ) ;
#define SIGNAL_LOW digitalWrite ( PIN_TX, LOW ) ;


const unsigned char display [ 9 ] = "UART_Test"  ;


void setup ( void ) 
{
	pinMode ( PIN_TX, OUTPUT ) ;
	pinMode ( PIN_RX, INPUT ) ;
	SIGNAL_HIGH ;
	delayMicroseconds ( 200 ) ;
}


void loop ( void )
{
	static unsigned char charData = 0 ;
	unsigned char shiftBit ;
	
	// Start ( 9600 Baudrate, 8 bits, no parity, 1 stop bit )  
	SIGNAL_LOW ;
	delayMicroseconds ( 100 ) ;
	
	for ( shiftBit = 0; shiftBit < 8; shiftBit++ ) {
	
		// There is a bug that can not be compiled without brackets
		if ( ( display [ charData ] & ( 0x01 << shiftBit ) ) == 0 ) {
			SIGNAL_LOW ;
		}
		else {
			SIGNAL_HIGH ;
		}
		
		delayMicroseconds ( 100 ) ;
	}
	
	charData = ++charData % 9 ;
	
	SIGNAL_HIGH ;
	delayMicroseconds ( 200 ) ;
	
	if ( charData == 0 ) 
		while ( 1 ) ;
}
