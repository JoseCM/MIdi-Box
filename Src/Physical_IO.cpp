#include "Physical_IO.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>



#include "gpio.h"
#include "MidiMessage.h"

#include <cstring>
#include <string>


/****************************************************
* SPI - I/O Expander - Encoders
****************************************************/
#define CS_PIN (27)
#define CS_HIGH() (gpio_set_pin(CS_PIN, HIGH))
#define CS_LOW() (gpio_set_pin(CS_PIN, LOW))


/***************************************************
* I2c - Trellis Adafruit - Buttons
****************************************************/
// I2C definitions
#define I2C_SLAVE 0x0703
#define I2C_SMBUS 0x0720  /* SMBus-level access */

#define I2C_SMBUS_READ  1
#define I2C_SMBUS_WRITE 0

// SMBus transaction types
#define I2C_SMBUS_QUICK       0
#define I2C_SMBUS_BYTE        1
#define I2C_SMBUS_BYTE_DATA     2
#define I2C_SMBUS_WORD_DATA     3
#define I2C_SMBUS_PROC_CALL     4
#define I2C_SMBUS_BLOCK_DATA      5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7   /* SMBus 2.0 */
#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages
#define I2C_SMBUS_BLOCK_MAX 32  /* As specified in SMBus standard */
#define I2C_SMBUS_I2C_BLOCK_MAX 32  /* Not specified but we use same structure */

//TRELIS - HT16K33 - ADAFRUIT
#define HT16K33_BLINK_CMD       0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_CMD_BRIGHTNESS  0xE0

#define LED_ON  1
#define LED_OFF 0

#define HT16K33_BLINK_OFF    0
#define HT16K33_BLINK_2HZ    1
#define HT16K33_BLINK_1HZ    2
#define HT16K33_BLINK_HALFHZ 3

#define numKeys 16

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

//I2C Driver Rpi
union i2c_smbus_data
{
  uint8_t  byte ;
  uint16_t word ;
  uint8_t  block [I2C_SMBUS_BLOCK_MAX + 2] ;  // block [0] is used for length + one more for PEC
} ;

struct i2c_smbus_ioctl_data
{
  char read_write ;
  uint8_t command ;
  int size ;
  union i2c_smbus_data *data ;
} ;

static inline int i2c_smbus_access (int fd, char rw, uint8_t command, int size, union i2c_smbus_data *data)
{
  struct i2c_smbus_ioctl_data args ;

  args.read_write = rw ;
  args.command    = command ;
  args.size       = size ;
  args.data       = data ;
  return ioctl (fd, I2C_SMBUS, &args) ;
}

static int write_data(int fd, uint8_t data)
{
   return i2c_smbus_access (fd, I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL) ;
}

static int read_data(int fd, uint8_t *data)
{
  union i2c_smbus_data dt;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE_DATA, &dt))
    return -1 ;

  *data = dt.byte & 0xFF;
  return 1 ;

}

static int write_Reg8 (int fd, int reg, int value)
{
  union i2c_smbus_data data ;

  data.byte = value ;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data) ;
}

int read_reg8 (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
    return -1 ;
  else
    return data.byte & 0xFF;
}

static int write_block_data(int file, uint8_t command,
                                             uint8_t *values, uint8_t length)
{
        union i2c_smbus_data data;
        int i;
        if (length > 32)
                length = 32;
        for (i = 1; i <= length; i++)
                data.block[i] = values[i-1];
        data.block[0] = length;
        return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
                                I2C_SMBUS_BLOCK_DATA, &data);
}

static const uint8_t
  ledLUT[16] =
    { 0x3A, 0x37, 0x35, 0x34,
      0x28, 0x29, 0x23, 0x24,
      0x16, 0x1B, 0x11, 0x10,
      0x0E, 0x0D, 0x0C, 0x02 },
  buttonLUT[16] =
    { 0x07, 0x04, 0x02, 0x22,
      0x05, 0x06, 0x00, 0x01,
      0x03, 0x10, 0x30, 0x21,
      0x13, 0x12, 0x11, 0x31 };

static  uint16_t displaybuffer[8] = {0};
static  uint8_t keys[6] = {0} , lastkeys[6] = {0};


bool setBrightness(int *fd, uint8_t b) {
  if (b > 15) b = 15;
  if(write_data(*fd, HT16K33_CMD_BRIGHTNESS | b) < 0) {
      perror("Writing Britness: ");
      return false;
  }
  return true;
}

