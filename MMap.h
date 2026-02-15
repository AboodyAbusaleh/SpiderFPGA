/**
 * @file   MMap.h
 * @Author John Kimani (j.kimani@northeastern.edu)
 * @date   August, 2025
 * @brief  Process GPIO input and output for the DE1-SoC
 *
 * Contains a MMap class that opens GPIO ports through 
 * memory-mapping for reading switches and push buttons and 
 * writing to LEDs
 */

#ifndef MMAP_H
#define MMAP_H

#include <stdint.h>
#include "RegisterMap.h"

/* Cyclone V FPGA device addresses */
const unsigned int LW_BRIDGE_BASE 	= 0xFC000000;		// Base offset
const unsigned int LW_BRIDGE_SPAN 	= 0x04000000;		// Address map size
const unsigned int LW_REGS_MASK 	= 0x03FFFFFF;		// Address map size mask
const unsigned int LW_FPGASLVS_OFST	= 0xFF200000;		// ALT_LWFPGASLVS_OFST

/*
*	Class MMap
*/
class MMap {
  private:
	char *pBase;
	int fd;
  public:
	MMap();		// Constructor
	~MMap();		// Destructor
	bool isMapped();
	void RegisterWrite(unsigned int reg_offset, int value);
	int RegisterRead(unsigned int reg_offset);
};

#endif
