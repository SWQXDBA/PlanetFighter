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
static int screan = 20;// raw/screan��ΪͼƬ�Ĵ�С
static vector<Bullet> bullets;
static vector<Enemy> enemys;
static int maxBullets = 20;
static int shootSpeed = 1;//����ӵ������ٶ� 0-100 ͬʱӰ���ӵ����е��ٶ�
static int enemyFreshTime = 3;//���ˢ��һ�ε���
static int enemyFreshCount = 2;//ÿ��ˢ�µĵ�������
static int enemyMoveSpeed = 1;//Խ������ƶ���Խ��
void flyBullets(vector<Bullet> &bs);//��ѭ���иı��ӵ���λ��
void showBullets(vector<Bullet> &bs);//��ʾ�ӵ�
void flushEnemy(Timer &t, vector<Enemy> &ems);//ˢ���µĵ��� ���ú�һ����ˢ�� ��Ҫ����enemyFreshTime��ʱ��
void EnemyMove(vector<Enemy> &e);//��ѭ���иı���˵�λ��
void clearEnemy();//��ѭ�����ж���Щ���˸ñ���ɱ���������ܵ��˺�
static int flushTime = 3;//���ڿ������������ٶȡ�ÿ����flushTime�κ����Ż�ִ��һ�� ����3��ʱ�����������ק�С�
int main() {
    // ��ʼ����ͼ����
    initgraph(raw, column);
    BulletFactory bulletFactory;
    IMAGE PlayerFighter;//��ҷɻ�

    loadimage(&PlayerFighter, (path+"\\playerfighter.png").c_str(), raw/screan,
              raw/screan);//������ҷɻ�ͼƬ
    loadimage(nullptr,
              R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)", raw,
              column);//���ر���ͼ


    Timer time;
    BeginBatchDraw();//��ʼ������ͼ ���ڻ���
    while (true) {
        static int movetime = 0;
        MOUSEMSG option;
        PeekMouseMsg(&option, true);
        movetime++;



        //���ü�ʱ�� ������ifʹ�ó��򲻻����ؽ���
        if (movetime > flushTime * 100) {
            movetime = 0;
        }
        if (movetime % flushTime != 0) {
            continue;
        }

        //������Σ����ӵ���һ���
        flyBullets(bullets);
        //��������ƶ�
        EnemyMove(enemys);
        //���һ��Ƶ�ʰ�����ӵ�����ˢ�¶�����
        if (movetime != 0 && movetime % (flushTime * (100 / shootSpeed)) == 0) {
            bullets.push_back(bulletFactory.getPlayerBullet(option.x - 50, option.y - 90, 0));
            for(auto i=enemys.begin();i<enemys.end();i++){
                int c1=bullets.size();
                i->shoot(bullets,1,bulletFactory.getEnemyBulletByType(i->x,i->y+50,0,2));
                int c2=bullets.size();
            }

        }

        if (_kbhit())        //��������ֵʱ
        {
            int key;
            key = _getch();
            //�ո�
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
        //ˢ�µ���
        flushEnemy(time, enemys);
        //�������
        clearEnemy();
        //////////////////////////////////////////////////////////////////��ʼ����//////////////////////////////////////////
        cleardevice();//����֮ǰ������
        //���ر���ͼ
        loadimage(nullptr,
                  R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",
                  raw, column);


        //������ҷɻ�
        putimage(option.x - 50, option.y - 50, &PlayerFighter);


        //���ص��˷ɻ�
        for (auto i = enemys.begin(); i < enemys.end(); i++) {
            putimage(i->x, i->y, &i->picture);
        }

        //�������� �������е���
        showBullets(bullets);
        //ִ��δ��ɵĻ�������
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
    //����������Ļ��Ե���ӵ�
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
            IMAGE enemyFighter;//���˷ɻ�
            loadimage(&enemyFighter, (path+"\\enemyfighter.png").c_str(), raw/screan,
                      raw/screan);//���ص��˷ɻ�
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
                //�ж�ײ��
                if (abs(j->x - i->x) < raw/screan && abs(j->y - i->y) < raw/screan) {
                    //���п�Ѫ
                    i->HP -= j->ATTACK;
                    if (i->HP <= 0) {
                        //���ûѪ�� ɾ��
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