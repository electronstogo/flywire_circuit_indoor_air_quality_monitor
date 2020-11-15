#ifndef SGP30_H_
#define SGP30_H_


#include <Wire.h>

#define SGP30_I2C_ADDRESS 	0x58
#define SGP30_CRC8_POLYNOMIAL 0x31

#define CONCAT_BYTES(msb, lsb)             (((uint16_t)msb << 8) | (uint16_t)lsb)


class SGP30Sensor
{
	private:
		uint16_t get_featureset();
		uint8_t generate_crc(uint8_t *data, uint8_t data_length);
		
		
	public:
		SGP30Sensor();
		void do_raw_measurement(uint16_t *ethanol_raw_measurement, uint16_t *h2_raw_measurement);
		void do_measurement(uint16_t *tvoc_measurement, uint16_t *co2_measurement);
		void get_baseline(uint16_t *co2_baseline, uint16_t *tvoc_baseline);
		uint16_t get_abs_humidity_from_rel_humidity(uint16_t rel_humidity, int16_t temperature);
		void set_absolute_humidity(uint16_t abs_humidity);
		
		
		uint16_t featureset;
};




#endif
