// 2020 - electronstogo

#include <avr/sleep.h>
#include "sgp30.h"
#include "bme280.h"
#include "ssd1306.h"
#include <Wire.h>
#include <stdio.h>





/**
* RTC handling stuff.
*/

#define RTC_I2C_ADDRESS 0x68



// Sets the time in the RTC registers, with the given arguments.
// This function should only be used, when its neccessary to set the correct time.
void set_rtc_time(int hours, int minutes, int seconds)
{
	Wire.beginTransmission(RTC_I2C_ADDRESS);
	Wire.write(0x0);
	Wire.write(((seconds / 10) << 4) + (seconds % 10));
	Wire.write(((minutes / 10) << 4) + (minutes % 10));
	// set hours, and activate 24 hr mode.
	byte hour_byte = ((hours / 10) << 4) + (hours % 10);
	Wire.write(hour_byte & 0b10111111);
	Wire.write(hour_byte);
	Wire.endTransmission();
  
}


// read time data from RTC controller DS3231. 
void get_rtc_time_values(unsigned int* hours, unsigned int* minutes, unsigned int* seconds)
{
	// Start reading the RTC registers from address 0x0.
	Wire.beginTransmission(RTC_I2C_ADDRESS);
	Wire.write(0x0);
	Wire.endTransmission();


	// Get 3 bytes (seconds register, minutes register, hours register) 
	Wire.requestFrom(RTC_I2C_ADDRESS, 3);
	*seconds = bcd_to_dec(Wire.read() & 0x7F);
	*minutes = bcd_to_dec(Wire.read() & 0x7F);
	*hours = bcd_to_dec(Wire.read() & 0x7F);
}


// get dec format number, from bcd
byte bcd_to_dec(byte bcd_value)
{
	return (bcd_value & 0x0F) + (bcd_value >> 4) * 10;
}






/**
* Microcontroller handling stuff.
*/


// init stuff for microcontroller.
void init_mcu()
{
	// enable external interrupt rising edge at INT0 pin
	EICRA = 0x03;
	EIMSK = 0x01;
	EIFR  = 0x01;	
}



// external interrupt at INT0 pin, triggered by RTC 1Hz signal.
ISR (INT0_vect)
{
	// just wake up controller, for a display refresh.
}







/**
* Main program handling stuff.
*/


// Numbers of digital inputs for button handling.
#define BUTTON_PIN_RIGHT	4
#define BUTTON_PIN_LEFT		12 


// Arduino init
void setup()
{
	// disable interrupts
	cli();
	
	// init digital inputs for buttons.
	pinMode(BUTTON_PIN_RIGHT, INPUT);
	pinMode(BUTTON_PIN_LEFT, INPUT);
	
	//activate pull ups for buttons.
	digitalWrite(BUTTON_PIN_RIGHT, HIGH);
	digitalWrite(BUTTON_PIN_LEFT, HIGH);
	
	
	// init microcontroller stuff
	init_mcu();
	
	// init arduino i2c framework class.
	Wire.begin();
	
	// enable interrupts
	sei();
}



