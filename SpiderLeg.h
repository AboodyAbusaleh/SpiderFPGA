 /**
 * @file   SpiderLeg.h
 * @Author Richard, Modified by: John Kimani (j.kimani@northeastern.edu)
 * @date   Modified on: 11/01/2025
 * @brief  Process a spider leg with 3 joints: Hip, Knee, and Ankle
 *
 * Contains a SpiderLeg class that provides functions to operate
 * on the various legs on the Terasic Spider robot
 */

#ifndef SPIDERLEG_H_
#define SPIDERLEG_H_

#include "MMap.h"
#include "Motor.h"

/*
*	Class Spider
*/
class SpiderLeg {
public:
	// Define the 3 joints for each leg
	typedef enum{
		Hip=0,
		Knee,
		Ankle,
		JOINT_NUM
	}JOINT_ID;

private:
	Motor *m_szMotor[JOINT_NUM];	// Array of 3 joints

public:
	SpiderLeg(MMap* mmio, int Joint0_MotorID,int Joint1_MotorID,int Joint2_MotorID);
	~SpiderLeg();

	void Reset(void);
	void MoveJoint(JOINT_ID JointID, float fAngle);
	bool IsReady(void);
	float GetfAngle(JOINT_ID JointID);
	void SetSpeed(JOINT_ID JointID,int Speed);
	uint32_t GetSpeed(JOINT_ID JointID);
};

#endif /* SPIDERLEG_H_ */
