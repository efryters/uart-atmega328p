/**
 * USART module comms with the arduino
 * Eric Fryters
 * Purpose: Enable the use of the USART module and use it to interface with rs-232
 * Framing config ----
 * Baud: 9600
 * Data bits: 8
 * Stop bits: 1
 * No parity
 */

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR ((F_CPU / (BAUD * 16UL)) - 1 )

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/**
 * Method Prototypes
 */
void init_usart(void);
uint8_t rx_usart(void);
void tx_usart( uint8_t c);
void tx_usart_str( char *str);

char let = ' ';

int main(void)
{
    
    init_usart();
    while (1)
    {
        _delay_ms(500);
        tx_usart(let);

        let = rx_usart();
    }

    return 0;
}

void init_usart(void)
{
    //Set baud
    UBRR0H = (uint8_t)(UBRR >> 8);
    UBRR0L = (uint8_t)UBRR;
    //Enable rx/tx
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    //Set frame format 8N1
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

uint8_t rx_usart(void)
{
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void tx_usart(uint8_t c)
{
    while(!(UCSR0A & ( 1 << UDRE0 ))); //waits until something was received
    UDR0 = c;
}
