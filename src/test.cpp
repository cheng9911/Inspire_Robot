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
    InspireAction *inspireAction=new InspireAction(&inspire);
    int single_value_speed=Inspire::MAX_SPEED;
    int single_value=1000;
    std::vector<int> five_finger_speed={single_value_speed,single_value_speed,single_value_speed,single_value_speed,single_value_speed,single_value_speed};
    // 设置五指的速度
    inspireAction->SetFiveFingerSpeed(five_finger_speed);
    usleep(10000);
    inspireAction->ThumbAction(single_value);

    inspireAction->IndexAction(single_value);

    inspireAction->MiddleAction(single_value);

    inspireAction->RingAction(single_value);

    inspireAction->PinkyAction(single_value);
    usleep(200000);
    std::vector<int> value={1000,100,100,100,100,100};
    inspireAction->FiveFingerAction(value);
    usleep(10000);
    inspire.InspireCloseDevice();




    // YAML::Node config = YAML::LoadFile("config/inspire.yaml");

    // Read the values from the YAML file
    // int i = 0;
    // int j;
    // while (Isrunning)
    // {
    //     for(int i=0;i<10;i++)
    //     {
    //         if (inspire.WriteCan(1486 + i*2, 100, 1) != 0)
    //         {
    //             std::cout << "It is error when write" << std::endl;
    //         }
    //         usleep(100000);
    //     }
    //     for(int i=0;i<10;i++)
    //     {
    //         if (inspire.WriteCan(1486 + i*2, 1000, 1) != 0)
    //         {
    //             std::cout << "It is error when write" << std::endl;
    //         }
    //         usleep(100000);
    //     }
            

        
    // }

    // usleep(1000000);
    // inspire.InspireCloseDevice();
    return 0;
}
