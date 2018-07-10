// PCA9536.h
// Interface for PCA9536 V2 (Blue Label) with NVIDIA Jetson TK1
// The MIT License (MIT)


#ifndef PCA9536_H
#define PCA9536_H

#include <cstddef>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


// I2C Slave Address
#define kPCA9536I2CAddress                    0x41

// Internal Control Registers
#define kPCA9536CommandControlRegister        0x01    // Command Control Register

// High byte set means read two bytes
#define kPCA9536TempMSB          0x02    // Calculated distance in cm (difference between signal and reference delay)
                                                        // High byte of calculated delay of signal [Read Only]: reference – calculated after correlation record processing
                                                        // If the returned MSB is 1 then the reading is not considered valid.

#define kPCA9536TempLSB          0x03    // Low byte of calculated delay of signal [Read Only]: reference – calculated after correlation record processing
#define MCP9808_REG_AMBIENT_TEMP         0x05
// External Control Registers
//#define kPCA9536HardwareVersion               0x41    // Hardware Version: revisions begin with 0x01
//#define kPCA9536SoftwareVersion               0x4f    // Software Version: Revisions begin with 0x01

// Register Command
#define kPCA9536Measure                       0x00    // Take acquisition & correlation processing with DC correction

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
   int writePCA9536(int writeRegister,int writeValue) ;
    int readPCA9536(int readRegister) ;
    float getTemp();
    //int getTemp() ;

    int getError() ;
  //uint16_t read16(uint8_t reg);
  
  //private:

//uint8_t _i2caddr;

};

#endif // PCA9536_H
