//
// Created by SWQXDBA2 on 2021/5/27.
//

#ifndef EASYXTEST_CHECKPOINT_H
#define EASYXTEST_CHECKPOINT_H

#endif //EASYXTEST_CHECKPOINT_H

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

class Checkpoint {
public:
    PlayerFighter playerFighter;//玩家飞机
    IMAGE background;//关卡背景图
    vector<vector<Enemy>> enemyGroup;//敌人刷新组
    vector<int> cd;//每一组刷新的cd
    int nowTime = 0;//现在刷新到第几组了

    Checkpoint(int c, EnemyFactory &enemyFactory) {
        if (c == 1) {
            //生成玩家飞机
            playerFighter = PlayerFighter(100, 3);
            loadimage(&playerFighter.picture, (MainPath + "\\playerfighter.png").c_str(), raw / screan,
                      raw / screan);//加载玩家飞机图片

//关卡背景图片
            loadimage(&background,
                      (MainPath + "\\background\\background1.png").c_str(),
                      raw, column);
            //设置刷新组 参数为间隔多少秒刷新这一组（从上一组开始）
            cd.push_back(0);
            cd.push_back(5);
            cd.push_back(7);
            cd.push_back(15);
            vector<Enemy> es;
            es.push_back(enemyFactory.getEnemy(0));
            es.push_back(enemyFactory.getEnemy(0));
            enemyGroup.push_back(es);

            vector<Enemy> es1;
            es1.push_back(enemyFactory.getEnemy(0));
            es1.push_back(enemyFactory.getEnemy(0));
            es1.push_back(enemyFactory.getEnemy(0));
            enemyGroup.push_back(es1);

            vector<Enemy> es2;
            es2.push_back(enemyFactory.getEnemy(0));
            es2.push_back(enemyFactory.getEnemy(0));
            es2.push_back(enemyFactory.getEnemy(1));
            enemyGroup.push_back(es2);

            vector<Enemy> es3;
            es3.push_back(enemyFactory.getEnemy(0));
            es3.push_back(enemyFactory.getEnemy(1));
            es3.push_back(enemyFactory.getEnemy(1));
            enemyGroup.push_back(es3);
        } else if (c == 2) {
            //生成玩家飞机
            playerFighter = PlayerFighter(150, 4);
            loadimage(&playerFighter.picture, (MainPath + "\\playerfighter.png").c_str(), raw / screan,
                      raw / screan);//加载玩家飞机图片

//关卡背景图片
            loadimage(&background,
                      (MainPath + "\\background\\background2.png").c_str(),
                      raw, column);
            //设置刷新组 参数为间隔多少秒刷新这一组（从上一组开始）
            cd.push_back(0);
            cd.push_back(7);
            cd.push_back(12);
            cd.push_back(16);

            vector<Enemy> es;
            es.push_back(enemyFactory.getEnemy(0));
            es.push_back(enemyFactory.getEnemy(0));
            es.push_back(enemyFactory.getEnemy(0));
            enemyGroup.push_back(es);

            vector<Enemy> es1;
            es1.push_back(enemyFactory.getEnemy(1));
            es1.push_back(enemyFactory.getEnemy(1));
            es1.push_back(enemyFactory.getEnemy(0));
            es1.push_back(enemyFactory.getEnemy(0));

            enemyGroup.push_back(es1);

            vector<Enemy> es2;
            es2.push_back(enemyFactory.getEnemy(1));
            es2.push_back(enemyFactory.getEnemy(1));
            es2.push_back(enemyFactory.getEnemy(1));
            enemyGroup.push_back(es2);

            vector<Enemy> es3;
            es3.push_back(enemyFactory.getEnemy(1));
            es3.push_back(enemyFactory.getEnemy(0));
            es3.push_back(enemyFactory.getEnemy(1));
            es3.push_back(enemyFactory.getEnemy(0));
            es3.push_back(enemyFactory.getEnemy(1));
            enemyGroup.push_back(es3);
        } else if (c == 3) {
            //生成玩家飞机
            playerFighter = PlayerFighter(200, 9);
            loadimage(&playerFighter.picture, (MainPath + "\\playerfighter.png").c_str(), raw / screan,
                      raw / screan);//加载玩家飞机图片
            loadimage(&background,
                      (MainPath + "\\background\\background3.png").c_str(),
                      raw, column);
            cd.push_back(0);
            cd.push_back(5);
            cd.push_back(5);
            cd.push_back(7);
            cd.push_back(10);

            vector<Enemy> es;
            es.push_back(enemyFactory.getEnemy(0));
            es.push_back(enemyFactory.getEnemy(0));
            enemyGroup.push_back(es);

            vector<Enemy> es1;
            es1.push_back(enemyFactory.getEnemy(0));
            es1.push_back(enemyFactory.getEnemy(1));
            es1.push_back(enemyFactory.getEnemy(1));
            enemyGroup.push_back(es1);

            vector<Enemy> es2;
            es2.push_back(enemyFactory.getEnemy(1));
            es2.push_back(enemyFactory.getEnemy(1));

            enemyGroup.push_back(es2);

            vector<Enemy> es3;

            es3.push_back(enemyFactory.getEnemy(1));
            es3.push_back(enemyFactory.getEnemy(1));
            es3.push_back(enemyFactory.getEnemy(1));
            enemyGroup.push_back(es3);

            vector<Enemy> es4;
            es4.push_back(enemyFactory.getEnemy(1));
            es4.push_back(enemyFactory.getEnemy(1));
            es4.push_back(enemyFactory.getEnemy(1));
            es4.push_back(enemyFactory.getEnemy(1));
            es4.push_back(enemyFactory.getEnemy(2, 100, 20));
            enemyGroup.push_back(es4);
        }
    }


};