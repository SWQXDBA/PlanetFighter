//
// Created by SWQXDBA2 on 2021/5/26.
//

#ifndef EASYXTEST_MODEL_H
#define EASYXTEST_MODEL_H

#endif //EASYXTEST_MODEL_H

#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include<vector>
#include<cmath>
using namespace std;
extern int raw;
extern int column;
extern string path;
extern int shootSpeed;
extern int screan;

extern int leftMargin;
extern int rightMargin ;
extern int topMargin;
extern int bottomMargin;

class Timer {
    //定时器
    time_t lasttime;
public:
    bool passedtime(int s) {
        if (lasttime == 0) {
            lasttime = time(nullptr);
            return true;
        }
        time_t nowtime = time(nullptr);
        if (nowtime - lasttime >= s) {
            lasttime = nowtime;
            return true;
        }
        return false;
    }
};


class Bullet {
public:
    static vector<IMAGE> playBulletIMAGEs;
    int from ;//0 玩家发射的 1 敌人发射的
    IMAGE picture;
    int x;
    int y;
    int xmove;
    int ymove;
    int speed = 1;
    int ATTACK = 3;
    void bulletFly(){
        x+= (xmove * (1 + speed / screan));
        y+= (ymove * (1 + speed / screan));
    }
};

class BulletFactory {
public:
    vector<IMAGE> playBulletIMAGEs;
    vector<IMAGE> enemyBulletIMAGEs;
    BulletFactory() {

        IMAGE pi1;
        loadimage(&pi1, (path + "\\bullet.png").c_str(), raw/screan, raw/screan);
        playBulletIMAGEs.push_back(pi1);


        IMAGE ei1;
        loadimage(&ei1, (path + "\\bullet.png").c_str(), raw/screan, raw/screan);
        enemyBulletIMAGEs.push_back(ei1);
    }

    Bullet getPlayerBullet(int X, int Y, int loc) {
        Bullet nBullet;
        nBullet.picture = playBulletIMAGEs[loc];
        nBullet.x = X;
        nBullet.y = Y;
        nBullet.speed=shootSpeed;
        nBullet.from=0;
        nBullet.xmove = 0;
        nBullet.ymove = -5;
        return nBullet;
    }



    Bullet getEnemyBulletByType(int X, int Y, int loc,int speed) {
        Bullet nBullet;
        nBullet.picture = playBulletIMAGEs[loc];
        nBullet.x = X;
        nBullet.y = Y;
        nBullet.from=1;
        nBullet.speed=speed;
        nBullet.xmove = 0;
        nBullet.ymove = 5;
        return nBullet;
    }
};


class Enemy {
public:
    Timer shootTimer;
    Timer moveTimer;
    IMAGE picture;
    int x;
    int y;
    int HP = 10;
    int Attack;
    int movetoX;
    int movetoY;
    int speed = 20;

    void move() {

        if ((abs(movetoX-x) <=(raw/screan))&&(abs(movetoY-y) <=(raw/screan))) {
            if(moveTimer.passedtime(2)){
                movetoX = leftMargin+ rand() % raw ;
                movetoY = topMargin+ rand() % (column/2);
            }else{
                return;
            }
        }

        if(abs(movetoX-x) >raw/screan){
            if((movetoX - x)>=0){
                x+=speed;
            }else{
                x-=speed;
            }
        }
        if(abs(movetoY-y) >=raw/screan){
            if(movetoY - y>0){
                y +=speed;
            }else{
                y -=speed;
            }
        }



    }

    bool isDead() {
        return HP <= 0;
    }

    void shoot(vector<Bullet> &bullets, int time, const Bullet& bullet) {
        if (shootTimer.passedtime(time)) {
            bullets.push_back(bullet);
        }
    }
};


class PlayerFighter {
public:
    PlayerFighter(int hp, int attack) : HP(hp), Attack(attack) {}

    IMAGE picture;
    int x=0;
    int y=0;
    int HP=0;
    int Attack;

    bool isDead() {
        return HP <= 0;
    }
};

