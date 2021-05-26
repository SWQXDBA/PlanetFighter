#include <iostream>
#include <graphics.h>
#include <conio.h>
#include "vector"
#include <windows.h>
#include "iterator"
#include "model.h"
#include "sstream"
using namespace std;
static string path = "C:\\Users\\SWQXDBA\\CLionProjects\\EasyXTest\\icons";
static int raw = 2000;
static int column = 1000;
static int screan = 20;// raw/screan��ΪͼƬ�Ĵ�С

//������߾�
static int leftMargin =screan*5;
static int rightMargin =screan*5;
static int topMargin =screan*5;
static int bottomMargin =screan*5;


static vector<Bullet> bullets;
static vector<Enemy> enemys;
static int maxBullets = 20;
static int shootSpeed = 1;//����ӵ������ٶ� 0-100 ͬʱӰ���ӵ����е��ٶ�
static int enemyFreshTime = 5;//���ˢ��һ�ε���
static int enemyFreshCount = 1;//ÿ��ˢ�µĵ�������
static int enemyMoveSpeed = 1;//Խ������ƶ���Խ��




void flyBullets(vector<Bullet> &bs);//��ѭ���иı��ӵ���λ��
void showBullets(vector<Bullet> &bs);//��ʾ�ӵ�
void flushEnemy(Timer &t, vector<Enemy> &ems);//ˢ���µĵ��� ���ú�һ����ˢ�� ��Ҫ����enemyFreshTime��ʱ��
void EnemyMove(vector<Enemy> &e);//��ѭ���иı���˵�λ��
bool  clearEnemy(PlayerFighter &playerFighter);//��ѭ�����ж���Щ���˸ñ���ɱ���������ܵ��˺� ͬʱ��������ܵ����˺� ���ûhpʱ����false
static int flushTime = 3;//���ڿ������������ٶȡ�ÿ����flushTime�κ����Ż�ִ��һ�� ����3��ʱ�����������ק�С�
int main() {
    // ��ʼ����ͼ����
    initgraph(raw+leftMargin+rightMargin, column+topMargin+bottomMargin);
    BulletFactory bulletFactory;
    PlayerFighter playerFighter(100,3);
    loadimage(&playerFighter.picture, (path + "\\playerfighter.png").c_str(), raw / screan,
              raw/screan);//������ҷɻ�ͼƬ

    //���ر���ͼ
    IMAGE B;

    loadimage(&B,
              R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",
              raw, column);
    putimage(leftMargin,topMargin, &B);




    Timer time;
    BeginBatchDraw();//��ʼ������ͼ ���ڻ���
    while (true) {
        static int movetime = 0;
        //��������ź�
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

    //��ҷɻ���λ�� �����Ե���
        if(option.x-raw/screan/2<=leftMargin)
            playerFighter.x=leftMargin;
        else if(option.x-raw/screan/2>=leftMargin+raw-raw/screan)
            playerFighter.x=leftMargin+raw-raw/screan;
        else
            playerFighter.x=option.x - (raw/screan/2);

        if(option.y-raw/screan/2<=topMargin)
            playerFighter.y=topMargin;
        else if(option.y-raw/screan/2>topMargin+column-raw/screan)
            playerFighter.y=topMargin+column-raw/screan;
        else
        playerFighter.y=option.y - (raw/screan/2);


        //������Σ����ӵ���һ���
        flyBullets(bullets);
        //��������ƶ�
        EnemyMove(enemys);
        //���һ��Ƶ�ʰ�����ӵ�����ˢ�¶�����
        if (movetime != 0 && movetime % (flushTime * (100 / shootSpeed)) == 0) {
            bullets.push_back(bulletFactory.getPlayerBullet(playerFighter.x, playerFighter.y - raw/screan, 0));
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
       if(!clearEnemy(playerFighter)){
           cout<<"game over!"<<endl;
           return 0;
       }
        //////////////////////////////////////////////////////////////////��ʼ����//////////////////////////////////////////
        cleardevice();//����֮ǰ������
        loadimage(&B,
                  R"(C:\Users\SWQXDBA\AppData\Roaming\SpaceEngineers\Mods\cesha\icons\1111111111111111111111111.png)",
                  raw, column);
        putimage(leftMargin,topMargin, &B);


        //������ҷɻ�
        putimage(playerFighter.x, playerFighter.y, &playerFighter.picture);


        //���ص��˷ɻ�
        for (auto i = enemys.begin(); i < enemys.end(); i++) {
            putimage(i->x, i->y, &i->picture);
        }

//���ʣ��Ѫ��
        settextcolor( 0x0000AA);
        settextstyle(2*screan, 0, _T("Consolas"));
        stringstream ss;
        ss<<playerFighter.HP;
        string str;
        ss>>str;
        outtextxy(screan, topMargin, str.c_str());


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
        if (i->x<leftMargin||i->x>leftMargin+raw || i->y < topMargin||i->y>topMargin+column- raw/screan) {
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
            int X = leftMargin+ rand() % raw;
            int Y = topMargin+ rand() % (column/2);
            Enemy n = Enemy();
            n.x = X;
            n.y = Y;
            n.HP = 10;
            n.movetoX = leftMargin+ rand() % raw ;
            n.movetoY = topMargin+ rand() % (column/2);
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

bool clearEnemy(PlayerFighter &playerFighter) {

    for (auto j = bullets.begin(); j < bullets.end(); j++) {
        if(j->from==0){
            for (auto i = enemys.begin(); i < enemys.end(); i++) {
                //�ж�ײ��
                if (abs(j->x - i->x) < raw/screan && abs(j->y - i->y) < raw/screan) {
                    //���п�Ѫ
                    i->HP -= j->ATTACK;
                    if (i->isDead()) {
                        //���ûѪ�� ɾ��
                        i = enemys.erase(i);
                        if (i > enemys.begin() && !enemys.empty()) {
                            i--;
                        } else {
                            break;
                        }
                    }
                    j = bullets.erase(j);
                    //Ҫɾ����ײ�����ӵ�
                    if (j > bullets.begin() && !bullets.empty()){
                        j--;
                    }
                    else{
                        return true;
                    }

                }
            }
        }else{
            if (abs(j->x - playerFighter.x) < raw/screan && abs(j->y - playerFighter.y) < raw/screan){
                playerFighter.HP-=j->ATTACK;
                //Ҫɾ����ײ�����ӵ�
                j=bullets.erase(j);
                if(j>bullets.begin()&&!bullets.empty()){
                    j--;
                }else{
                    //������һ����������� �򷵻�false
                    if(playerFighter.isDead()){
                        return false;
                    }
                    return true;
                }
            }
            //�ж���һ���û��Ѫ
            if(playerFighter.isDead()){
               return false;
            }
        }
    }
        return true;
}