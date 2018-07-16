/* created  Jul 10, 2018

by Fateme Kiaie
*/


#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

//I2C address of PCA9536
#define kPCA9536I2CAddress                    0x41


class PCA9536
{
public:
    unsigned char kI2CBus ;         // I2C bus of the PCA9536
    int kI2CFileDescriptor ;        // File Descriptor to the PCA9536
    int error ;
    PCA9536();
    ~PCA9536() ;
    bool openPCA9536() ;                   // Open the I2C bus to the PCA9536
    void closePCA9536();                   // Close the I2C bus to the PCA9536
   int writePCA9536(int writeRegister, int writeValue);
   int getError() ;
    struct ch_status {
	int c1_status;
	int c2_status;
	int c3_status;
	int c4_status;
} APM_st_msg;

};


PCA9536::PCA9536()
{
    kI2CBus = 1 ;           // Default I2C bus on Jetson TK1
    error = 0 ;
}

PCA9536::~PCA9536()
{
    closePCA9536() ;
}

// Returns true if device file descriptor opens correctly, false otherwise

bool PCA9536::openPCA9536()
{
    char fileNameBuffer[32];
    
    sprintf(fileNameBuffer,"/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
    if (kI2CFileDescriptor < 0) {
        // Could not open the file
        error = errno ;
        printf("Error: %d",  error);
        printf("Can not open the file");

        return false ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kPCA9536I2CAddress) < 0) {
        // Could not open the device on the bus
        error = errno ;
        
        printf("Error: %d",  error);
        printf("Can not open the device on the bus");
        return false ;
    }
    return true ;
}

int PCA9536::writePCA9536(int writeRegister, int writeValue)
{
    int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
    // Wait a little bit to make sure it settles
    usleep(10000);
    if (toReturn < 0) {
        error = errno ;
        toReturn = -1 ;
    }
    return toReturn ;

}

void PCA9536::closePCA9536()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        // WARNING - This is not quite right, need to check for error first
        kI2CFileDescriptor = -1 ;
    }
}


//************************************************

int main() 
{
	int CmdByte(0x0F);
	
	PCA9536 *pca9536 = new PCA9536() ;
			
	char fileNameBuffer[32];
	
	int I2CFile= open(fileNameBuffer, O_RDWR);
	int err = pca9536->openPCA9536();
	

	// Get I2C device, PCA9536 I2C address is 0x41(65)
	ioctl(I2CFile, I2C_SLAVE, kPCA9536I2CAddress);
	
	//Writing on Configuration to configure all as an output
	int ret = pca9536->writePCA9536(0x03, 0x00);
	
	usleep(100);
		
	// Turn on all channels at startup
        CmdByte &= 0xF0;

       
	ret = pca9536->writePCA9536(0x01, 0x00);
	//pca9536->APM_st_msg.c1_status = (CmdByte & 0x01) ? 0 : 1;
	
	printf("Pin-1 state is : %d", pca9536->APM_st_msg.c1_status);
	
	
	usleep(1);
	//turn off all channel
	//ret = pca9536->writePCA9536(0x01, 0xFF);
	// Select configuration register(0x03)
	// Set all pins as OUTPUT(0x00)
	//////////int config[2] =  {0};
	//////////config[0] = 0x03;
	//////////config[1] = 0x00;
	//////////int ret = pca9536->writePCA9536(kI2CFileDescriptor, config[0]);
	//usleep(100);

	//ret = pca9536->writePCA9536(0x03, 0x00);
	//usleep(1);
	//ret = pca9536->writePCA9536(0x01, 0x00);
	// Select output port register(0x01)
	// Set pin-1 as HIGH(0x01)
	//char config[2] = {0};
	//config[0] = 0x01;
	//config[1] = 0x01;
	//ret = pca9536->writePCA9536(kI2CFileDescriptor, config[1]);
	//usleep(1);
	
	// Output to screen
	//printf("Pin-1 state is : HIGH");
	
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
