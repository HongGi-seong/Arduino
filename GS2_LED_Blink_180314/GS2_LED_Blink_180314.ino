#define DIVIDE_PERIOD 20
#define DUTY_INCRE_RATIO 10
#define REPEAT_PULSE 5
#define LEDPIN 5
#define COUNT_PARTS 100

int pulsePart = 0, pwmDuty = 10, countPulse = 0 ;
boolean pwmDutyIncrease = true ;


void setup ( void ) 
{
  pinMode ( LEDPIN, OUTPUT ) ;
}

void loop ( void ) 
{
	// The wavelength is divided into 100, and the PWM duty cycle is adjusted to 100 steps
	if ( ( pulsePart == 0 ) && ( pwmDuty != 0 ) ) 
		digitalWrite ( LEDPIN, HIGH ) ;
	else if ( pulsePart == pwmDuty ) 
		digitalWrite ( LEDPIN, LOW ) ;
		
	delayMicroseconds ( DIVIDE_PERIOD ) ;
	
	// When the number of the pulseparts of the COUNT_PARTS is gathered, it becomes one wavelength
	pulsePart = ( pulsePart + 1 ) % COUNT_PARTS ;
	
	if ( pulsePart == 0 ) {
		// Repeat the same PWM for the REPEAT_PULSE
		countPulse = ( countPulse + 1 ) % REPEAT_PULSE ;
		
		if ( countPulse == 0 ) {
			if ( ( pwmDuty > 99 ) || ( pwmDuty < 1 ) ) 
				pwmDutyIncrease = !pwmDutyIncrease ;
				
			if ( pwmDutyIncrease == true ) 
				pwmDuty += DUTY_INCRE_RATIO ;
			else if ( pwmDutyIncrease == false ) 
				pwmDuty -= DUTY_INCRE_RATIO ; 
		}
	}
}
