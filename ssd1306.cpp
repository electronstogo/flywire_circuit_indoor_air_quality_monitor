// 2020 - electronstogo

#include <Wire.h>
#include "ssd1306.h"
#include <ctype.h>




// oled pixel buffer
unsigned char oled_buffer[8][OLED_WIDTH] = {0x0};



// init sequence for the SSD1306 controller.
const unsigned char SSD1306_INIT_SEQUENCE[] = 
{  
	// deactivate screen
	0xAE,
	// set page address mode.
	0x20, 0x02,
	// set multiplex ratio 63
	0xA8, 0x3F,
	// set display offset 0
	0xD3, 0x0, 
	// set display start line.
	0x40,
	// set segment re-map
	0xA1,
	// set com output scan mode direction
	0xC0,
	// set com pins config
	0xDA, 0x12,
	// set contrast to max.
	0x81, 0xFF,
	// activate data from ram.
	0xA4,
	// set normal display mode.
	0xA6,
	// set display oscillator frquency
	0xD5, 0xF0,
	// activate internal charge pump
	0x8D, 0x14,
	// deactivate scroll mode.        
	0x2E,
	// activate display
	0xAF,
};




const unsigned char CHAR_WIDTH_DATA[]=
{
	2, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};



const unsigned char FONT_TABLE[] PROGMEM = 
{
	0x3E, 0x00, //   #####  
	0x63, 0x00, //  ##   ## 
	0x61, 0x00, //  ##    # 
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0x43, 0x00, //  #    ## 
	0x63, 0x00, //  ##   ## 
	0x3E, 0x00, //   #####  

	0x18, //    ##   
	0x78, //  ####   
	0xD8, // ## ##   
	0x98, // #  ##   
	0x18, //    ##   
	0x18, //    ##   
	0x18, //    ##   
	0x18, //    ##   
	0x18, //    ##   
	0x18, //    ##   
	0x18, //    ##   
	0x18, //    ##   
	0xFF, // ########

	0x78, //  ####   
	0x8E, // #   ### 
	0x06, //      ## 
	0x06, //      ## 
	0x06, //      ## 
	0x06, //      ## 
	0x0C, //     ##  
	0x0C, //     ##  
	0x18, //    ##   
	0x30, //   ##    
	0x60, //  ##     
	0xC0, // ##      
	0xFF, // ########

	0x7C, //  #####  
	0x8E, // #   ### 
	0x06, //      ## 
	0x06, //      ## 
	0x06, //      ## 
	0x0C, //     ##  
	0x78, //  ####   
	0x06, //      ## 
	0x03, //       ##
	0x03, //       ##
	0x03, //       ##
	0x86, // #    ## 
	0x7C, //  #####  

	0x07, 0x00, //      ###  
	0x0F, 0x00, //     ####  
	0x0B, 0x00, //     # ##  
	0x1B, 0x00, //    ## ##  
	0x13, 0x00, //    #  ##  
	0x33, 0x00, //   ##  ##  
	0x63, 0x00, //  ##   ##  
	0x43, 0x00, //  #    ##  
	0xC3, 0x00, // ##    ##  
	0xFF, 0xC0, // ##########
	0x03, 0x00, //       ##  
	0x03, 0x00, //       ##  
	0x03, 0x00, //       ##  

	0x7E, //  ###### 
	0x60, //  ##     
	0x60, //  ##     
	0x60, //  ##     
	0x60, //  ##     
	0x7C, //  #####  
	0x06, //      ## 
	0x03, //       ##
	0x03, //       ##
	0x03, //       ##
	0x03, //       ##
	0x86, // #    ## 
	0x7C, //  #####  

	0x1F, 0x00, //    ##### 
	0x30, 0x00, //   ##     
	0x60, 0x00, //  ##      
	0xC0, 0x00, // ##       
	0xC0, 0x00, // ##       
	0xDE, 0x00, // ## ####  
	0xE3, 0x00, // ###   ## 
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0x63, 0x00, //  ##   ## 
	0x3E, 0x00, //   #####  

	0xFF, 0x80, // #########
	0x01, 0x80, //        ##
	0x03, 0x00, //       ## 
	0x03, 0x00, //       ## 
	0x06, 0x00, //      ##  
	0x06, 0x00, //      ##  
	0x04, 0x00, //      #   
	0x0C, 0x00, //     ##   
	0x0C, 0x00, //     ##   
	0x18, 0x00, //    ##    
	0x18, 0x00, //    ##    
	0x30, 0x00, //   ##     
	0x30, 0x00, //   ##     

	0x3E, 0x00, //   #####  
	0x63, 0x80, //  ##   ###
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xE3, 0x00, // ###   ## 
	0x76, 0x00, //  ### ##  
	0x1E, 0x00, //    ####  
	0x77, 0x00, //  ### ### 
	0xC3, 0x80, // ##    ###
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xE3, 0x00, // ###   ## 
	0x3E, 0x00, //   #####  

	0x3E, 0x00, //   #####  
	0x63, 0x00, //  ##   ## 
	0xC1, 0x00, // ##     # 
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0x63, 0x80, //  ##   ###
	0x3D, 0x80, //   #### ##
	0x01, 0x80, //        ##
	0x01, 0x00, //        # 
	0x03, 0x00, //       ## 
	0x06, 0x00, //      ##  
	0xFC, 0x00, // ######   

	
	0x70, 0x30, //  ###      ## 
	0xD8, 0x20, // ## ##     #  
	0x88, 0x40, // #   #    #   
	0x88, 0xC0, // #   #   ##   
	0x89, 0x80, // #   #  ##    
	0xDB, 0x00, // ## ## ##     
	0x72, 0x70, //  ###  #  ### 
	0x06, 0xD8, //      ## ## ##
	0x0C, 0x88, //     ##  #   #
	0x18, 0x88, //    ##   #   #
	0x10, 0x88, //    #    #   #
	0x20, 0xD8, //   #     ## ##
	0x60, 0x70, //  ##      ### 
 
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0xE0, // ###  
	0xE0, // ###
	0xE0, // ###
    
	0x00, //   
	0x00, // 
	0xC0, // ##
	0xC0, // ##   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0x00, //   
	0xC0, // ##
	0xC0, // ##
	0x00, //  
	
	0x00, //
	0x60, //  ## 
	0x90, // #  #
	0x90, // #  #
	0x90, // #  #
	0x60, //  ## 
	0x00, //     
	0x00, //     
	0x00, //     
	0x00, //     
	0x00, //     
	0x00, //     
	0x00, //     
	0x00, //         

	0x06, 0x00, //      ##     
	0x0F, 0x00, //     ####    
	0x0F, 0x00, //     ####    
	0x09, 0x00, //     #  #    
	0x19, 0x80, //    ##  ##   
	0x19, 0x80, //    ##  ##   
	0x10, 0xC0, //    #    ##  
	0x30, 0xC0, //   ##    ##  
	0x30, 0xC0, //   ##    ##  
	0x3F, 0xE0, //   ######### 
	0x60, 0x60, //  ##      ## 
	0x60, 0x60, //  ##      ## 
	0xC0, 0x30, // ##        ##

	0xFC, 0x00, // ######   
	0xC6, 0x00, // ##   ##  
	0xC3, 0x00, // ##    ## 
	0xC3, 0x00, // ##    ## 
	0xC3, 0x00, // ##    ## 
	0xC6, 0x00, // ##   ##  
	0xFE, 0x00, // #######  
	0xC3, 0x00, // ##    ## 
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0xC3, 0x00, // ##    ## 
	0xFE, 0x00, // #######  

	0x1F, 0x00, //    ##### 
	0x30, 0x80, //   ##    #
	0x60, 0x00, //  ##      
	0x40, 0x00, //  #       
	0xC0, 0x00, // ##       
	0xC0, 0x00, // ##       
	0xC0, 0x00, // ##       
	0xC0, 0x00, // ##       
	0xC0, 0x00, // ##       
	0xC0, 0x00, // ##       
	0x60, 0x00, //  ##      
	0x30, 0x80, //   ##    #
	0x1F, 0x00, //    ##### 

	0xFE, 0x00, // #######   
	0xC3, 0x00, // ##    ##  
	0xC1, 0x80, // ##     ## 
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0x80, // ##      # 
	0xC1, 0x80, // ##     ## 
	0xC3, 0x00, // ##    ##  
	0xFC, 0x00, // ######    

	0xFE, // #######
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xFC, // ###### 
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xFE, // #######

	0xFE, // #######
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xFE, // #######
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     

	0x0F, 0xC0, //     ###### 
	0x38, 0x60, //   ###    ##
	0x60, 0x00, //  ##        
	0x60, 0x00, //  ##        
	0xC0, 0x00, // ##         
	0xC0, 0x00, // ##         
	0xC3, 0xE0, // ##    #####
	0xC0, 0x60, // ##       ##
	0xC0, 0x60, // ##       ##
	0x60, 0x60, //  ##      ##
	0x60, 0x60, //  ##      ##
	0x30, 0x60, //   ##     ##
	0x0F, 0xC0, //     ###### 

	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xFF, 0xC0, // ##########
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##

	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##

	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x98, // #  ##
	0x70, //  ### 

	0xC1, 0x80, // ##     ##
	0xC3, 0x00, // ##    ## 
	0xC6, 0x00, // ##   ##  
	0xCC, 0x00, // ##  ##   
	0xD8, 0x00, // ## ##    
	0xF0, 0x00, // ####     
	0xF0, 0x00, // ####     
	0xD8, 0x00, // ## ##    
	0xCC, 0x00, // ##  ##   
	0xCE, 0x00, // ##  ###  
	0xC6, 0x00, // ##   ##  
	0xC3, 0x00, // ##    ## 
	0xC1, 0x80, // ##     ##

	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xC0, // ##     
	0xFF, // ########

	0xE0, 0x1C, // ###        ###
	0xF0, 0x3C, // ####      ####
	0xF0, 0x3C, // ####      ####
	0xD8, 0x2C, // ## ##     # ##
	0xD8, 0x6C, // ## ##    ## ##
	0xD8, 0x4C, // ## ##    #  ##
	0xCC, 0xCC, // ##  ##  ##  ##
	0xCC, 0xCC, // ##  ##  ##  ##
	0xC4, 0x8C, // ##   #  #   ##
	0xC7, 0x8C, // ##   ####   ##
	0xC7, 0x8C, // ##   ####   ##
	0xC3, 0x0C, // ##    ##    ##
	0xC3, 0x0C, // ##    ##    ##

	0xE0, 0xC0, // ###     ##
	0xF0, 0xC0, // ####    ##
	0xF0, 0xC0, // ####    ##
	0xD8, 0xC0, // ## ##   ##
	0xD8, 0xC0, // ## ##   ##
	0xCC, 0xC0, // ##  ##  ##
	0xCC, 0xC0, // ##  ##  ##
	0xC6, 0xC0, // ##   ## ##
	0xC6, 0xC0, // ##   ## ##
	0xC3, 0xC0, // ##    ####
	0xC3, 0xC0, // ##    ####
	0xC1, 0xC0, // ##     ###
	0xC1, 0xC0, // ##     ###

	0x1F, 0x80, //    ######   
	0x30, 0xC0, //   ##    ##  
	0x60, 0x60, //  ##      ## 
	0x40, 0x30, //  #        ##
	0xC0, 0x30, // ##        ##
	0xC0, 0x30, // ##        ##
	0xC0, 0x30, // ##        ##
	0xC0, 0x30, // ##        ##
	0xC0, 0x30, // ##        ##
	0xC0, 0x20, // ##        # 
	0x60, 0x60, //  ##      ## 
	0x30, 0xC0, //   ##    ##  
	0x1F, 0x80, //    ######   

	0xFC, // ######  
	0xC6, // ##   ## 
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC6, // ##   ## 
	0xFC, // ######  
	0xC0, // ##      
	0xC0, // ##      
	0xC0, // ##      
	0xC0, // ##      
	0xC0, // ##            

	0x1F, 0x80, //    ######     
	0x30, 0xC0, //   ##    ##    
	0x60, 0x60, //  ##      ##   
	0x40, 0x30, //  #        ##  
	0xC0, 0x30, // ##        ##  
	0xC0, 0x30, // ##        ##  
	0xC0, 0x30, // ##        ##  
	0xC0, 0x30, // ##        ##  
	0xC0, 0x30, // ##        ##  
	0xC0, 0x20, // ##        #   
	0x60, 0x60, //  ##      ##   
	0x30, 0xC0, //   ##    ##    
	0x1F, 0xF0, //    #########  

	0xFC, 0x00, // ######   
	0xC6, 0x00, // ##   ##  
	0xC3, 0x00, // ##    ## 
	0xC3, 0x00, // ##    ## 
	0xC3, 0x00, // ##    ## 
	0xC6, 0x00, // ##   ##  
	0xFC, 0x00, // ######   
	0xC6, 0x00, // ##   ##  
	0xC6, 0x00, // ##   ##  
	0xC3, 0x00, // ##    ## 
	0xC3, 0x00, // ##    ## 
	0xC1, 0x00, // ##     # 
	0xC1, 0x80, // ##     ##

	0x3C, //   ####  
	0x62, //  ##   # 
	0xC0, // ##      
	0xC0, // ##      
	0xE0, // ###     
	0x70, //  ###    
	0x3C, //   ####  
	0x0E, //     ### 
	0x03, //       ##
	0x03, //       ##
	0x03, //       ##
	0x86, // #    ## 
	0x7C, //  #####  

	0xFF, 0xC0, // ##########
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    

	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0xC0, 0xC0, // ##      ##
	0x61, 0x80, //  ##    ## 
	0x3E, 0x00, //   #####   

	0xC0, 0x20, // ##        #
	0x60, 0x60, //  ##      ##
	0x60, 0x60, //  ##      ##
	0x60, 0x40, //  ##      # 
	0x30, 0xC0, //   ##    ## 
	0x30, 0xC0, //   ##    ## 
	0x10, 0x80, //    #    #  
	0x19, 0x80, //    ##  ##  
	0x19, 0x80, //    ##  ##  
	0x09, 0x00, //     #  #   
	0x0F, 0x00, //     ####   
	0x0E, 0x00, //     ###    
	0x06, 0x00, //      ##    

	0xC1, 0x81, // ##     ##      #
	0xC1, 0xC1, // ##     ###     #
	0xC1, 0xC1, // ##     ###     # 
	0x43, 0xC3, //  #    ####    ## 
	0x63, 0x43, //  ##   ## #    ## 
	0x63, 0x63, //  ##   ## ##   ## 
	0x62, 0x62, //  ##   #  ##   #  
	0x26, 0x26, //   #  ##   #  ##  
	0x36, 0x36, //   ## ##   ## ##  
	0x34, 0x34, //   ## #    ## #   
	0x14, 0x34, //    # #    ## #   
	0x1C, 0x1C, //    ###     ###   
	0x1C, 0x18, //    ###     ##    

	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##
	0x63, 0x00, //  ##   ## 
	0x36, 0x00, //   ## ##  
	0x36, 0x00, //   ## ##  
	0x1C, 0x00, //    ###   
	0x1C, 0x00, //    ###   
	0x1C, 0x00, //    ###   
	0x36, 0x00, //   ## ##  
	0x63, 0x00, //  ##   ## 
	0x63, 0x00, //  ##   ## 
	0xC1, 0x80, // ##     ##
	0xC1, 0x80, // ##     ##

	0xC0, 0xC0, // ##      ##
	0x61, 0x80, //  ##    ## 
	0x61, 0x80, //  ##    ## 
	0x33, 0x00, //   ##  ##  
	0x33, 0x00, //   ##  ##  
	0x1E, 0x00, //    ####   
	0x1E, 0x00, //    ####   
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    
	0x0C, 0x00, //     ##    

	0xFF, // ########
	0x03, //       ##
	0x06, //      ## 
	0x06, //      ## 
	0x0C, //     ##  
	0x18, //    ##   
	0x18, //    ##   
	0x30, //   ##    
	0x30, //   ##    
	0x60, //  ##     
	0xC0, // ##      
	0xC0, // ##      
	0xFF, // ########  
};






