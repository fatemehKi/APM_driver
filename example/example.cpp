/* created  Jul 10, 2018

by Fateme Kiaie
*/


#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>


#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>


//#include <PCA9536.h>


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
  
    int getError() ;

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
	int CmdByte;
	char rxBuffer[32];  // receive buffer
  	char txBuffer[32];  // transmit buffer
	PCA9536 *pca9536 = new PCA9536() ;
	
	char fileNameBuffer[32];
	//sprintf(fileNameBuffer,"/dev/i2c-%d", kI2CBus);
	int I2CFile= open(fileNameBuffer, O_RDWR);
	int err = pca9536->openPCA9536();
	
	// Create I2C bus
	//kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
	//char *bus = "/dev/i2c-1";
	//if((file = open(bus, O_RDWR)) < 0) 
	//{
		//printf("Failed to open the bus. \n");
		//exit(1);
	//}
	// Get I2C device, PCA9536_R11 I2C address is 0x41(65)
	//ioctl(file, I2C_SLAVE, 0x41);
	char APM_ConfigReg[2] = {0x03, 0x00};
	int ret = write(I2CFile, APM_ConfigReg, 2);
	
	// Turn on all channels at startup
                CmdByte &= 0xF0;

                //uint8_t APM_OutputReg[2] = {0x01, 0x00};
                char APM_OutputReg[2];
                APM_OutputReg[0] = 0x01;
                APM_OutputReg[1] = CmdByte;
	ret = write(I2CFile, APM_OutputReg, 2);
	
	// Select configuration register(0x03)
	// Set all pins as OUTPUT(0x00)
	//////////int config[2] =  {0};
	//////////config[0] = 0x03;
	//////////config[1] = 0x00;
	//////////int ret = pca9536->writePCA9536(kI2CFileDescriptor, config[0]);
	usleep(1);

	// Select output port register(0x01)
	// Set pin-1 as HIGH(0x01)
	char config[2] = {0};
	config[0] = 0x01;
	config[1] = 0x01;
	//ret = pca9536->writePCA9536(kI2CFileDescriptor, config[1]);
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
