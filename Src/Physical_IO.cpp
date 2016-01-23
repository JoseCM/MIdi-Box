#include "Physical_IO.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "gpio.h"

#include <cstring>
#include <string>

#define CS_PIN (27)
#define CS_HIGH() (gpio_set_pin(CS_PIN, HIGH))
#define CS_LOW() (gpio_set_pin(CS_PIN, LOW))


Physical_IO::Physical_IO(string device_i2c, string device_spi): MIDI_IO()
{
	fd_i2c = fd_spi = -1;
	this->device_i2c = device_i2c;
	this->device_spi = device_spi;

	
	//set buttons mapping
	buttonMap.assign(16, MidiMessage() );
	for(int i=0; i < 16; i++)
		buttonMap[i].setCommand(0x90, i+48, 127);	

	//set encoder mapping
	encoderMap.assign(4, MidiMessage() );
	for(int i = 0; i < 4; i++)
		encoderMap[i].setCommand(0xA0, i+1, 0);

	for(int i = 0; i < 4; i++)
		encoder_value[i] = 63.5;

}


Physical_IO::~Physical_IO()
{
	close();
}


bool Physical_IO::open()
{

	struct spi_ioc_transfer spi;
	char spi_out[10];

	//open i2c device
	if(Init_TrellisI2C(&fd_i2c, device_i2c.c_str(), 0x70) < 0)
		return false;

	//open spi device
	uint8_t spi_mode = 0 | SPI_NO_CS;
	uint8_t spi_bpw = 8;
	uint32_t spi_speed = 10000000; //MHz

	gpio_init_pin(CS_PIN, OUT);
	CS_HIGH();

	if((fd_spi= ::open(device_spi.c_str(), O_RDWR)) < 0){
		perror("Failed to open SPI: ");
		return false;
	} 

	if(ioctl(fd_spi, SPI_IOC_WR_MODE, &spi_mode) < 0) { perror("Writing mode: "); return false;}	
	if(ioctl(fd_spi, SPI_IOC_WR_BITS_PER_WORD, &spi_bpw) < 0) { perror("Writing bpw: "); return false;}
	if(ioctl(fd_spi, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0) { perror("Writing speed: "); return false;}

	if(ioctl(fd_spi, SPI_IOC_RD_MODE, &spi_mode) < 0) { perror("Writing mode: "); return false;}	
	if(ioctl(fd_spi, SPI_IOC_RD_BITS_PER_WORD, &spi_bpw) < 0) { ("Writing bpw: "); return false;}
	if(ioctl(fd_spi, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed) < 0) { perror("Writing speed: "); return false;}

	spi_out[0] = 0x40; spi_out[1] = 0x02; spi_out[2] = 0xff;
	CS_LOW();
	write(fd_spi, spi_out, 3);
	CS_HIGH();

	spi_out[0] = 0x40; spi_out[1] = 0x06; spi_out[2] = 0xff;
	CS_LOW();
	write(fd_spi, spi_out, 3);
	CS_HIGH();
	
	spi_out[0] = 0x40; spi_out[1] = 0x05; spi_out[2] = 0x20;
	CS_LOW();
	write(fd_spi, spi_out, 3);
	CS_HIGH();		

	spi_out[0] = 0x41; spi_out[1] = 0x09;
	CS_LOW();
	write(fd_spi, spi_out, 2);

	return true;
}


bool Physical_IO::close()
{
	if(::close(fd_i2c) < 0)
		return false;

	if(::close(fd_spi) < 0)
		return false;

	return true;
}


bool Physical_IO::addMidiMsgtoButton(int button , MidiMessage &msg)
{
	if(button < 0 || button > 15)
		return false;

	buttonMap[button] = msg;

	return true;

}


bool Physical_IO::addMidiMsgtoEncoder(int encoder, MidiMessage &msg)
{
	if(encoder < 0 || encoder > 3)
		return false;

	encoderMap[encoder] = msg;

	return true;

}

MidiMessage& Physical_IO::buttonToMidiMsg(int button, uint8_t noteonff)
{

	MidiMessage &msg = buttonMap[button];

	msg[0] = noteonff ? msg[0] |= 0x90 : msg[0] |= 0x80; 

	return msg;
}

MidiMessage& Physical_IO::encoderToMidiMsg(int encoder)
{
	MidiMessage &msg = encoderMap[encoder];

	msg[2] = (uint8_t) encoder_value[encoder];

	return msg;
}

uint8_t Physical_IO::updateEncoderState(){

		static uint8_t encoder_state = 0xff, prev_encoder_state = 0xff, temp_prev, temp_new;

		uint8_t status = 0;

		if(read(fd_spi, &encoder_state, 1) != 1)
			printf("Not reading...\n");

		for(int i = 0; i < 8; i+=2){

			temp_prev = (prev_encoder_state >> i) & 0x03;
			temp_new = (encoder_state >> i) & 0x03;

			if(temp_prev != temp_new){

				if(temp_prev == 0x00 && temp_new == 0x01){
					encoder_value[i/2] += 5.3;
					status |= ( 1 << i/2);
				} else if (temp_prev == 0x01 && temp_new == 0x00){
					encoder_value[i/2] -= 5.3;		
					status |= ( 1 << i/2);
				} 

				if(encoder_value[i/2] < 0.0) {
					encoder_value[i/2] = 0.0;
				} else if (encoder_value[i/2] > 127.0){
					encoder_value[i/2] = 127.0;
				}

			}			

		}

		prev_encoder_state = encoder_state;

		return status;

}


uint8_t Physical_IO::updateButtonState(){

	return readSwitches(&fd_i2c);

}