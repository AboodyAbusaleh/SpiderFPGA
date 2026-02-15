/**
 * @file   LEDControl.cpp
 * @Author John Kimani (j.kimani@northeastern.edu)
 * @date   June, 2025
 * @brief  Process GPIO input and output for the DE1-SoC
 *
 * Contains a LEDControl class that opens GPIO ports through 
 * memory-mapping for reading switches and push buttons and 
 * writing to LEDs
 */
 
 // system libraries
#include <iostream>
#include "LEDControl.h"
using namespace std;

/**
 * Initialize general-purpose I/O
 *  - Opens access to physical memory /dev/mem
 *  - Maps memory at offset into virtual address space
 *
 * @param  fd	File descriptor passed by reference, where the result
 *		of function 'open' will be stored.
 * @return	Address to virtual memory which is mapped to physical,
 *          	or MAP_FAILED on error.
 */
LEDControl::LEDControl(MMap* mmio)
{
	m_map = mmio;
	cout << "Initializing LEDs, Switches, & Buttons..." << endl;
}

/**
 * Close general-purpose I/O.
 *
 * @param pBase	Virtual address where I/O was mapped.
 * @param fd	File descriptor previously returned by 'open'.
 * Close the previously-opened virtual address mapping
 * Close /dev/mem to give access to physical addresses
 */
LEDControl::~LEDControl()
{
	cout << "Closing LEDs, Switches, & Buttons..." << endl;
}

/**
 * Changes the state of an LED (ON or OFF)
 *
 * - Uses base address of I/O
 * @param pBase		Base address returned by 'mmap'.
 * @param ledNum	LED number (0 to 9)
 * @param state	State to change to (ON or OFF)
 */
void LEDControl::Write1Led(int LedNum, int state)
{
	unsigned int bitmask = (1 << LedNum);
	unsigned int leds_state = m_map->RegisterRead(LEDR_BASE);
	unsigned int new_state = (leds_state & ~bitmask) | (state << LedNum);
	m_map->RegisterWrite(LEDR_BASE, new_state);
}

/**
 * Show lower10 bits of integer value on LEDs
 *
 * Sets all LEDs to the given value
 * @param pBase		Base address returned by 'mmap'.
 * @param value	Value to show on LEDs
 */
void LEDControl::WriteAllLeds(int value)
{
	m_map->RegisterWrite(LEDR_BASE, value);
}

/**
 * Reads the value of a switch
 *
 * - Uses base address of I/O
 * @param pBase		Base address returned by 'mmap'.
 * @param switchNum	Switch number (0 to 9)
 * @return		Switch value read
 */
int LEDControl::Read1Switch(int switchNum)
{
	return ((m_map->RegisterRead(SW_BASE) >> switchNum) & 1);
}

/**
 * Reads the switch values into a decimal integer
 *
 * - Calls Read1Switch() to read all switches 
 * @param pBase		Base address returned by 'mmap'.
 * @return		Switches' value read
 */
int LEDControl::ReadAllSwitches()
{
	return m_map->RegisterRead(SW_BASE);
}

/**
 * Reads the button key values into a decimal integer
 *
 * - Determines the index of button pressed
 * @param buttonKey		Push button [0 or 1] for KEY0 or KEY1.
 * @return		1 if the push button is pressed, 0 otherwise
 */
int LEDControl::ReadButton(int buttonKey)
{
	int regValue = ~(m_map->RegisterRead(KEY_BASE));
	return (regValue >> buttonKey) & 0x1;
}