bool blinkRate(int *fd, uint8_t b) {
  if (b > 3) b = 0; // turn off if not sure
  if(write_data(*fd, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1)) < 0) {
      perror("Writing blinkRate: ");
      return false;
  }
  return true;
}

bool isKeyPressed(uint8_t k) {
  if (k > 15) return false;
  k = buttonLUT[k];
  return (keys[k>>4] & (1 <<(k & 0x0F)));
}

bool wasKeyPressed(uint8_t k) {
  if (k > 15) return false;
  k = buttonLUT[k];
  return (lastkeys[k>>4] & (1 <<(k & 0x0F)));
}

bool justPressed(uint8_t k) {
  return (isKeyPressed(k) & !wasKeyPressed(k));
}

bool justReleased(uint8_t k) {
  return (!isKeyPressed(k) & wasKeyPressed(k));
}

bool isLED(uint8_t x) {
  if (x > 15) return false;
  x = ledLUT[x];
  return ( (displaybuffer[x >> 4] & (1<<(x & 0x0F)) ) > 0 );
}

void setLED(uint8_t x) {
  if (x > 15) return;
  x = ledLUT[x];
  displaybuffer[x >> 4] |=  ( 1<< (x & 0x0F));
}

void clrLED(uint8_t x) {
  if (x > 15) return;
  x = ledLUT[x];
  displaybuffer[x >> 4] &= ~ (1<<(x & 0x0F));
}

bool readSwitches(int *fd) {
  memcpy(lastkeys, keys, sizeof(keys));

  uint8_t reg = 0x40;
  for (uint8_t i = 0; i < 6; i++)
  {
    keys[i] = read_reg8(*fd, reg);
    reg++;
  }

  for (uint8_t i=0; i<6; i++) {
    if (lastkeys[i] != keys[i]) {
      return true;
    }
  }
  return false;
}


bool writeDisplay(int *fd) {

  uint8_t toDisplay[16] = {0};
  uint8_t index = 0;

  for (uint8_t i = 0; i < 8; i++)
  {
      toDisplay[index++] = displaybuffer[i] & 0xFF;
      toDisplay[index] = displaybuffer[i] >> 8;
  }

  if ( write_block_data(*fd, 0x00, toDisplay, 16) < 0){
    perror("Writing display: ");
    return false;
  }
  return true;
}

void clear(void) {
  memset(displaybuffer, 0, sizeof(displaybuffer));
}



/***********************************************************************
 * Class Physical IO Implementation
***********************************************************************/
Physical_IO::Physical_IO(string device_i2c, string device_spi): MIDI_IO()
{
	fd_i2c = fd_spi = -1;
	this->device_i2c = device_i2c;
	this->device_spi = device_spi;


	//set buttons mapping
	buttonMap.assign(16, MidiMessage() );

    buttonMap[0].setCommand(0x90, 48+12, 127);
    buttonMap[1].setCommand(0x90, 48+8, 127);
    buttonMap[2].setCommand(0x90, 48+4, 127);
    buttonMap[3].setCommand(0x90, 48+0, 127);
    buttonMap[4].setCommand(0x90, 48+13, 127);
    buttonMap[5].setCommand(0x90, 48+9, 127);
    buttonMap[6].setCommand(0x90, 48+5, 127);
    buttonMap[7].setCommand(0x90, 48+1, 127);
    buttonMap[8].setCommand(0x90, 48+14, 127);
    buttonMap[9].setCommand(0x90, 48+10, 127);
    buttonMap[10].setCommand(0x90, 48+6, 127);
    buttonMap[11].setCommand(0x90, 48+2, 127);
    buttonMap[12].setCommand(0x90, 48+15, 127);
    buttonMap[13].setCommand(0x90, 48+11, 127);
    buttonMap[14].setCommand(0x90, 48+7, 127);
    buttonMap[15].setCommand(0x90, 48+3, 127);



	//set encoder mapping
	encoderMap.assign(4, MidiMessage() );

    encoderMap[0].setCommand(0xB0, 44, 0);
    encoderMap[1].setCommand(0xB0, 46, 0);
    encoderMap[2].setCommand(0xB0, 48, 0);
    encoderMap[3].setCommand(0xB0, 53, 0);

	for(int i = 0; i < 4; i++)
		encoder_value[i] = 63.5;

    open();
}


