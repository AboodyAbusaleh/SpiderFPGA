/**
 * @file   SpiderLeg.cpp
 * @Author Richard, Modified by: John Kimani (j.kimani@northeastern.edu)
 * @date   Modified on: 11/01/2025
 * @brief  Process a spider leg with 3 joints: Hip, Knee, and Ankle
 *
 * Contains a SpiderLeg class that provides functions to operate
 * on the various legs on the Terasic Spider robot
 */
#include <cassert>
#include "SpiderLeg.h"

/**
 *  Constructor initializes 3 dynamic motor objects for the Leg
 */
SpiderLeg::SpiderLeg(MMap* mmio,
                     int Joint0_MotorID,
                     int Joint1_MotorID,
                     int Joint2_MotorID)
{
	m_szMotor[0] = new Motor(mmio, Joint0_MotorID);
	m_szMotor[1] = new Motor(mmio, Joint1_MotorID);
	m_szMotor[2] = new Motor(mmio, Joint2_MotorID);
}

/**
 * Delete the 3 dynamic motor objects for the Leg
 */
SpiderLeg::~SpiderLeg() {
	for(int i = 0; i < JOINT_NUM; i++){
		delete m_szMotor[i];
		m_szMotor[i] = nullptr;
	}
}

/**
 * Reset the 3 joints on the Leg
 */
void SpiderLeg::Reset(void) {
	for (int i = 0; i < JOINT_NUM; ++i) {
		assert(m_szMotor[i] != nullptr);
		// Call the Motor reset function
		m_szMotor[i]->Reset();      // <-- use the actual Motor reset API
	}
}

/**
 * Move the specified joint to the specified angle
 */
void SpiderLeg::MoveJoint(JOINT_ID JointID, float fAngle){
	assert(JointID >= 0 && JointID < JOINT_NUM);
	assert(m_szMotor[JointID] != nullptr);

	// Call the Motor function that moves to an angle
	m_szMotor[JointID]->Move(fAngle);   // <-- e.g. MoveTo(), SetAngle(), etc.
}

/**
 *  Check if all the joints on the Leg are ready
 */

bool SpiderLeg::IsReady(void){
	for (int i = 0; i < JOINT_NUM; ++i) {
		assert(m_szMotor[i] != nullptr);
		// If ANY joint is not ready, entire leg is not ready
		if (!m_szMotor[i]->IsReady()) {    // <-- use actual IsReady() API
			return false;
		}
	}
	return true;
}

/**
 *  Get the angle for the specified joint
 */

float SpiderLeg::GetfAngle(JOINT_ID JointID)
{
	assert(JointID >= 0 && JointID < JOINT_NUM);
	assert(m_szMotor[JointID] != nullptr);

	// Return current angle reported by that motor
	return m_szMotor[JointID]->GetfAngle();    // <-- e.g. GetAngle(), GetfAngle(), etc.
}

/**
 *  Set the speed for the specified joint
 */

void SpiderLeg::SetSpeed(JOINT_ID JointID, int Speed)
{
	assert(JointID >= 0 && JointID < JOINT_NUM);
	assert(m_szMotor[JointID] != nullptr);

	m_szMotor[JointID]->SetSpeed(Speed);     // <-- use actual SetSpeed() API
}

/**
 *  Return the speed for the specified joint
 */

uint32_t SpiderLeg::GetSpeed(JOINT_ID JointID)
{
	assert(JointID >= 0 && JointID < JOINT_NUM);
	assert(m_szMotor[JointID] != nullptr);

	return m_szMotor[JointID]->GetSpeed();   // <-- use actual GetSpeed() API
}
