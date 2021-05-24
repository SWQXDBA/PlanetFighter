//
// Created by SWQXDBA on 2021/5/24.
//

#ifndef EASYXTEST_ENEMY_H
#define EASYXTEST_ENEMY_H

#include <iostream>
#include <graphics.h>
#include <conio.h>
#include "model.h"
extern int raw;
extern int column;
class Enemy {
public:
    IMAGE picture;
    int x;
    int y;
    int HP = 10;
    int Attack;
    int movetoX;
    int movetoY;
    int speed=20;

    void move(){
        if(step<=1){
            movetoY = rand()%column;
            movetoX = rand()%raw;
            step=speed;
        }
        x += (movetoX-x)/step;
        y += (movetoY-y)/step;
        step--;
    }
    bool isDead(){
        return HP<=0;
    }

private:
    int step=-1;
};


#endif //EASYXTEST_ENEMY_H