SSD1306::SSD1306(unsigned char ssd1306_i2c_address)
{
	this->i2c_address = ssd1306_i2c_address;

	// send init sequence to the SSD1306 controller. 
	for(int i = 0; i < sizeof(SSD1306_INIT_SEQUENCE); i++)
	{
		write_command(SSD1306_INIT_SEQUENCE[i]);
	}
}


void SSD1306::write_command(unsigned char command)
{
	Wire.beginTransmission(this->i2c_address);
	Wire.write(0x0);
	Wire.write(command);
	Wire.endTransmission();
}


void SSD1306::write_data(unsigned char *data, unsigned data_length)
{	
	for(int i = 0; i < data_length; i++)
	{
		Wire.beginTransmission(this->i2c_address);
		Wire.write(0x40);	
		Wire.write(*data);
		Wire.endTransmission();
		data++;
	}
}

void SSD1306::set_page(unsigned int page)
{
	write_command(0xb0 + page);
}


void SSD1306::clear_oled_buffer()
{
	memset(oled_buffer, 0, 8 * OLED_WIDTH);
}


void SSD1306::flush_oled_buffer()
{
	for(int i = 0; i < 8; i++)
	{
		this->set_page(i);
		this->write_data(oled_buffer[i], OLED_WIDTH);
	}
}


