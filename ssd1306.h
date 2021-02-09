// 2020 - electronstogo

#ifndef SSD1306_H_
#define SSD1306_H_



#define SSD1306_I2C_ADDRESS 	0x3C//0x3D


#define OLED_WIDTH 128
#define OLED_HEIGHT 64



class SSD1306
{
	private:
		void write_command(unsigned char command);
		void write_data(unsigned char *data, unsigned data_length);
		void set_page(unsigned int page);
		void draw_point(unsigned char x, unsigned char y);
		void draw_letter(unsigned int x_pos, unsigned int y_pos, unsigned int char_index);
		
		unsigned char i2c_address;
		
	public:
		SSD1306(unsigned char ssd1306_i2c_address = SSD1306_I2C_ADDRESS);
		void clear_oled_buffer();
		void flush_oled_buffer();
		void draw_string(unsigned int x_pos, unsigned int y_pos, unsigned char *string);
};




#endif
