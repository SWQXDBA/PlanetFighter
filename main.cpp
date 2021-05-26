#include <iostream>
#include <graphics.h>
#include <conio.h>
#include "vector"
#include <windows.h>
#include "iterator"
#include "model.h"
using namespace std;
static string path = "C:\\Users\\SWQXDBA2\\CLionProjects\\PlanetFighter\\icons";
static int raw = 1000;
static int column = 500;
static int screan = 20;// raw/screan作为图片的大小
static vector<Bullet> bullets;
static vector<Enemy> enemys;
static int maxBullets = 20;
static int shootSpeed = 1;//玩家子弹发射速度 0-100 同时影响子弹飞行的速度
static int enemyFreshTime = 3;//多久刷新一次敌人
static int enemyFreshCount = 2;//每次刷新的敌人数量
static int enemyMoveSpeed = 1;//越大敌人移动得越慢
void flyBullets(vector<Bullet> &bs);//在循环中改变子弹的位置
void showBullets(vector<Bullet> &bs);//显示子弹
void flushEnemy(Timer &t, vector<Enemy> &ems);//刷新新的敌人 调用后不一定会刷新 需要满足enemyFreshTime的时间
void EnemyMove(vector<Enemy> &e);//在循环中改变敌人的位置
void clearEnemy();//在循环中判断哪些敌人该被“杀死”或者受到伤害
static int flushTime = 3;//用于控制整体运行速度。每经过flushTime次后程序才会执行一次 低于3的时候会有明显拖拽感。
int main() {
    // 初始化绘图窗口
    initgraph(raw, column);
    BulletFactory bulletFactory;
    IMAGE PlayerFighter;//玩家飞机

    loadimage(&PlayerFighter, (path+"\\playerfighter.png").c_str(), raw/screan,
              raw/screan);//加载玩家飞机图片
    loadimage(nullptr,
              R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)", raw,
              column);//加载背景图


    Timer time;
    BeginBatchDraw();//开始批量绘图 用于缓存
    while (true) {
        static int movetime = 0;
        MOUSEMSG option;
        PeekMouseMsg(&option, true);
        movetime++;



        //重置计时器 这两个if使得程序不会过快地进行
        if (movetime > flushTime * 100) {
            movetime = 0;
        }
        if (movetime % flushTime != 0) {
            continue;
        }

        //无论如何，让子弹飞一会儿
        flyBullets(bullets);
        //敌人随机移动
        EnemyMove(enemys);
        //间隔一段频率把玩家子弹加入刷新队列中
        if (movetime != 0 && movetime % (flushTime * (100 / shootSpeed)) == 0) {
            bullets.push_back(bulletFactory.getPlayerBullet(option.x - 50, option.y - 90, 0));
            for(auto i=enemys.begin();i<enemys.end();i++){
                int c1=bullets.size();
                i->shoot(bullets,1,bulletFactory.getEnemyBulletByType(i->x,i->y+50,0,2));
                int c2=bullets.size();
            }

        }

        if (_kbhit())        //键盘输入值时
        {
            int key;
            key = _getch();
            //空格
            if (key == 32) {
                bullets.push_back(bulletFactory.getPlayerBullet(option.x - 120, option.y - 90, 0));
                bullets.push_back(bulletFactory.getPlayerBullet(option.x + 20, option.y - 90, 0));
            }
            //esc
            if (key == 27) {
                closegraph();
                break;
            }

        }
        //刷新敌人
        flushEnemy(time, enemys);
        //清理敌人
        clearEnemy();
        //////////////////////////////////////////////////////////////////开始绘制//////////////////////////////////////////
        cleardevice();//清理之前的内容
        //加载背景图
        loadimage(nullptr,
                  R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",
                  raw, column);


        //加载玩家飞机
        putimage(option.x - 50, option.y - 50, &PlayerFighter);


        //加载敌人飞机
        for (auto i = enemys.begin(); i < enemys.end(); i++) {
            putimage(i->x, i->y, &i->picture);
        }

        //遍历容器 加载所有弹道
        showBullets(bullets);
        //执行未完成的绘制任务
        FlushBatchDraw();
        continue;

    }
}

void flyBullets(vector<Bullet> &bs) {
    if (bs.empty())
        return;
    for (auto i = bs.begin(); i < bs.end(); i++) {
        i->bulletFly();
    }
    //清理触碰了屏幕边缘的子弹
    for (auto i = bs.begin(); i < bs.end(); i++) {
        if (i->x < 0 || i->y < 0) {
            i = bs.erase(i);
            if (i > bs.begin() && !bs.empty()) {
                i--;
            } else {
                break;
            }
        }
    }
}

void showBullets(vector<Bullet> &bs) {
    for (auto i = bs.begin(); i < bs.end(); i++) {
        putimage(i->x, i->y, &i->picture);
    }
}

void flushEnemy(Timer &t, vector<Enemy> &ems) {
    if (t.passedtime(enemyFreshTime)) {
        for (int i = 0; i < enemyFreshCount; i++) {
            IMAGE enemyFighter;//敌人飞机
            loadimage(&enemyFighter, (path+"\\enemyfighter.png").c_str(), raw/screan,
                      raw/screan);//加载敌人飞机
            int X = rand() % raw;
            int Y = rand() % (column/2);
            Enemy n = Enemy();
            n.x = X;
            n.y = Y;
            n.HP = 10;
            n.movetoX = rand() % raw ;
            n.movetoY = rand() % (column/2);
            n.speed = enemyMoveSpeed;
            n.picture = enemyFighter;
            ems.push_back(n);
        }
    }
}

void EnemyMove(vector<Enemy> &e) {
    for (auto i = e.begin(); i < e.end(); i++) {
        i->move();
    }
}

void clearEnemy() {
    for (auto j = bullets.begin(); j < bullets.end(); j++) {
        for (auto i = enemys.begin(); i < enemys.end(); i++) {
            if(j->from==0){
                //判断撞击
                if (abs(j->x - i->x) < raw/screan && abs(j->y - i->y) < raw/screan) {
                    //进行扣血
                    i->HP -= j->ATTACK;
                    if (i->HP <= 0) {
                        //如果没血了 删除
                        i = enemys.erase(i);
                        if (i > enemys.begin() && enemys.size() > 0) {
                            i--;
                        } else {
                            return;
                        }
                    }
                    j = bullets.erase(j);
                    if (j > bullets.begin() && !bullets.empty())
                        j--;
                    else
                        return;
                }
            }

        }
    }

}