void SSD1306::draw_point(unsigned char x, unsigned char y)
{
	// check if point is in valid area
	if(y > OLED_HEIGHT || x > OLED_WIDTH)
	{
		return;
	}

	oled_buffer[y / 8][x] |= (0x1 << (y % 8));
}




void SSD1306::draw_letter(unsigned int x_pos, unsigned int y_pos, unsigned int char_index)
{
	unsigned char byte_buffer;
	
	// height of current char in bits.
	unsigned char height = 13;
	
	// width of the current char in byte (varies for every char).
	unsigned char width = CHAR_WIDTH_DATA[char_index];
	
	// table byte offset, where we can find the current char.
	unsigned int char_table_offset = 0;
	
	
	// caculate the offset of char data in the table.
	for(int i = 0; i < char_index; i++)
	{
		char_table_offset += CHAR_WIDTH_DATA[i] * height;
	}
	
	
	// loop through all bytes, and set the pixels in the OLED display buffer.
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			byte_buffer = pgm_read_byte_near(FONT_TABLE + char_table_offset + i * width + j);
			
			for(int bit_pos = 0; bit_pos < 8; bit_pos++)
			{
				if(byte_buffer & (0x1 << (7 - bit_pos)))
				{
					this->draw_point(x_pos + bit_pos + j * 8, y_pos - i);
				}
			}
		}
	}
}




