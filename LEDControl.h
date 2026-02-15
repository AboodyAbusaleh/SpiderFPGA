/**
 * @file   LEDControl.h
 * @Author John Kimani (j.kimani@northeastern.edu)
 * @date   June, 2025
 * @brief  Process GPIO input and output for the DE1-SoC
 *
 * Contains a LedCount program that opens GPIO ports through 
 * memory-mapping for reading switches and push buttons and 
 * writing to LEDs
 */

#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include "RegisterMap.h"
#include "MMap.h"

/* Cyclone V FPGA device addresses */
const unsigned int LEDR_BASE 		= LED_PIO_BASE;		// Leds offset 
const unsigned int SW_BASE 			= DIPSW_PIO_BASE;	// Switches offset
const unsigned int KEY_BASE 		= BUTTON_PIO_BASE;	// Push buttons offset

/*
*	Class LEDControl
*/
class LEDControl{
  private:
	MMap* m_map;
	
  public:
	LEDControl(MMap* mmio);		// Constructor
	~LEDControl();		// Destructor

	void Write1Led(int LedNum, int state);
	void WriteAllLeds(int value);
	int Read1Switch(int switchNum);
	int ReadAllSwitches();
	int ReadButton(int buttonKey);
};

#endif