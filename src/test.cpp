#include "Inspire.hpp"
#include <iostream>
#include <string.h>
#include <yaml-cpp/yaml.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
bool Isrunning = true;
void signalHandler(int signo)
{
    if (signo == SIGINT)
    {
        std::cout << "\033[1;31m"
                  << "[!!SIGNAL!!]"
                  << "INTERRUPT by CTRL-C"
                  << "\033[0m" << std::endl;

        Isrunning = false;

        exit(0);
    }
}
void PerformOpenGesture(InspireAction *inspireAction, InspireAction *inspireAction1)
{
    // Openhand手势的角度
    inspireAction->ThumbRotAction(1000);  // 大拇指旋转
    inspireAction->ThumbAction(1000);     // 大拇指弯曲
    inspireAction->IndexAction(1000);     // 食指弯曲
    inspireAction->MiddleAction(1000);    // 中指弯曲
    inspireAction->RingAction(1000);      // 无名指弯曲
    inspireAction->PinkyAction(1000);     // 小指弯曲

    inspireAction1->ThumbRotAction(1000);  // 大拇指旋转
    inspireAction1->ThumbAction(1000);     // 大拇指弯曲
    inspireAction1->IndexAction(1000);     // 食指弯曲
    inspireAction1->MiddleAction(1000);    // 中指弯曲
    inspireAction1->RingAction(1000);      // 无名指弯曲
    inspireAction1->PinkyAction(1000);     // 小指弯曲
}


void PerformOKGesture(InspireAction *inspireAction, InspireAction *inspireAction1)
{
    // 示例手势OK的角度
    inspireAction->ThumbRotAction(1000);  // 大拇指旋转
    inspireAction->ThumbAction(500);     // 大拇指弯曲
    inspireAction->IndexAction(500);     // 食指弯曲
    inspireAction->MiddleAction(1000);    // 中指弯曲
    inspireAction->RingAction(1000);      // 无名指弯曲
    inspireAction->PinkyAction(1000);     // 小指弯曲

    inspireAction1->ThumbRotAction(1000);  // 大拇指旋转
    inspireAction1->ThumbAction(500);     // 大拇指弯曲
    inspireAction1->IndexAction(500);     // 食指弯曲
    inspireAction1->MiddleAction(1000);    // 中指弯曲
    inspireAction1->RingAction(1000);      // 无名指弯曲
    inspireAction1->PinkyAction(1000);     // 小指弯曲
}


void PerformBallGesture(InspireAction *inspireAction, InspireAction *inspireAction1)
{
    // 示例手势Ball的角度
    inspireAction->ThumbRotAction(620);  // 大拇指旋转
    inspireAction->ThumbAction(260);     // 大拇指弯曲
    inspireAction->IndexAction(600);     // 食指弯曲
    inspireAction->MiddleAction(600);    // 中指弯曲
    inspireAction->RingAction(600);      // 无名指弯曲
    inspireAction->PinkyAction(600);     // 小指弯曲

    inspireAction1->ThumbRotAction(620);  // 大拇指旋转
    inspireAction1->ThumbAction(260);     // 大拇指弯曲
    inspireAction1->IndexAction(600);     // 食指弯曲
    inspireAction1->MiddleAction(600);    // 中指弯曲
    inspireAction1->RingAction(600);      // 无名指弯曲
    inspireAction1->PinkyAction(600);     // 小指弯曲
}


void PerformOpenBallGesture(InspireAction *inspireAction, InspireAction *inspireAction1)
{
    // 示例手势OpenBall的角度
    inspireAction->ThumbRotAction(620);  // 大拇指旋转
    inspireAction->ThumbAction(260);     // 大拇指弯曲
    inspireAction->IndexAction(1000);     // 食指弯曲
    inspireAction->MiddleAction(1000);    // 中指弯曲
    inspireAction->RingAction(1000);      // 无名指弯曲
    inspireAction->PinkyAction(1000);     // 小指弯曲

    inspireAction1->ThumbRotAction(620);  // 大拇指旋转
    inspireAction1->ThumbAction(260);     // 大拇指弯曲
    inspireAction1->IndexAction(1000);     // 食指弯曲
    inspireAction1->MiddleAction(1000);    // 中指弯曲
    inspireAction1->RingAction(1000);      // 无名指弯曲
    inspireAction1->PinkyAction(1000);     // 小指弯曲
}


void DisplayMenu()
{
    std::cout << "Please select a gesture to perform:" << std::endl;
    std::cout << "1. Open Gesture" << std::endl;
    std::cout << "2. OK Gesture" << std::endl;
    std::cout << "3. Ball Gesture" << std::endl;
    std::cout << "4. OpenBall Gesture" << std::endl;
   
    std::cout << "6. Exit" << std::endl;
}
int main(int argc, char const *argv[])
{
    if (signal(SIGINT, signalHandler) == SIG_ERR)
    {
        std::cout << "\033[1;31m"
                  << "Can not catch SIGINT"
                  << "\033[0m" << std::endl;
    }
    Inspire inspire;
    if (inspire.InitCan() != 0)
    {
        std::cout << "It is error when init can" << std::endl;
        return 1;
    }
    


    usleep(1000000);
    int handid0 = 1;
    int handid1 = 2;

    InspireAction *inspireAction = new InspireAction(&inspire, handid0);
    InspireAction *inspireAction1 = new InspireAction(&inspire, handid1);
  
    int single_value_speed=Inspire::MAX_SPEED;
    int single_value=1000;
    std::vector<int> five_finger_speed={single_value_speed,single_value_speed,single_value_speed,single_value_speed,single_value_speed,single_value_speed};
    // 设置五指的速度
    inspireAction->SetFiveFingerSpeed(five_finger_speed);
    inspireAction1->SetFiveFingerSpeed(five_finger_speed);
    usleep(10000);
    std::vector<int> five_finger_value={1000,1000,0,0,0,0};
    std::vector<int> five_finger_value_r={1000,1000,1000,1000,1000,1000};
    // inspireAction->FiveFingerAction(five_finger_value_r);


    // // inspireAction1->FiveFingerAction(five_finger_value_r);
    inspireAction->FiveFingerAction(five_finger_value);
    std::vector<int> steps_0 = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,1000, 1000, 1000, 1000, 1000, 1000, 300};
    std::vector<int> steps_1 = {1000, 1000, 1000, 1000, 1000, 1000, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 300};
    
    inspireAction->setActionSequenceData(4,2,steps_0,steps_1);
    // // sleep(1);
    inspireAction1->RunActionSequence(3);//闭合握拳left
    sleep(1);
    inspireAction1->RunActionSequence(4);//张开left

    inspireAction->RunActionSequence(4);//张开right



    // while (Isrunning)
    // {
    //     DisplayMenu();
    //     int choice;
    //     std::cin >> choice;

    //     switch (choice)
    //     {
    //         case 1:
    //             PerformOpenGesture(inspireAction,inspireAction1);
    //             break;
    //         case 2:
    //             PerformOKGesture(inspireAction,inspireAction1);
    //             break;
    //         case 3:
    //             PerformBallGesture(inspireAction,inspireAction1);
    //             break;
    //         case 4:
    //             PerformOpenBallGesture(inspireAction,inspireAction1);
    //             break;
            
    //         case 6:
    //             Isrunning = false;
    //             break;
    //         default:
    //             std::cout << "Invalid choice, please select again." << std::endl;
    //     }

    //     usleep(200000);
    // }

    inspire.InspireCloseDevice();

    std::cout << "ok" << std::endl;
    return 0;
}