void SSD1306::draw_string(unsigned int x_pos, unsigned int y_pos, unsigned char *string)
{
	unsigned int letter_index;
	unsigned int while_loop_counter = 0;
	unsigned int x_pos_new;
	

	while(*string)
	{
		// get the letter index in the font table.
		if(isdigit(*string))
		{
			letter_index = *string - '0';
			x_pos_new = x_pos + 12;
		}
		else if(isupper(*string))
		{
			letter_index = (unsigned int)(*string - 'A' + 14);
			x_pos_new = x_pos + 13;
			
			if(*string == 'M')
			{
				x_pos_new += 4;
			}
			else if(*string == 'B')
			{
				x_pos_new -= 2;
			}
		}
		else if(*string == ' ')
		{
			x_pos = x_pos + 5;
			string++;
			continue;	
		}
		else if(*string == '%')
		{
			letter_index = 10;
			x_pos_new = x_pos + 18;
		}
		else if(*string == '.')
		{
			letter_index = 11;
			x_pos_new = x_pos + 6;
		}
		else if(*string == ':')
		{
			letter_index = 12;
			x_pos_new = x_pos + 6;
		}
		else if(*string == '$') // '°'
		{
			letter_index = 13;
			x_pos_new = x_pos + 7;
		}
		else
		{
			// couldnt find letter index in the font table.
			string++;
			continue;
		}	

		this->draw_letter(x_pos, y_pos, letter_index);
		x_pos = x_pos_new;
	
	
		string++;
		while_loop_counter++;
		
		// watchdog that breaks in case of too long strings.
		// strings can become very long, when the '\0' char at the end is missing.
		if(while_loop_counter > 10)break;
	}



}
