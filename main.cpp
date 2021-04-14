#include <iostream>
#include <utility>
#include <thread>
#include <fstream>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <queue>
#include <cmath>
#include "Address.h"
#include "Car.h"
#include "Driver.h"
#include "Order.h"
#include "Passenger.h"
#include <dir.h>
#include <filesystem>

using namespace std;


int main(){
    mkdir("Passengers",0777);
    mkdir("Passengers/orderHistory",0777);
    mkdir("Passengers/Users",0777);
    mkdir("Passengers/Accepted",0777);
    mkdir("Drivers",0777);
    mkdir("Drivers/orderHistory",0777);
    mkdir("Drivers/Users",0777);
    string input;
    PassengerGateway x;
    DriverGateway y;
    string name = "";
    while(true){
        cout<<name<<"& ";
        cin>> input;
        if(input == "exit" || input == "out"){
            return 1;
        }else if(input == "login"){
            // name = x.SingIn();
            name = y.SingIn();
        }else if(input == "register"){
            // x.SingUp();
            y.SingUp();
        }else if(input == "info"){
            // x.info();
            y.info();
        }else if(input == "set_payment"){
            x.set_payment();
        }else if(input == "set_pinned_address"){
            x.set_pinned_address();
        }else if(input == "order_driver"){
            x.order_driver();
        }else if(input == "show_history"){
            x.show_history();
            y.show_history();
        }else if(input == "show_active_orders"){
            y.show_active_orders();
        }else if(input == "set_status"){
            y.set_status();
        }else if(input == "set_car"){
            y.set_car();
        }else if(input == "add_bottles"){
            y.add_bottles();
        }else if(input == "select_order"){
            y.select_order();
        }
    }
}