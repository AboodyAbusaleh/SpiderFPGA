/**
 * @file   Main.cpp
 * @brief  Tests the Spider robot rc servo movements (Assignment 1)
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h> // Required for usleep
#include <string.h>
#include <time.h>
#include <iostream>
#include "MMap.h"
#include "Motor.h"
#include "LEDControl.h"
#include "SpiderLeg.h"   // <-- add this

using namespace std;

int main(int argc, char *argv[]){

    cout << "\nProgram Starting...!" << endl;
     
    // 1. Create necessary class objects
    MMap *m_map = new MMap();
    LEDControl *pio = new LEDControl(m_map);
         int HIP_ID   = 0;
         int KNEE_ID  =1;
         int ANKLE_ID = 2;
    cout << "\nEnter which leg to control (e.g., RF, RM, RB, LF, LM, LB): ";
    string legInput;
    cin >> legInput;
    if (legInput != "RF" && legInput != "RM" && legInput != "RB" &&
        legInput != "LF" && legInput != "LM" && legInput != "LB") {
        cout << "Invalid leg input. Exiting program." << endl;
        delete pio;      
        delete m_map;
        return 1;
    } else if (legInput == "RF") {
        HIP_ID   = RF_hip;
        KNEE_ID  = RF_knee;
        ANKLE_ID = RF_ankle;
    } else if (legInput == "RM") {
         HIP_ID   = RM_hip;
         KNEE_ID  = RM_knee;
         ANKLE_ID = RM_ankle;
    } else if (legInput == "RB") {
         HIP_ID   = RB_hip;
         KNEE_ID  = RB_knee;
         ANKLE_ID = RB_ankle;
    } else if (legInput == "LF") {
        HIP_ID   = LF_hip;
         KNEE_ID  = LF_knee;
         ANKLE_ID = LF_ankle;
    } else if (legInput == "LM") {
         HIP_ID   = LM_hip;
        KNEE_ID  = LM_knee;
         ANKLE_ID = LM_ankle;
    } else if (legInput == "LB") {
         HIP_ID   = LB_hip;
        KNEE_ID  = LB_knee;
         ANKLE_ID = LB_ankle;
    }

    SpiderLeg *leg = new SpiderLeg(m_map, HIP_ID, KNEE_ID, ANKLE_ID);

 
    leg->Reset();
    while (!leg->IsReady()) {
        usleep(10000);
    }

    leg->SetSpeed(SpiderLeg::Hip,   80);
    leg->SetSpeed(SpiderLeg::Knee,  80);
    leg->SetSpeed(SpiderLeg::Ankle, 80);

    cout << "Leg initialized to 0.0 degrees on all joints." << endl;
    cout << "Use KEY1 to Increment Knee (+15), KEY0 to Decrement Knee (-15)" << endl;

 
    float kneeAngle = 0.0f;
    float AnkleAn =0.0f;
 
    while (true) {
       
        // Check KEY1 (Increment knee)
        if (pio->ReadButton(1)) {
            if (leg->IsReady()) {
                kneeAngle += 15.0f;
              AnkleAn -= 15.0f;
         
                if (kneeAngle > 90.0f) kneeAngle = 90.0f;
                if (AnkleAn < -90.0f) AnkleAn = -90.0f;

                leg->MoveJoint(SpiderLeg::Knee, kneeAngle);
                leg->MoveJoint(SpiderLeg::Ankle, AnkleAn);
                cout << "Knee moving to: " << kneeAngle << " degrees" << endl;
                cout << "Ankle moving to: " << AnkleAn << " degrees" << endl;
                usleep(400000);
            }
        }
       
   
        else if (pio->ReadButton(0)) {
            if (leg->IsReady()) {
                kneeAngle -= 15.0f;
                AnkleAn += 15.0f;
               
                if (kneeAngle < -90.0f) kneeAngle = -90.0f;

                if (AnkleAn > +90.0f) AnkleAn = -90.0f;

                leg->MoveJoint(SpiderLeg::Knee, kneeAngle);
              

               
                leg->MoveJoint(SpiderLeg::Ankle, AnkleAn);
                cout << "Knee moving to: " << kneeAngle << " degrees" << endl;
                 cout << "Ankle moving to: " << AnkleAn << " degrees" << endl;

                // Small delay to debounce button press (400ms)
                usleep(400000);
            }
        }
       
       
        if (pio->Read1Switch(0) && pio->Read1Switch(1)) {
            break;
        }
       
        usleep(10000);
    }

   
    delete leg;
    delete pio;      
    delete m_map;

    cout << "Terminating..." << endl;
    return 0;
}