Physical_IO::~Physical_IO()
{
	close();
}


bool Physical_IO::open()
{

	char spi_out[10];

    /******************
     * Open I2C device
    ******************/
    if ((fd_i2c = ::open(device_i2c.c_str(), O_RDWR)) < 0) {
        perror("Unable to open I2C device: ");
        return false;
    }

    if (ioctl (fd_i2c, I2C_SLAVE, 0x70) < 0) {perror("Unable to select I2C device: "); return false;}

    // turn on oscillator
    if(write_data(fd_i2c, 0x21) < 0) { perror("Writing oscillator: "); return false;}

    //turn off blink rate
    if(!blinkRate(&fd_i2c, HT16K33_BLINK_OFF)) return false;

    // set brightness to maximum
    if(!setBrightness(&fd_i2c, 15)) return false;

    // turn on interrupt, active low
    if(write_data(fd_i2c, 0xA1) < 0) { perror("Writing interrupt: "); return false;}

    /******************
     * Open SPI device
    ******************/
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

void Physical_IO::upOctave(){

    if(buttonMap[3].getP1() + 12 > 108)
        return;

    for(int i=0; i < 16; i++)
        buttonMap[i].setP1(buttonMap[i].getP1() + 12);

}

void Physical_IO::downOctave(){
    if(buttonMap[3].getP1() - 12 < 0)
        return;

    for(int i=0; i < 16; i++)
        buttonMap[i].setP1(buttonMap[i].getP1() - 12);

}

uint8_t Physical_IO::updateEncoderState(){

		static uint8_t encoder_state = 0xff, prev_encoder_state = 0xff, temp_prev, temp_new;

		uint8_t status = 0;

        if(read(fd_spi, &encoder_state, 1) != 1);
            //printf("Not reading...\n");

		for(int i = 0; i < 8; i+=2){

			temp_prev = (prev_encoder_state >> i) & 0x03;
			temp_new = (encoder_state >> i) & 0x03;

			if(temp_prev != temp_new){

				if(temp_prev == 0x00 && temp_new == 0x02){
					encoder_value[i/2] -= 5.3;
					status |= ( 1 << (i/2));
				} else if (temp_prev == 0x02 && temp_new == 0x00){
					encoder_value[i/2] += 5.3;
					status |= ( 1 << (i/2));
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

void* Physical_IO::Thread_InEncoder(void* arg)
{
    Physical_IO *phys_io = static_cast<Physical_IO*>(arg);
	uint8_t encoderchanged = 0;
	MidiMessage msg;

	while(1){

		encoderchanged = phys_io->updateEncoderState();

		if(!encoderchanged)
			continue;

		for(int i = 0; i < 4; i++){
			if(encoderchanged & (1 << i)){
				msg = phys_io->encoderToMidiMsg(i);
				phys_io->writeInMidiMsg(0, msg);
			}
		}
	}

}

void* Physical_IO::Thread_InButton(void* arg)
{
    Physical_IO *phys_io = static_cast<Physical_IO*>(arg);
	MidiMessage msg;

	while(1){

		usleep(30000);

		if (phys_io->updateButtonState()){

			for (uint8_t i=0; i<16; i++) {
	        	// if it was pressed...
	      		if (justPressed(i)){
                    printf("pressed %d\n", i);
					msg = phys_io->buttonToMidiMsg(i, NOTE_ON);
					phys_io->writeInMidiMsg(0, msg);
				}

				// if it was released, turn it off
				if (justReleased(i)){
                    printf("released %d\n", i);
					msg = phys_io->buttonToMidiMsg(i, NOTE_ON);
					msg[2] = 0;
					phys_io->writeInMidiMsg(0, msg);
				}
	 		}
	    }

	}
}

void Physical_IO::run()
{
    struct sched_param pthread_param;
    pthread_attr_t thread_attr;

    pthread_param.sched_priority = 2;

    pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedparam(&thread_attr, &pthread_param);

    //pthread_create(&handle1, &thread_attr, Physical_IO::Thread_InEncoder, static_cast<void*>(this));
    //pthread_create(&handle2, &thread_attr, Physical_IO::Thread_InButton, static_cast<void*>(this));

    pthread_create(&handle1, NULL, Physical_IO::Thread_InEncoder, static_cast<void*>(this));
    pthread_create(&handle2, NULL, Physical_IO::Thread_InButton, static_cast<void*>(this));

}



