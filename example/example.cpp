/* created  Jul 10, 2018

by Fateme Kiaie
*/


#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "PCA9536.h"

int main() 
{
	
	PCA9536 *pca9536 = new PCA9536() ;
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, PCA9536_R11 I2C address is 0x41(65)
	ioctl(file, I2C_SLAVE, 0x41);

	// Select configuration register(0x03)
	// Set all pins as OUTPUT(0x00)
	char config[2] =  {0};
	config[0] = 0x03;
	config[1] = 0x00;
	writePCA9536(file, config);
	usleep(1);

	// Select output port register(0x01)
	// Set pin-1 as HIGH(0x01)
	//char config[2] = {0};
	config[0] = 0x01;
	config[1] = 0x01;
	pca9536->writePCA9536(file, config);
	usleep(1);
	
	// Output to screen
	printf("Pin-1 state is : HIGH");
	
	/*
	// Select output port register(0x01)
	// Set pin-1 as LOW(0x00)
	char config[2] = {0};
	config[0] = 0x01;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);
	
	// Output to screen
	printf("Pin-1 state is : LOW");
	
	// Select output port register(0x01)
	// Set pin-2 as HIGH(0x02)
	char config[2] = {0};
	config[0] = 0x01;
	config[1] = 0x02;
	write(file, config, 2);
	sleep(1);
	
	// Output to screen
	printf("Pin-2 state is : HIGH");
	
	// Select output port register(0x01)
	// Set pin-2 as LOW(0x00)
	char config[2] = {0};
	config[0] = 0x01;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);
	
	// Output to screen
	printf("Pin-2 state is : LOW");*/
	return 0;
}