// Arduino while loop
void loop()
{	
	// Initiate the class variables.
	// Attention: This doesnt work during the Arduino setup() function,
	// because the I2C communication in the native Wire (TWI)library class will stuck.
	
	
	// SSD1306 controller to control oled.
	SSD1306 ssd1306(0x3C);
	SSD1306 ssd1306_2(0x3D);
	
	// Initiate a bme280 sensor class variable.
	BME280Sensor bme280_sensor;
	
	// Initiate a sgp30 sensor class variable.
	SGP30Sensor sgp30_sensor;
	
	

	// Following communication sets the control register of the RTC, for the 1Hz interrupt.
	// This is only needed one time, at the first use!
	//Wire.beginTransmission(RTC_I2C_ADDRESS);
	//Wire.write(0x0E);
	//Wire.write(0x40);
	//Wire.endTransmission();

	// Set the RTC time here.
	// This function should only be used, when its neccessary to set the correct time.
	//set_rtc_time(20, 40, 0);

	
	int32_t temperature;
	uint32_t pressure, humidity;
	uint16_t tvoc_measurement, co2_measurement;
	unsigned int hours, minutes, seconds;
	
	// flag that counts the time how long a button is pushed.
	unsigned int button_active_time = 0;
	
	uint8_t loop_counter = 0;
	
	// values for string handling.
	char* char_pointer;
  	String string_buffer;


	while(1)
	{
		// get measurements from BME280 sensor.
		bme280_sensor.do_humidity_temperature_pressure_measurement(&temperature, &pressure, &humidity);
		
		ssd1306.clear_oled_buffer();
		
		//concatenate humidity.
		string_buffer = String((float)humidity / 1000.0, 1);
		string_buffer += "% R.H.";
		string_buffer += '\0';
		char_pointer = string_buffer.c_str();
		ssd1306.draw_string(10, 40, char_pointer);
		
		// concatenate temperature.
		string_buffer = String(temperature / 100.0, 1);
		string_buffer += "$C";
		string_buffer += '\0';
		char_pointer = string_buffer.c_str();
		ssd1306.draw_string(10, 60, char_pointer);
		
		// pressure.
		string_buffer = String(pressure);
		string_buffer += " MBAR";
		string_buffer += '\0';
		char_pointer = string_buffer.c_str();
		ssd1306.draw_string(10, 20, char_pointer);
		
		ssd1306.flush_oled_buffer();

		
		
		
		// calculate the absolute humidity, and write it into the SGP30 memory, every 4 seconds.
		if(!(loop_counter % 4))
		{
			uint16_t abs_humidity = sgp30_sensor.get_abs_humidity_from_rel_humidity(humidity / 100, temperature / 10);
			sgp30_sensor.set_absolute_humidity(abs_humidity);
		}
		
		
		// get measurements from SGP30 sensor.
		sgp30_sensor.do_measurement(&tvoc_measurement, &co2_measurement);
		
		
		
		ssd1306_2.clear_oled_buffer();
		
		string_buffer = String(tvoc_measurement);
		string_buffer += " PPB";
		string_buffer += '\0';
		char_pointer = string_buffer.c_str();
		ssd1306_2.draw_string(10, 60, char_pointer);
		
		string_buffer = String(co2_measurement);
		string_buffer += " PPM";
		string_buffer += '\0';
		char_pointer = string_buffer.c_str();
		ssd1306_2.draw_string(10, 40, char_pointer);

		
		
		
		// read the current time from RTC.
		get_rtc_time_values(&hours, &minutes, &seconds);
		
		
		
		// check the time set buttons, and handle the time modification.
		bool pin_left_active = !digitalRead(BUTTON_PIN_LEFT);
		bool pin_right_active = !digitalRead(BUTTON_PIN_RIGHT);
		
		signed int hours_temp, minutes_temp;
		
		// check if left button is active, while right button is inactive.
		if(pin_left_active && !pin_right_active)
		{
			minutes_temp = (button_active_time > 3) ? minutes - 10 : minutes - 1;

			if(minutes_temp < 0)
			{
				minutes_temp = minutes_temp + 60;
				hours_temp = hours - 1;
				
				hours = (hours_temp < 0) ? 23 : hours_temp;
			}
			
			minutes = minutes_temp;
			set_rtc_time(hours, minutes, seconds);
			button_active_time++;
			
		}
		// check if right button is active, while left button is inactive.
		else if(!pin_left_active && pin_right_active)
		{
			minutes_temp = (button_active_time > 3) ? minutes + 10 : minutes + 1;
				
			if(minutes_temp > 59)
			{
				minutes_temp = minutes_temp - 60;
				hours_temp = hours + 1;
				
				hours = (hours_temp >= 24) ? 0 : hours_temp;
			}
			
			minutes = minutes_temp;
			set_rtc_time(hours, minutes, seconds);
			button_active_time++;
		}
		else // reset the active button time, if no button or both buttons are active.
		{
			button_active_time = 0;
		}
			
		
		
		// Transform a 2 digit string for display.
		char seconds_string[10];
		sprintf(seconds_string, "%i%i", seconds / 10, seconds % 10);
		
		// Transform a 2 digit string for display.
		char minutes_string[10];
		sprintf(minutes_string, "%i%i", minutes / 10, minutes % 10);
		
		// Transform a 2 digit string for display.
		char hours_string[10];
		sprintf(hours_string, "%i%i", hours / 10, hours % 10);
		
		string_buffer = String(hours_string);
		string_buffer += ":";
		string_buffer += String(minutes_string);
		string_buffer += ":";
		string_buffer += String(seconds_string);
		string_buffer += '\0';
		char_pointer = string_buffer.c_str();
		ssd1306_2.draw_string(10, 20, char_pointer);
		
		ssd1306_2.flush_oled_buffer();
		
		
		// enter sleep mode, no need for a active controller, until the next display refresh.
		// RTC 1Hz output will awake controller through a extern interrupt.
		SMCR |= (1 << 0);
		SMCR |= (1 << 2);
		sleep_cpu();
		
		loop_counter++;
	}
}
