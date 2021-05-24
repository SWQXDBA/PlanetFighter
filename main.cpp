#include <iostream>
#include <graphics.h>
#include <conio.h>
#include "model.h"
#include "vector"
#include <windows.h>
#include "iterator"
#include "Enemy.h"

using namespace std;
static int raw = 2000;
static int column = 1200;
static vector<Bullet> bullets;
static vector<Enemy> enemys;
static int maxBullets=20;
static int shootSpeed = 10;//玩家子弹发射速度 0-100 同时影响子弹飞行的速度
static int enemyFreshTime=5;//多久刷新一次敌人
static int enemyFreshCount=5;//每次刷新的敌人数量
static int enemyMoveSpeed= 100;//越大敌人移动得越慢
void flyBullets(vector<Bullet> &bs);
void showBullets(vector<Bullet> &bs);
void flushEnemy(Timer &t,vector<Enemy> &ems);
void EnemyMove(vector<Enemy> &e);
void clearEnemy();
static int flushTime = 3;
int main()
{
    // 初始化绘图窗口
    initgraph(2000, 1200);

    IMAGE PlayerFighter;//玩家飞机
    IMAGE playerBullet;//玩家子弹


    loadimage(&playerBullet,R"(C:\Users\SWQXDBA\CLionProjects\EasyXTest\icons\bullet.png)",100,100);//加载玩家子弹图片
    loadimage(&PlayerFighter,R"(C:\Users\SWQXDBA\CLionProjects\EasyXTest\icons\playerfighter.png)",100,100);//加载玩家飞机图片
    loadimage(NULL,R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",raw,column);//加载背景图


    Timer time;
    BeginBatchDraw();//开始批量绘图 用于缓存
   while(true){
       static int movetime=0;
       MOUSEMSG option;
       PeekMouseMsg( &option,true);

       if(option.uMsg==WM_MOUSEMOVE){
           movetime++;
           //重置计时器
           if(movetime>flushTime*100){
               cout<<"重置了"<<endl;
               movetime=0;
           }
           if(movetime%flushTime!=0){
               continue;
           }

           //无论如何，让子弹飞一会儿
           flyBullets(bullets);
           //敌人随机移动
           EnemyMove(enemys);
           //间隔一段频率把玩家子弹加入刷新队列中
           if(movetime!=0&&movetime%(flushTime*(100/shootSpeed))==0){
               bullets.push_back(getPlayerBullet(option.x-50,option.y-90,playerBullet));
           }

           if(_kbhit())        //键盘输入值时
           {
               int key;
               key = _getch();
               if(key == 32)
               {
                   bullets.push_back(getPlayerBullet(option.x-120,option.y-90,playerBullet));
                   bullets.push_back(getPlayerBullet(option.x+20,option.y-90,playerBullet));
               }

           }
           //刷新敌人
           flushEnemy(time,enemys);

           //////////////////////////////////////////////////////////////////开始绘制//////////////////////////////////////////
           cleardevice();//清理之前的内容
           //加载背景图
           loadimage(NULL,R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",2000,1200);


           //加载玩家飞机
           putimage(option.x-50,option.y-50,&PlayerFighter);

           clearEnemy();
           //加载敌人飞机
           for(auto i=enemys.begin();i<enemys.end();i++){
               putimage(i->x,i->y,&i->picture);
           }

           //遍历容器 加载所有弹道
           showBullets(bullets);
           //执行未完成的绘制任务
           FlushBatchDraw();
           continue;
       }
       if(option.uMsg==WM_MBUTTONDOWN){
        return 0;

       }
   }
    closegraph();
}

void flyBullets(vector<Bullet> &bs){
    if(bs.empty())
        return;
    for(auto i=bs.begin();i<bs.end();i++){
        i->x+=(i->xmove*(1+shootSpeed/3));
        i->y+=(i->ymove*(1+shootSpeed/3));
    }
    for(auto i=bs.begin();i<bs.end();i++){
        if(i->x<0||i->y<0){
          i= bs.erase(i);
          if(i>bs.begin()&&!bs.empty()){
              i--;
          }else{
              break;
          }
        }
    }
}
void showBullets(vector<Bullet> &bs){
    for(auto i=bs.begin();i<bs.end();i++){
        putimage(i->x,i->y,&i->picture);
    }
}
void flushEnemy(Timer &t,vector<Enemy> &ems){
    if(t.passedtime(enemyFreshTime)){
        for(int i=0;i<enemyFreshCount;i++){
            IMAGE enemyFighter;//敌人飞机
            loadimage(&enemyFighter,R"(C:\Users\SWQXDBA\CLionProjects\EasyXTest\icons\enemyfighter.png)",100,100);//加载敌人飞机
            int X = 200+rand()%1600;
            int Y = 150+rand()%500;
            Enemy n= Enemy();
            n.x=X;
            n.y=Y;
            n.HP=10;
            n.speed=enemyMoveSpeed;
            n.picture = enemyFighter;
            ems.push_back(n);
        }
    }
}
void EnemyMove(vector<Enemy> &e){
    for(auto i = e.begin();i<e.end();i++) {
        i->move();
    }
}
void clearEnemy(){
    for(auto j=bullets.begin();j<bullets.end();j++){
        for(auto i=enemys.begin();i<enemys.end();i++){
            //判断撞击
            if(abs(j->x-i->x) <100&&abs(j->y-i->y) <100){
                //进行扣血
                i->HP-=j->ATTACK;
                if(i->HP<=0){
                    //如果没血了 删除
                    i=enemys.erase(i);
                    if(i>enemys.begin()&&enemys.size()>0){
                        i--;
                    }else{
                        return;
                    }

                }
                j=bullets.erase(j);
                if(j>bullets.begin()&&!bullets.empty())
                    j--;
                else
                    return;




            }
        }
    }

}