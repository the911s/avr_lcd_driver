#ifndef _LIQUIDCRYSTALLIB_
#define _LIQUIDCRYSTALLIB_

#include <avr/io.h>
#include <util/delay.h>

#include "HD44780_codes.h"

class LiquidCrystalLib {

	public: 
		
		LiquidCrystalLib(void) {};
		LiquidCrystalLib ( volatile uint8_t *content_port, volatile uint8_t *control_port, volatile uint8_t *content_port_dir, volatile uint8_t *control_port_dir, uint8_t enable_pin, uint8_t rw_pin, uint8_t rs_pin );
		~LiquidCrystalLib (void); 
	
		void clear();
	
		void write_char(uint8_t value);
		void write_str(char text[]);
		
		
		
	private: 
		
		// Variables
		volatile uint8_t *conte_port;
		volatile uint8_t *contr_port;
		volatile uint8_t *conte_port_dir;
		volatile uint8_t *contr_port_dir;
		uint8_t enable;
		uint8_t rw;
		uint8_t rs;
		
		// Methods
		void send_command(const uint8_t command);
		void pulse_enable(void);
		void init_LED(void);
		void LCD_on(void);
		void LCD_off(void);
		
};

#endif


