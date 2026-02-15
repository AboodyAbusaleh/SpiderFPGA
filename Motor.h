 /**
 * @file   Motor.h
 * @Author Richard, Modified by: John Kimani (j.kimani@northeastern.edu)
 * @date   Modified on: 11/01/2025
 * @brief  Process an RC Serco motor in the Terasic Spider
 *
 * Contains a Motor class that provides functions to operate
 * on the various rc servos on the Terasic Spider robot
 */

#ifndef CMOTOR_H_
#define CMOTOR_H_

#include "MMap.h"

/* An array of PWM BASE register map for all servos */
const uint32_t szPWM_Base[] = {
		PWM0_BASE,
		PWM1_BASE,
		PWM2_BASE,
		PWM3_BASE,
		PWM4_BASE,
		PWM5_BASE,
		PWM6_BASE,
		PWM7_BASE,
		PWM8_BASE,
		PWM9_BASE,
		PWM10_BASE,
		PWM11_BASE,
		PWM12_BASE,
		PWM13_BASE,
		PWM14_BASE,
		PWM15_BASE,
		PWM16_BASE,
		PWM17_BASE
};

// Registers for each PWM signal. Each register is 4 bytes (32 bits) away
#define REG_TOTAL_DUR	( 0 * 4 )	// hence multiply each offset by 4
#define REG_HIGH_DUR	( 1 * 4 )
#define REG_ADJ_SPEED   ( 2 * 4 )
#define REG_ADJ_STATUS  ( 2 * 4 )
#define REG_ABORT       ( 3 * 4 )

// PWM period, duty cycle, & Speed limits
#define PWM_PERIOD (50000000/50)    // clock count in 20 ms = 1000000 cycles
#define PWM_MAX    (PWM_PERIOD*25/200) // 2.5 ms = 125000 cycles
#define PWM_MIN    (PWM_PERIOD*5/200)  // 0.5 ms = 25000 cycles

#define SPEED_MAX        100	// Max user input for speed
#define SPEED_MIN        0		// Min user input for speed

#define DEGREE_MIN		-90		// Min angle input for 0.5 ms
#define DEGREE_MAX		 90		// Max angle input for 2.5 ms

#define DELAY_MIN 		1000 	// Minimum delay = high speed
#define DELAY_MAX 		1700 	// Maximum delay = low speed

// Terasic Spider motor indices
// R=Right, L=Left, F=Front, M=Middle, B=BACK
const uint32_t RF_hip 		= 0;
const uint32_t RF_knee 		= 1;
const uint32_t RF_ankle 	= 2;
const uint32_t RM_hip 		= 3;
const uint32_t RM_knee 		= 4;
const uint32_t RM_ankle 	= 5;
const uint32_t RB_hip 		= 6;
const uint32_t RB_knee 		= 7;
const uint32_t RB_ankle 	= 8;
const uint32_t LF_hip 		= 9;
const uint32_t LF_knee 		= 10;
const uint32_t LF_ankle 	= 11;
const uint32_t LM_hip 		= 12;
const uint32_t LM_knee 		= 13;
const uint32_t LM_ankle 	= 14;
const uint32_t LB_hip 		= 15;
const uint32_t LB_knee 		= 16;
const uint32_t LB_ankle 	= 17;

/*
*	Class Motor
*/
class Motor {
private:
	//The ID of the motor, this should correspond to the
	//PWM index (0-17) as specified in the Spider Robot documentation.
	int m_nMotorID;
	//The current angle (in the range [-90,90]) of the servo motor.
	float motor_angle;
	//The current effective operating speed.
	uint32_t motor_speed;
	
	// Memory map object for register access
	MMap* mmio;

public:
	// Class Member functions
	Motor(MMap* mmio, int nMonotrID);
	~Motor();
	void Reset(void);
	bool IsReady(void);
	float GetfAngle(void);
	void SetSpeed(int Speed);
	uint32_t GetSpeed(void);
	void Move(float fAngle);
};

#endif /* CMOTOR_H_ */
