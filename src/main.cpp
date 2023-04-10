#include <iostream>
#include "Visualiser/Visualiser.hpp"

int main(){
    Visualiser visualiser;
    while(true){
        visualiser.visualise();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 10 Hz
    }
}