




#define F_CPU 16000000UL  // 8 MHz

/*Very Important - change F_CPU to match target clock 
  Note: default AVR CLKSEL is 1MHz internal RC
  This program transmits continously on USART. Interrupt is used for 
	Receive charactor, which is then transmitted instead. LEDs are used 
	as a test. Normal RX routine is included but not used.
  Change USART_BAUDRATE constant to change Baud Rate
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define baud rate
#define USART_BAUDRATE 9600   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

volatile unsigned int value;  
/* This variable is volatile so both main and RX interrupt can use it.
It could also be a uint8_t type */

/* Interrupt Service Routine for Receive Complete 
NOTE: vector name changes with different AVRs see AVRStudio -
Help - AVR-Libc reference - Library Reference - <avr/interrupt.h>: Interrupts
for vector names other than USART_RXC_vect for ATmega32 */
int i =0;
ISR(USART_RXC_vect){
unsigned char k;
 k= UDR;             //read UART register into value
 USART_SendByte(k);
 // value=~value ;
            // output inverted value on LEDs (0=on)
   //PORTB=~PORTB;
  //value=~value;
  i++;
  //if(i%2==0)
  {
    value=~value;
  }                 
   PORTB = value;
    UCSRA &=0b01111111;
}

void USART_Init(void){
   // Set baud rate
   UBRRL = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
   UBRRH = (BAUD_PRESCALE >> 8); 
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/ 

  // Enable receiver and transmitter and receive complete interrupt 
  UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));
}


void USART_SendByte(uint8_t u8Data){

  // Wait until last byte has been transmitted
  while((UCSRA &(1<<UDRE)) == 0);

  // Transmit data
  UDR = u8Data;
}


// not being used but here for completeness
      // Wait until a byte has been received and return received data 
uint8_t USART_ReceiveByte(){
  while((UCSRA &(1<<RXC)) == 0);
  return UDR;
}

void Led_init(void){
   //outputs, all off
	 DDRB =0xFF;       
   PORTB = 0xFF; 
_delay_ms(1000);
PORTB=0x00;
_delay_ms(1000);
PORTB = 0xFF; 
_delay_ms(1000);
PORTB=0x00;
_delay_ms(1000);
}

int main(void){
   USART_Init();  // Initialise USART
   sei();         // enable all interrupts
   Led_init();    // init LEDs for testing
   value = 'A'; //0x41;    
   PORTB = ~value; // 0 = LED on
  
  // Serial.print('d');
   //Serial.print(value);
   for(;;){    // Repeat indefinitely
             
   //  USART_SendByte(value);  // send value 
//     _delay_ms(2000);  
    
		         // delay just to stop Hyperterminal screen cluttering up 

   }
   // Serial.begin(9600);
 //Serial.print("saf");
}
	
