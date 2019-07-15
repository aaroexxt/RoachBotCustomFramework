/* I2C_Aaron.c
*
* Author: Aaron Becker
* For COSMOS Cluster 6 2019
*
*/
#include "IO_Ports.h"
static char I2C_initted = FALSE;

int I2CAaron_Init(unsigned int speed) {
	if (!I2C_initted) {
		I2C_initted = true;
		//Configure output ports
		IO_PortsSetPortDirection(I2C_SDAOUT, 0x0000); //set port direction of output ports
		IO_PortsSetPortDirection(I2C_SCLOUT, 0x0000);
		//Set output port high

	}
}
void internal_startSeq(char I2Caddress) {

}