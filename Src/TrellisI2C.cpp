#include "TrellisI2C.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/**************************************************************
I2C Driver Rpi
***************************************************************/
// Structures used in the ioctl() calls
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

static int read_block_data(int file, uint8_t command, 
                                              uint8_t *values, uint8_t length)
{
        union i2c_smbus_data data;
        int i;

        data.block[0] = length;   //linha acrecentada

        if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
                             I2C_SMBUS_BLOCK_DATA,&data))
                return -1;
        else {
                for (i = 1; i <= data.block[0]; i++)
                        values[i-1] = data.block[i];
                return data.block[0];
        }
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

/**************************************************************
TRELIS - HT16K33 - ADAFRUIT
**************************************************************/
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


int Init_TrellisI2C(int *fd, const char *device, int devId)
{
	if ((*fd = open (device, O_RDWR)) < 0)
    	perror("Unable to open I2C device: "); 

  	if (ioctl (*fd, I2C_SLAVE, devId) < 0)
    	perror("Unable to select I2C device: "); 

      // turn on oscillator
  	if(write_data(*fd, 0x21) < 0) 
    	perror("Writing oscillator: "); 

  	blinkRate(fd, HT16K33_BLINK_OFF);

  	// max brightness
  	setBrightness(fd, 15);

  	// turn on interrupt, active low
  	if(write_data(*fd, 0xA1) < 0) 
   		perror("Writing interrupt: ");

   	return *fd;
}


void setBrightness(int *fd, uint8_t b) {
  if (b > 15) b = 15;

  if(write_data(*fd, HT16K33_CMD_BRIGHTNESS | b) < 0) 
    perror("Writing Britness: ");
}

void blinkRate(int *fd, uint8_t b) {
  
  if (b > 3) b = 0; // turn off if not sure

  if(write_data(*fd, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1)) < 0) 
    perror("Writing blinkRate: ");
}

/* 
Helper button functions, the data is updated every readSwitches() call!
*/
bool isKeyPressed(uint8_t k) {
  if (k > 15) return false;
  k = buttonLUT[k];
  
  return (keys[k>>4] & (1 <<(k & 0x0F)));
  //return (keys[k>>4] & (k & 0x0F));
}

bool wasKeyPressed(uint8_t k) {
  if (k > 15) return false;
  k = buttonLUT[k];
  return (lastkeys[k>>4] & (1 <<(k & 0x0F)));
  //return (lastkeys[k>>4] & _BV(k & 0x0F));
}

bool justPressed(uint8_t k) {
  return (isKeyPressed(k) & !wasKeyPressed(k));
}

bool justReleased(uint8_t k) {
  return (!isKeyPressed(k) & wasKeyPressed(k));
}

/* 
Helper LED functions, the data is written on writeDisplay()
*/
bool isLED(uint8_t x) {
  if (x > 15) return false;
  x = ledLUT[x];
  //return ((displaybuffer[x >> 4] & (x & 0x0F)) > 0);
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


void writeDisplay(int *fd) {

  uint8_t toDisplay[16] = {0}; 
  uint8_t index = 0;

  for (uint8_t i = 0; i < 8; i++)
  {
      toDisplay[index++] = displaybuffer[i] & 0xFF;
      toDisplay[index] = displaybuffer[i] >> 8;
  }

  if ( write_block_data(*fd, 0x00, toDisplay, 16) < 0)
    perror("Writing display: ");

/*
  uint8_t reg = 0x00;
  for(uint8_t i = 0; i < 16;)
  {
      if(read_reg8 (fd, reg) < 0)
        printf("error while writing in display %d\n", i);
      reg++;
  }
  */

//  Wire.beginTransmission(i2c_addr);
//  Wire.write((uint8_t)0x00); // start at address $00

//  for (uint8_t i=0; i<8; i++) {
//    Wire.write(displaybuffer[i] & 0xFF);    
//    Wire.write(displaybuffer[i] >> 8);    
//  }
//  Wire.endTransmission();  
}

void clear(void) {
  memset(displaybuffer, 0, sizeof(displaybuffer));
}