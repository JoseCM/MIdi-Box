#include "gpio.h"

#define MAX_BUF 128
char *directions[2] = {"in", "out"};

int fd;
char buf[MAX_BUF]; 
int gpio = 25;

int gpio_fd[28];
int gpio_active[28] = {0};
DIR gpio_dir[28];

void gpio_init_pin(uint8_t gpio, DIR direction){


	if(gpio_active[gpio] && (gpio_dir[gpio] == direction))
		return;

	if(!gpio_active[gpio]){
	
		gpio_active[gpio] = 1;

		fd = open("/sys/class/gpio/export", O_WRONLY);
		sprintf(buf, "%d", gpio); 
		write(fd, buf, strlen(buf));
		close(fd);	

		gpio_dir[gpio] = direction;
		sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
		fd = open(buf, O_WRONLY);	
		write(fd,  directions[direction], strlen(directions[direction])); 
		close(fd);

		sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
		gpio_fd[gpio] = open(buf, O_RDWR);

	} else if(gpio_dir[gpio] != direction) {

		gpio_dir[gpio] = direction;
		sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
		fd = open(buf, O_WRONLY);	
		write(fd,  directions[direction], strlen(directions[direction])); 
		close(fd);
		

	}

}

int gpio_set_pin(uint8_t gpio, VAL value){

	char v;

	if(gpio_dir[gpio] != OUT)
		return -1;

	v = value + '0';

	write(gpio_fd[gpio], &v, 1); 

	return 0;

}

int gpio_read_pin(uint8_t gpio){

	char v;

	if(gpio_dir[gpio] != OUT)
		return -1;

	read(gpio_fd[gpio], &v, 1); 

	return v;
}

void gpio_deinit_pin(uint8_t gpio){


	if(!gpio_active[gpio])
		return;

	gpio_active[gpio] = 0;

	close(gpio_fd[gpio]);

	fd = open("/sys/class/gpio/unexport", O_WRONLY);

	sprintf(buf, "%d", gpio);

	write(fd, buf, strlen(buf));

	close(fd);


}