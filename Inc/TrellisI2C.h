#ifndef _TRELLIS_H_
#define _TRELLIS_H_

/*************************
I2C Driver Rpi
*************************/
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



/**************************
TRELIS - HT16K33 - ADAFRUIT
**************************/
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

int Init_TrellisI2C(int *fd, const char *device, int devId);

void setBrightness(int *fd, uint8_t b);
void blinkRate(int *fd, uint8_t b);
void writeDisplay(int *fd);
void clear(void);
bool isKeyPressed(uint8_t k);
bool wasKeyPressed(uint8_t k);
bool isLED(uint8_t x);
void setLED(uint8_t x);
void clrLED(uint8_t x);
bool readSwitches(int *fd);
bool justPressed(uint8_t k);
bool justReleased(uint8_t k);


#endif