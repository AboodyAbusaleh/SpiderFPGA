/**
 * @file   Motor.cpp
 * @Author Richard, Modified by: John Kimani (j.kimani@northeastern.edu)
 * @date   Modified on: 11/01/2025
 * @brief  Process an RC Servo motor in the Terasic Spider
 */
 
#include "Motor.h"
#include "RegisterMap.h"
#include <math.h>
#include <iostream>
using namespace std;

Motor::Motor(MMap* mio, int MonotrID)
{
      m_nMotorID = MonotrID;        
      motor_angle = 0.0;                  
      motor_speed = DELAY_MAX;      
      mmio = mio;                         
      
      uint32_t duty_cycle = 75000;  // Neutral 0.0 degrees (1.5ms)
      
    // Setup the PWM period, default (duty cycle=75000), default speed=1700 cycles
      mmio->RegisterWrite((szPWM_Base[m_nMotorID] + REG_TOTAL_DUR), PWM_PERIOD);
      mmio->RegisterWrite((szPWM_Base[m_nMotorID] + REG_HIGH_DUR), duty_cycle);
      mmio->RegisterWrite((szPWM_Base[m_nMotorID] + REG_ADJ_SPEED), motor_speed);
      mmio->RegisterWrite((szPWM_Base[m_nMotorID] + REG_ABORT), 0);
}

Motor::~Motor() {
      // Stop sending PWM
      mmio->RegisterWrite((szPWM_Base[m_nMotorID] + REG_ABORT), 1);
}

/**
 * Puts the motor into the neutral, angle 0 position
 */
void Motor::Reset(void){
      Move(0.0);
}

/**
 * Checks if the servo has finished its previous movement.
 * Reads the LSB of the Status register (offset 2).
 */
bool Motor::IsReady(void)
{
    // Read register at offset 2 (REG_ADJ_STATUS)
    int status = mmio->RegisterRead(szPWM_Base[m_nMotorID] + REG_ADJ_STATUS);
   
    // Check Bit 0 (1 = Ready, 0 = Moving)
    if ((status & 0x01) == 1) {
        return true;
    }
      return false;
}

float Motor::GetfAngle(void)
{
      return motor_angle;
}

/**
 * Calculates delay cycles based on speed input (0-100).
 * Speed 0 = 1700 cycles (Slow), Speed 100 = 1000 cycles (Fast).
 */
void Motor::SetSpeed(int speed)
{
    // Clamp inputs
    if (speed > SPEED_MAX) speed = SPEED_MAX;
    if (speed < SPEED_MIN) speed = SPEED_MIN;

    // Linear equation: Delay = 1700 - (7 * speed)
    motor_speed = DELAY_MAX - (7 * speed);

    // Write to register
      mmio->RegisterWrite((szPWM_Base[m_nMotorID] + REG_ADJ_SPEED), motor_speed);
}

uint32_t Motor::GetSpeed(void)
{
      return motor_speed;
}

/**
 * Converts angle (-90 to 90) to Duty Cycle (25000 to 125000).
 * Handles Right-side motor inversion.
 */
void Motor::Move(float fAngle) {
    //Limit the angle bounds
    if (fAngle > DEGREE_MAX) fAngle = DEGREE_MAX;
    if (fAngle < DEGREE_MIN) fAngle = DEGREE_MIN;

    //Handle -0.0 edge case
    if (fAngle == -0.0f) fAngle = 0.0f;

    //Update internal state
    motor_angle = fAngle;

    //Handle Right Side Inversion (IDs 0-8)
    float effective_angle = fAngle;
    if (m_nMotorID < 9) {
        effective_angle = -fAngle;
    }

    //Calculate Duty Cycle
    // 0 deg = 75,000. Slope = 50,000 cycles / 90 degrees = 555.556 cycles/deg
    int duty_cycle = 75000 + (int)(effective_angle * 555.556f);

    //Write to register
    mmio->RegisterWrite((szPWM_Base[m_nMotorID] + REG_HIGH_DUR), duty_cycle);
}