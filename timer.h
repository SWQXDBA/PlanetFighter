//
// Created by SWQXDBA on 2021/5/24.
//
#include <ctime>
#ifndef EASYXTEST_TIMER_H
#define EASYXTEST_TIMER_H
class Timer{

    //¶¨Ê±Æ÷
    time_t lasttime;
public:
    bool passedtime(int s){
        if(lasttime==0){
            lasttime=time(NULL);
            return true;
        }
        time_t nowtime = time(NULL);
        if(nowtime-lasttime>=s){
            lasttime=nowtime;
            return true;
        }
        return false;
    }
};
#endif //EASYXTEST_TIMER_H
