#ifndef UNTITLED3_PAYMENT_H
#define UNTITLED3_PAYMENT_H

#include <iostream>
#include <utility>
#include <thread>
#include <fstream>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <string>
#include <queue>
#include <cmath>


using namespace std;
inline bool file_exists(const string& file_name) {
    ifstream f(file_name.c_str());
    return f.good();
}
class Payment{
private:
    string _card_number, _date;
public:
    bool _is_card = false;
    Payment() = default;
    Payment(bool is_card, string card_number, string date){
        _is_card = is_card;
        _card_number = std::move(card_number);
        _date = std::move(date);
    }
    string get_card_number(){
        return _card_number;
    }
    string get_date(){
        return _date;
    }
    static Payment from_string(string data){
        string name, res, card_number_, date_;
        bool tor = false, is_card = false;
        for(int i=1;i<data.size();i++){
            if(data[i] == '\''){
                if(!tor){
                    name = "";
                    res = "";
                }else{
                    i+=2;
                }
                tor = !tor;
            }else if(data[i] == '{'){
                int count = 1;
                res = "{";
                i++;
                while(count!=0){
                    res += data[i];
                    if(data[i] == '{'){
                        count ++;
                    }else if(data[i] == '}'){
                        count --;
                    }
                    i++;
                }
                i--;
            }else if(data[i] == ',' || data[i] == '}'){
                if(name == "date"){
                    date_ = res;
                }else if(name == "card_number"){
                    card_number_ = res;
                }else if(name == "is_card"){
                    is_card =( res == "1");
                }
                i+=1;
            }else{
                if(tor){
                    name += data[i];
                }else{
                    res += data[i];
                }
            }
        }
        return Payment(is_card, card_number_, date_);
    }
    string to_string(){
        string is_card;
        if(_is_card){
            is_card = "1";
        }else{
            is_card = "0";
        }
        return "{'is_card': "+is_card+", 'card_number': " + _card_number + ", 'date': " + _date + "}";
    }
};


#endif //UNTITLED3_PAYMENT_H
