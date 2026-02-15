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

using namespace std;

int main(int argc, char *argv[]){

      cout << "\nProgram Starting...!" << endl;
      
    // Create necessary class objects
      MMap *m_map = new MMap();
      LEDControl *pio = new LEDControl(m_map);
   
    // Select a motor to test 
    cout<<"Choose the number of the motor you want to move 0-17: "<<endl;
    int motor_num=0;
    cin>>motor_num;
    
    Motor *motor = new Motor(m_map,motor_num);

      //Initialize Variables
    float currentAngle = 0.0;
   
    //Initialize motor to 0 degrees
    motor->Reset();
    motor->SetSpeed(50); //Set a reasonable speed
    cout << "Motor initialized to 0.0 degrees." << endl;
    cout << "Press KEY1 to Increment (+15), KEY0 to Decrement (-15)" << endl;

    //Control Loop
    while(true) {
       
        // Check KEY1
        if (pio->ReadButton(1)) {
            if (motor->IsReady()) {
                currentAngle += 15.0;
               
                //Clamp Angle
                if (currentAngle > 90.0) currentAngle = 90.0;
               
                motor->Move(currentAngle);
                cout << "Moving to: " << currentAngle << " degrees" << endl;
               
                // Small delay to debounce button press (200ms)
                usleep(200000);
            }
        }
       
        // Check KEY0 (Decrement)
        else if (pio->ReadButton(0)) {
            if (motor->IsReady()) {
                currentAngle -= 15.0;
               
                //Clamp Angle
                if (currentAngle < -90.0) currentAngle = -90.0;
               
                motor->Move(currentAngle);
                cout << "Moving to: " << currentAngle << " degrees" << endl;
               
                //Small delay to debounce button press (200ms)
                usleep(200000);
            }
        }
       
        //Exit condition (if both switches 0 and 1 are ON)
        if (pio->Read1Switch(0) && pio->Read1Switch(1)) {
            break;
        }
       
  
        usleep(10000);
    }
      
   
      delete motor;
      delete pio;       
      delete m_map;
      cout << "Done..." << endl;
      return 0;
}