#include "sgp30.h"
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

#define	F_CPU   20000000UL





SGP30Sensor::SGP30Sensor()
{   
	// do soft reset.
	/*Wire.beginTransmission(SGP30_I2C_ADDRESS);
	Wire.write(0x0);
	Wire.write(0x06);
	Wire.endTransmission();*/
	
	// wait for soft reset.
	//_delay_ms(10);
	
	
	// do inititialization.
  	Wire.beginTransmission(SGP30_I2C_ADDRESS);
  	Wire.write(0x20);
	Wire.write(0x03);
	Wire.endTransmission();
	
	// wait for initialization.
	_delay_ms(10);
	
	
	// get featureset
	this->featureset = this->get_featureset();
}









void SGP30Sensor::do_raw_measurement(uint16_t *ethanol_raw_measurement, uint16_t *h2_raw_measurement)
{
	// trigger raw measurement.    
	Wire.beginTransmission(SGP30_I2C_ADDRESS);
  	Wire.write(0x20);
	Wire.write(0x50);
	Wire.endTransmission();

	// wait until measurement is finished.
    _delay_ms(25);

	uint8_t io_buffer[6];
	Wire.requestFrom(SGP30_I2C_ADDRESS, 6);
	for(int i = 0; i < 6; i++)io_buffer[i] = Wire.read();
    
	
	*ethanol_raw_measurement = CONCAT_BYTES(io_buffer[0], io_buffer[1]); 
	*h2_raw_measurement = CONCAT_BYTES(io_buffer[3], io_buffer[4]);
}




void SGP30Sensor::do_measurement(uint16_t *tvoc_raw_measurement, uint16_t *co2_raw_measurement)
{
	// trigger measurement.    
	Wire.beginTransmission(SGP30_I2C_ADDRESS);
  	Wire.write(0x20);
	Wire.write(0x08);
	Wire.endTransmission();

	// wait until measurement is finished.
    _delay_ms(12);

	uint8_t io_buffer[6];
	Wire.requestFrom(SGP30_I2C_ADDRESS, 6);
	for(int i = 0; i < 6; i++)io_buffer[i] = Wire.read();
    
	
	*co2_raw_measurement = CONCAT_BYTES(io_buffer[0], io_buffer[1]); 
	*tvoc_raw_measurement = CONCAT_BYTES(io_buffer[3], io_buffer[4]);
}





void SGP30Sensor::get_baseline(uint16_t *co2_baseline, uint16_t *tvoc_baseline)
{
	// trigger baseline measurement.    
	Wire.beginTransmission(SGP30_I2C_ADDRESS);
  	Wire.write(0x20);
	Wire.write(0x15);
	Wire.endTransmission();

	// wait until measurement is finished.
    _delay_ms(10);

	uint8_t io_buffer[6];
	Wire.requestFrom(SGP30_I2C_ADDRESS, 6);
	for(int i = 0; i < 6; i++)io_buffer[i] = Wire.read();
    
	
	*co2_baseline = CONCAT_BYTES(io_buffer[0], io_buffer[1]); 
	*tvoc_baseline = CONCAT_BYTES(io_buffer[3], io_buffer[4]);
}



uint16_t SGP30Sensor::get_abs_humidity_from_rel_humidity(uint16_t rel_humidity, int16_t temperature)
{
	float rel_humidity_float = (float)rel_humidity / 10.0;
	float temperature_float = (float)temperature / 10.0;


	return (uint16_t)(216.7 * fabs(((rel_humidity_float / 100.0) * 6.112 *
		   exp((17.62 * temperature_float) / (243.12 + temperature_float))) / (273.15 + temperature_float)) * 100.0);
}



// sets the absolute humidity in the intern SGP30 sensor memory, for more accurate measurement results.
void SGP30Sensor::set_absolute_humidity(uint16_t abs_humidity)
{
	Wire.beginTransmission(SGP30_I2C_ADDRESS);
  	Wire.write(0x20);
	Wire.write(0x16);
	
	// send absolute humidity data
	Wire.write(abs_humidity / 100);
	Wire.write(abs_humidity % 100);
	
	uint8_t crc_data_buffer[2] = {abs_humidity / 100, abs_humidity % 100};
	Wire.write(this->generate_crc(crc_data_buffer, 2));
	
	Wire.endTransmission();


}


uint8_t SGP30Sensor::generate_crc(uint8_t *data, uint8_t data_length)
{
  uint8_t crc = 0xFF;

  for (uint8_t i = 0; i < data_length; i++)
  {
    //crc ^= (data >> (i * 8)) & 0xFF;
	crc ^= data[i];
    
    for (uint8_t b = 0; b < 8; b++)
    {
      if (crc & 0x80) crc = (crc << 1) ^ SGP30_CRC8_POLYNOMIAL;
      else crc <<= 1;
    }
  }
  
  return crc;
}



uint16_t SGP30Sensor::get_featureset()
{
	Wire.beginTransmission(SGP30_I2C_ADDRESS);
  	Wire.write(0x20);
	Wire.write(0x2F);
	Wire.endTransmission();
	
	
	uint8_t io_buffer[3];
	Wire.requestFrom(SGP30_I2C_ADDRESS, 3);
	for(int i = 0; i < 3; i++)io_buffer[i] = Wire.read();
	
	
	return CONCAT_BYTES(io_buffer[0], io_buffer[1]) & 0xF0; 
}
