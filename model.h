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
#include<vector>
#include<cmath>

using namespace std;
extern vector<IMAGE> bulletImages;
extern vector<IMAGE> enemyEmages;
extern int raw;
extern int column;
extern string MainPath;
extern int shootSpeed;
extern int screan;

extern int leftMargin;
extern int rightMargin;
extern int topMargin;
extern int bottomMargin;
extern int enemyMoveSpeed;

class Timer {
    //定时器
    clock_t lasttime = 0;
    clock_t gettedlasttime = 0;
public:
    bool passedtime(int ms) {
        if (lasttime == 0) {
            lasttime = clock();
            return true;
        }

        clock_t nowTime = clock();
        if (nowTime - lasttime >= ms) {
            lasttime = nowTime;
            return true;
        }
        return false;
    }

    int getPassedTime() {
        clock_t now = clock();
        clock_t t = now - gettedlasttime;
        gettedlasttime = now;
        return t;
    }
};


class Bullet {
public:
    int from;//0 玩家发射的 1 敌人发射的
    IMAGE picture;
    int x;
    int y;
    int xmove;
    int ymove;
    int speed = 1;
    int ATTACK = 3;

    void bulletFly() {
        x += (xmove * (1 + speed / screan));
        y += (ymove * (1 + speed / screan));
    }

    Bullet(int from, const IMAGE &picture, int x, int y, int xmove, int ymove, int speed, int attack) : from(from),
                                                                                                        picture(picture),
                                                                                                        x(x), y(y),
                                                                                                        xmove(xmove),
                                                                                                        ymove(ymove),
                                                                                                        speed(speed),
                                                                                                        ATTACK(attack) {}
};


class Enemy {
public:
    Timer shootTimer;//用于判断是否到了一个可以发射新子弹的时间
    Timer moveTimer;//用于判断是否到可以移动的时间(每次移动到一个目的地的时候 会停留一会儿)
    IMAGE picture;
    int x;//当前在游戏中的坐标
    int y;//当前在游戏中的坐标
    int HP = 10;
    int Attack;//子弹的攻击力
    int movetoX;//当前想要前往的目标
    int movetoY;//当前想要前往的目标
    int speed = 20;//移动的速度
    vector<Bullet> myBullets;//每次发射的子弹

    void move() {
        //如果达到了目的地 则要选择下一个坐标
        if ((abs(movetoX - x) <= (raw / screan)) && (abs(movetoY - y) <= (raw / screan))) {
            //判断是否停留了足够的时间 2s
            if (moveTimer.passedtime(2000)) {
                movetoX = leftMargin + rand() % raw;
                movetoY = topMargin + rand() % (column / 2);
            } else {
                return;
            }
        }
        //移动x
        if (abs(movetoX - x) > raw / screan) {
            if ((movetoX - x) >= 0) {
                x += speed;
            } else {
                x -= speed;
            }
        }
        //移动y
        if (abs(movetoY - y) >= raw / screan) {
            if (movetoY - y > 0) {
                y += speed;
            } else {
                y -= speed;
            }
        }


    }

    bool isDead() {
        return HP <= 0;
    }
        //发射子弹
    void shoot(vector<Bullet> &bullets, int time) {
        if (shootTimer.passedtime(time)) {
            for (auto i = myBullets.begin(); i < myBullets.end(); i++) {
                Bullet b = *i;
                b.x = x;
                b.y = y;
                bullets.push_back(b);
            }
        }
    }
};

class EnemyFactory {
    vector<Enemy> All;
public:
    Enemy getEnemy(int loc) {
        return All[loc];
    }

    Enemy getEnemy(int loc, int hp, int attack) {
        Enemy e = All[loc];
        e.HP = hp;
        e.Attack = attack;
        return e;
    }

    EnemyFactory() {
//设置敌人种类1
        Enemy n = Enemy();
        loadimage(&n.picture, (MainPath + "\\enemy\\b1.png").c_str(), raw / screan,
                  raw / screan);//加载敌人飞机
        int X = leftMargin + rand() % raw;
        int Y = topMargin + rand() % (column / 2);
        n.x = X;
        n.y = Y;
        n.HP = 10;
        n.movetoX = leftMargin + rand() % raw;
        n.movetoY = topMargin + rand() % (column / 2);
        n.Attack = 3;
        n.speed = enemyMoveSpeed;
        Bullet b = Bullet(1, bulletImages[0], X, Y, 0, 5, n.speed, n.Attack);
        n.myBullets.push_back(b);
        All.push_back(n);


        //////

//设置敌人种类2
        loadimage(&n.picture, (MainPath + "\\enemy\\b2.png").c_str(), raw / screan,
                  raw / screan);//加载敌人飞机
        X = leftMargin + rand() % raw;
        Y = topMargin + rand() % (column / 2);
        n.x = X;
        n.y = Y;
        n.HP = 20;
        n.movetoX = leftMargin + rand() % raw;
        n.movetoY = topMargin + rand() % (column / 2);
        n.Attack = 10;
        n.speed = enemyMoveSpeed * 1.5;
        n.myBullets.clear();

        b = Bullet(1, bulletImages[0], X, Y, -2, 5, n.speed, n.Attack);

        n.myBullets.push_back(b);

        b = Bullet(1, bulletImages[0], X, Y, 2, 5, n.speed, n.Attack);
        n.myBullets.push_back(b);
        All.push_back(n);


        //设置敌人种类3
        //BOSS
        loadimage(&n.picture, (MainPath + "\\enemy\\b3.png").c_str(), raw / screan,
                  raw / screan);//加载敌人飞机
        X = leftMargin + rand() % raw;
        Y = topMargin + rand() % (column / 2);
        n.x = X;
        n.y = Y;
        n.HP = 20;
        n.movetoX = leftMargin + rand() % raw;
        n.movetoY = topMargin + rand() % (column / 2);
        n.Attack = 15;
        n.speed = enemyMoveSpeed * 2;
        n.myBullets.clear();
        b = Bullet(1, bulletImages[2], X, Y, -3, 7, n.speed, n.Attack);
        n.myBullets.push_back(b);
        b = Bullet(1, bulletImages[2], X, Y, 0, 7, n.speed, n.Attack);
        n.myBullets.push_back(b);
        b = Bullet(1, bulletImages[2], X, Y, 3, 7, n.speed, n.Attack);
        n.myBullets.push_back(b);
        All.push_back(n);
    }
};

class PlayerFighter {
public:
    PlayerFighter(int hp, int attack) : HP(hp), Attack(attack) {}

    PlayerFighter() {}

    IMAGE picture;
    int x = 0;
    int y = 0;
    int HP = 0;
    int Attack;

    bool isDead() {
        return HP <= 0;
    }
};

