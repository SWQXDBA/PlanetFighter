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
static int shootSpeed = 10;//����ӵ������ٶ� 0-100 ͬʱӰ���ӵ����е��ٶ�
static int enemyFreshTime=5;//���ˢ��һ�ε���
static int enemyFreshCount=5;//ÿ��ˢ�µĵ�������
static int enemyMoveSpeed= 100;//Խ������ƶ���Խ��
void flyBullets(vector<Bullet> &bs);
void showBullets(vector<Bullet> &bs);
void flushEnemy(Timer &t,vector<Enemy> &ems);
void EnemyMove(vector<Enemy> &e);
void clearEnemy();
static int flushTime = 3;
int main()
{
    // ��ʼ����ͼ����
    initgraph(2000, 1200);

    IMAGE PlayerFighter;//��ҷɻ�
    IMAGE playerBullet;//����ӵ�


    loadimage(&playerBullet,R"(C:\Users\SWQXDBA\CLionProjects\EasyXTest\icons\bullet.png)",100,100);//��������ӵ�ͼƬ
    loadimage(&PlayerFighter,R"(C:\Users\SWQXDBA\CLionProjects\EasyXTest\icons\playerfighter.png)",100,100);//������ҷɻ�ͼƬ
    loadimage(NULL,R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",raw,column);//���ر���ͼ


    Timer time;
    BeginBatchDraw();//��ʼ������ͼ ���ڻ���
   while(true){
       static int movetime=0;
       MOUSEMSG option;
       PeekMouseMsg( &option,true);

       if(option.uMsg==WM_MOUSEMOVE){
           movetime++;
           //���ü�ʱ��
           if(movetime>flushTime*100){
               cout<<"������"<<endl;
               movetime=0;
           }
           if(movetime%flushTime!=0){
               continue;
           }

           //������Σ����ӵ���һ���
           flyBullets(bullets);
           //��������ƶ�
           EnemyMove(enemys);
           //���һ��Ƶ�ʰ�����ӵ�����ˢ�¶�����
           if(movetime!=0&&movetime%(flushTime*(100/shootSpeed))==0){
               bullets.push_back(getPlayerBullet(option.x-50,option.y-90,playerBullet));
           }

           if(_kbhit())        //��������ֵʱ
           {
               int key;
               key = _getch();
               if(key == 32)
               {
                   bullets.push_back(getPlayerBullet(option.x-120,option.y-90,playerBullet));
                   bullets.push_back(getPlayerBullet(option.x+20,option.y-90,playerBullet));
               }

           }
           //ˢ�µ���
           flushEnemy(time,enemys);

           //////////////////////////////////////////////////////////////////��ʼ����//////////////////////////////////////////
           cleardevice();//����֮ǰ������
           //���ر���ͼ
           loadimage(NULL,R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",2000,1200);


           //������ҷɻ�
           putimage(option.x-50,option.y-50,&PlayerFighter);

           clearEnemy();
           //���ص��˷ɻ�
           for(auto i=enemys.begin();i<enemys.end();i++){
               putimage(i->x,i->y,&i->picture);
           }

           //�������� �������е���
           showBullets(bullets);
           //ִ��δ��ɵĻ�������
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
            IMAGE enemyFighter;//���˷ɻ�
            loadimage(&enemyFighter,R"(C:\Users\SWQXDBA\CLionProjects\EasyXTest\icons\enemyfighter.png)",100,100);//���ص��˷ɻ�
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
            //�ж�ײ��
            if(abs(j->x-i->x) <100&&abs(j->y-i->y) <100){
                //���п�Ѫ
                i->HP-=j->ATTACK;
                if(i->HP<=0){
                    //���ûѪ�� ɾ��
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