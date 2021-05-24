//
// Created by SWQXDBA on 2021/5/24.
//
#include <iostream>
#include <graphics.h>
#include <conio.h>
#ifndef EASYXTEST_PLAYERFIGHTER_H
#define EASYXTEST_PLAYERFIGHTER_H

class playerFighter {
public:
    IMAGE picture;
    int x;
    int y;
    int HP;
    int Attack;
    bool isDead(){
        return HP<=0;
    }
};



#endif //EASYXTEST_PLAYERFIGHTER_H
