/**
 * @file   MMap.cpp
 * @Author John Kimani (j.kimani@northeastern.edu)
 * @date   August, 2025
 * @brief  Process GPIO input and output for the DE1-SoC
 *
 * Implements a MMap class that opens GPIO ports through 
 * memory-mapping for reading switches and push buttons and 
 * writing to LEDs
 */
 
 // system libraries
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <time.h>
#include <string.h>

#include "MMap.h"
using namespace std;

/**
 * Initialize general-purpose I/O
 *  - Opens access to physical memory /dev/mem
 *  - Maps memory at offset into virtual address space
 *
 * @param  	None
 * @return	None
 */
MMap::MMap()
{
	fd = -1;	pBase = (char *)MAP_FAILED;
	// Open /dev/mem to give access to physical addresses
	fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if (fd == -1)		//  check for errors in openning /dev/mem
	{
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
	}
	
   // Get a mapping from physical addresses to virtual addresses
   pBase = (char *)mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
   if (pBase == MAP_FAILED)		// check for errors
   {
      cout << "ERROR: mmap() failed..." << endl;
      close (fd);		// close memory before exiting
      exit(1);
   } 
}

/**
 * Close general-purpose I/O.
 *
 * @param 	None
 * @param 	None
 * Close the previously-opened virtual address mapping
 * Close /dev/mem to give access to physical addresses
 */
MMap::~MMap()
{
	if (munmap (pBase, LW_BRIDGE_SPAN) != 0)
	{
      cout << "ERROR: munmap() failed..." << endl;
      exit(1);
	}
   close (fd); 	// close memory
   fd = -1;	pBase = (char *)MAP_FAILED;
}

/**
 * @return true if the mapping has been established succesfully, else false
 */
bool MMap::isMapped() {
	return (pBase != MAP_FAILED) && (fd != -1);
}

/**
 * Write a 4-byte value at the specified general-purpose I/O location.
 *
 * @param reg_offset	Offset where device is mapped.
 * @param value		Value to be written.
 */
void MMap::RegisterWrite(unsigned int reg_offset, int value)
{
	* (volatile unsigned int *)(pBase + ((LW_FPGASLVS_OFST + reg_offset) & LW_REGS_MASK)) = value;
}
	
/**
 * Read a 4-byte value from the specified general-purpose I/O location.
 *
 * @param reg_offset	Offset where device is mapped.
 * @return		Value read.
 */
int MMap::RegisterRead(unsigned int reg_offset)
{
	return * (volatile unsigned int *)(pBase + ((LW_FPGASLVS_OFST + reg_offset) & LW_REGS_MASK));
}
