#include "LiquidCrystalLib.h"

using namespace std;

LiquidCrystalLib::LiquidCrystalLib ( volatile uint8_t *content_port, volatile uint8_t *control_port, volatile uint8_t *content_port_dir, volatile uint8_t *control_port_dir, uint8_t enable_pin, uint8_t rw_pin, uint8_t rs_pin )
{
	// Set the pin numbers
	conte_port = content_port;
	contr_port = control_port;
	conte_port_dir = content_port_dir;
	contr_port_dir = control_port_dir;
	enable = enable_pin; 
	rw = rw_pin;
	rs = rs_pin; 
	
	// Turn off JTAG (very important - otherwise PORTC will output garbage no matter what!!!)
	if ( *content_port == PORTC )
	{
		MCUCSR |=(1<<JTD);
		MCUCSR |=(1<<JTD);
	}
	
	// Do whatever this does...
	init_LED();
	
}

LiquidCrystalLib::~LiquidCrystalLib(void)
{
}

void LiquidCrystalLib::pulse_enable(void)
{
	// Pulse enable
	*contr_port &= ~(1 << enable); 
	_delay_us(2);
	*contr_port |= (1 << enable); 
	_delay_us(2);
	*contr_port &= ~(1 << enable); 
	_delay_us(100);	
}

void LiquidCrystalLib::send_command(const uint8_t command)
{
	*contr_port_dir |= (1 << enable | 1 << rw | 1 << rs);
	_delay_ms(50);		// Per Arduino LiquidCrystal.cpp
	
	// Pull RS and R/W low to begin commands
	*contr_port &= 0 << rs;
	*contr_port &= 0 << rw;
	*contr_port &= 0 << enable;
	
	*conte_port = command;
	
	pulse_enable();

}

void LiquidCrystalLib::init_LED(void) 
{
	uint8_t display_function = (LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS);
	uint8_t display_control = 0; 
	
	
	// Set LCD control direction to output
	*conte_port_dir = 0b11111111;
	*contr_port_dir |= (1 << enable | 1 << rw | 1 << rs);

	_delay_ms(50);		// Per LiquidCrystal.cpp

	// Pull RS and R/W low to begin commands
	*contr_port &= 0 << rs;
	*contr_port &= 0 << rw;
	*contr_port &= 0 << enable;
	
	// Send the init commands to the data port
	// Try three times 
	*conte_port = LCD_FUNCTIONSET | display_function;
	// Pulse enable
	pulse_enable();
	_delay_us(5000);

	// Turn the display on
	display_control |= (LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON);
	*conte_port =  display_control; 
	pulse_enable(); 
	_delay_us(5000);
	
	*conte_port = LCD_CLEARDISPLAY;
	pulse_enable();
	_delay_us(2000);
	
	*conte_port = (LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT); 
	pulse_enable(); 
}

void LiquidCrystalLib::clear()
{
	send_command(LCD_CLEARDISPLAY);
}

void LiquidCrystalLib::write_char(uint8_t value) 
{
	//LCD_CONTROL_DIR = 0xFF;
	//*contr_port = 0xFF;
	*contr_port |= (1 << rs);
	
	//*contr_port |= 1 << rs;
	//*contr_port &= 0 << rw;
	//*contr_port &= 0 << enable;

	*conte_port = value; 

	pulse_enable();
	//_delay_ms(5);

}

void LiquidCrystalLib::write_str(char text[])
{
	int i = 0;
	while(text[i] != '\0' && i < 16)
	{
		write_char( (uint8_t) text[i] );
		i ++;
	}
}

void LiquidCrystalLib::LCD_on(void)
{
	*conte_port =  0b00001111; 
	pulse_enable(); 
	_delay_ms(50);
}

void LiquidCrystalLib::LCD_off(void)
{
	*conte_port =  0b00001000; 
	pulse_enable(); 
	_delay_ms(50);
}
