#include <iostream>
#include <graphics.h>
#include <conio.h>
#include "vector"
#include <windows.h>
#include "iterator"
#include "model.h"
#include "sstream"
#include "checkpoint.h"

#include <direct.h>

using namespace std;
static string MainPath = "";
static int raw = 1000;
static int column = 750;
static int screan = 20;// raw/screan��ΪͼƬ�Ĵ�С

//������߾�
static int leftMargin = screan * 10;
static int rightMargin = screan * 5 * 0;
static int topMargin = screan * 5 * 0;
static int bottomMargin = screan * 5 * 0;

static vector<IMAGE> bulletImages;//�����ӵ���ͼ��
static vector<IMAGE> enemyEmages;//���е��˵�ͼ��
static vector<Bullet> bullets;//��ǰ��Ϸ�д��ڵ������ӵ�
static vector<Enemy> enemys;//��ǰ��Ϸ�д��ڵ����е���
static EnemyFactory *enemyFactory;
static int shootSpeed = 7;//����ӵ������ٶ� 0-100 ͬʱӰ���ӵ����е��ٶ�
static int enemyMoveSpeed = 2;//Խ������ƶ���Խ�죬ͬʱӰ������ӵ����е��ٶ�



inline void showDetails(int x, int y, int cnt, string message);

inline void showDetails(int x, int y, double cnt, string message);

void flyBullets(vector<Bullet> &bs);//��ѭ���иı��ӵ���λ��
void showBullets(vector<Bullet> &bs);//��ʾ�ӵ�
bool flushEnemy(Timer &t, vector<Enemy> &ems, Checkpoint &checkpoint);//ˢ���µĵ��� false:��Ϸ��δ���� true:��һ�ʤ
void EnemyMove(vector<Enemy> &e);//��ѭ���иı���˵�λ��
bool clearEnemy(Checkpoint &checkpoint);//��ѭ�����ж���Щ���˸ñ���ɱ���������ܵ��˺� ͬʱ��������ܵ����˺� ���ûhpʱ����false
int menu();//��ʾ�˵�
bool run(Checkpoint &checkpoint);//������
static int flushTime = 1000 / 100;//���ڿ������������ٶȡ���λ����
static int screanflushTime = 1000 / 90;//֡������ ��λ����




void init() {
    //��ʼ������·��
    char *buffer;
    //Ҳ���Խ�buffer��Ϊ�������
    if ((buffer = getcwd(nullptr, 0)) == nullptr) {
        perror("getcwd error");
    }

    stringstream ss;
    ss << buffer;
    ss << "\\icons";
    ss >> MainPath;


//��ʼ��������
    IMAGE pi;
    loadimage(&pi, (MainPath + "\\bullet\\b1.png").c_str(), raw / screan, raw / screan);
    bulletImages.push_back(pi);
    loadimage(&pi, (MainPath + "\\bullet\\b2.png").c_str(), raw / screan, raw / screan);
    bulletImages.push_back(pi);
    loadimage(&pi, (MainPath + "\\bullet\\b3.png").c_str(), raw / screan, raw / screan);
    bulletImages.push_back(pi);
    enemyFactory = new EnemyFactory();
}

int main() {
    init();
    // ��ʼ����ͼ����
    initgraph(raw + leftMargin + rightMargin, column + topMargin + bottomMargin);
    BeginBatchDraw();//��ʼ������ͼ ���ڻ���
    while (true) {

        Checkpoint checkpoint(menu(), *enemyFactory);
        if (run(checkpoint)) {
            cleardevice();//����֮ǰ������
            settextcolor(0x0000AA);
            settextstyle(screan * 2, 0, _T("Consolas"));
            outtextxy(leftMargin, column / 2, "��ϲ��ʤ ���¿ո������Ϸ��esc�ر���Ϸ");
            FlushBatchDraw();
        } else {
            cleardevice();//����֮ǰ������
            settextcolor(0x0000AA);
            settextstyle(screan * 2, 0, _T("Consolas"));
            outtextxy(leftMargin, column / 2, "��ʧ����! ���¿ո������Ϸ��esc�ر���Ϸ");
            FlushBatchDraw();
        }
        while (true) {
            if (_kbhit())        //��������ֵʱ
            {
                int key;
                key = _getch();
                //�ո�
                if (key == 32) {
                    break;
                }
                //esc
                if (key == 27) {
                    closegraph();
                    return 0;
                }
            }
        }

        //������һ�ѵ�����
        enemys.clear();
        bullets.clear();

    }


}

bool run(Checkpoint &checkpoint) {


    //���ر���ͼ
    IMAGE B;
    loadimage(&B,
              (MainPath + "\\background1.png").c_str(),
              raw, column);
    putimage(leftMargin, topMargin, &B);


    Timer time;



//���߼�ѭ��

    while (true) {
        //��������ʱ��
        static Timer mainTime;

        //��������ź�
        static MOUSEMSG option;
        while (MouseHit()) {
            PeekMouseMsg(&option, true);
        }


        static int runSpeed;
        static clock_t passed = 0;
        static clock_t now = 0;

// ��ʼˢ��ϵͳ
        if (mainTime.passedtime(flushTime)) {
            //�ж���Ϸ�Ƿ������
            if (enemys.empty() && checkpoint.nowTime == checkpoint.cd.size())
                return true;

            //��������Ƶ��
            now = mainTime.getPassedTime();
            runSpeed = 1000 / (now - passed);
            passed = mainTime.getPassedTime();


            //��ҷɻ���λ�� �����Ե���
            if (option.x - raw / screan / 2 <= leftMargin)
                checkpoint.playerFighter.x = leftMargin;
            else if (option.x - raw / screan / 2 >= leftMargin + raw - raw / screan)
                checkpoint.playerFighter.x = leftMargin + raw - raw / screan;
            else
                checkpoint.playerFighter.x = option.x - (raw / screan / 2);

            if (option.y - raw / screan / 2 <= topMargin)
                checkpoint.playerFighter.y = topMargin;
            else if (option.y - raw / screan / 2 > topMargin + column - raw / screan)
                checkpoint.playerFighter.y = topMargin + column - raw / screan;
            else
                checkpoint.playerFighter.y = option.y - (raw / screan / 2);


            //������Σ����ӵ���һ���
            flyBullets(bullets);
            //��������ƶ�
            EnemyMove(enemys);
            //���ӵ�����ˢ�¶�����
            static Timer shootTimer;
            if (shootTimer.passedtime(1500 / shootSpeed)) {
                bullets.emplace_back(0, bulletImages[1], checkpoint.playerFighter.x,
                                         checkpoint.playerFighter.y - raw / screan, 0, -5, 5,
                                         checkpoint.playerFighter.Attack);
                for (auto i = enemys.begin(); i < enemys.end(); i++) {
                    i->shoot(bullets, 1000);

                }
            }
//
//            if (_kbhit())        //��������ֵʱ
//            {
//                int key;
//                key = _getch();
//                //�ո�
//                if (key == 32) {
//                }
//                //esc
//                if (key == 27) {
//                    closegraph();
//                    break;
//                }
//            }

            //ˢ�µ���
            if (flushEnemy(time, enemys, checkpoint)) {
                return true;
            }
            //�������
            if (!clearEnemy(checkpoint)) {
                return false;
            }
        }

        //////////////////////////////////////////////////////////////////��ʼ����//////////////////////////////////////////
//����֡��
        static Timer mainScreanFlushTimer;
        static int screanSpeed;
        static clock_t screanpassed = 0;
        static clock_t screannow = 0;


        if (mainScreanFlushTimer.passedtime(screanflushTime)) {
            screannow = mainScreanFlushTimer.getPassedTime();
            screanSpeed = 1000 / (screannow - screanpassed);
            screanpassed = mainScreanFlushTimer.getPassedTime();


            cleardevice();//����֮ǰ������
            putimage(leftMargin, topMargin, &checkpoint.background);


            //������ҷɻ�
            putimage(checkpoint.playerFighter.x, checkpoint.playerFighter.y, &checkpoint.playerFighter.picture);


            //���ص��˷ɻ�
            for (auto i = enemys.begin(); i < enemys.end(); i++) {
                putimage(i->x, i->y, &i->picture);
            }

//���ʣ��Ѫ��
            settextcolor(0x0000AA);
            settextstyle(screan * 1.5, 0, _T("Consolas"));
            showDetails(screan, topMargin, checkpoint.playerFighter.HP, "HP:");
            //�������Ƶ��
            showDetails(screan, topMargin + screan * 2, (double) runSpeed / (1000 / flushTime), "ģ������:");
            //�������Ƶ��
            showDetails(screan, topMargin + screan * 4, runSpeed, "����Ƶ��:");
            //���֡��
            showDetails(screan, topMargin + screan * 6, screanSpeed, "֡��:");
            //�������� �������е���
            showBullets(bullets);
            //ִ��δ��ɵĻ�������
            FlushBatchDraw();

        }
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
        if (i->x < leftMargin || i->x > leftMargin + raw || i->y < topMargin ||
            i->y > topMargin + column - raw / screan) {
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

bool flushEnemy(Timer &t, vector<Enemy> &ems, Checkpoint &checkpoint) {
//�ؿ��е�ˢ���鶼ˢ�¹���
    if (checkpoint.nowTime >= checkpoint.cd.size()) {
        //�ؿ��еĵ��˶�û��
        if (enemys.empty()) {
            return true;
        }
        return false;
    }

    if (t.passedtime(checkpoint.cd[checkpoint.nowTime] * 1000)) {

        for (auto i = checkpoint.enemyGroup[checkpoint.nowTime].begin();
             i < checkpoint.enemyGroup[checkpoint.nowTime].end();
             i++) {
            enemys.push_back(*i);
        }
        checkpoint.nowTime++;
    }
    return false;
}

void EnemyMove(vector<Enemy> &e) {
    for (auto i = e.begin(); i < e.end(); i++) {
        i->move();
    }
}

bool clearEnemy(Checkpoint &checkpoint) {

    for (auto j = bullets.begin(); j < bullets.end(); j++) {
        if (j->from == 0) {
            for (auto i = enemys.begin(); i < enemys.end(); i++) {
                //�ж�ײ��
                if (abs(j->x - i->x) < raw / screan && abs(j->y - i->y) < raw / screan) {
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
                    if (j > bullets.begin() && !bullets.empty()) {
                        j--;
                    } else {
                        return true;
                    }

                }
            }
        } else {
            if (abs(j->x - checkpoint.playerFighter.x) < raw / screan &&
                abs(j->y - checkpoint.playerFighter.y) < raw / screan) {
                checkpoint.playerFighter.HP -= j->ATTACK;
                //Ҫɾ����ײ�����ӵ�
                j = bullets.erase(j);
                if (j > bullets.begin() && !bullets.empty()) {
                    j--;
                } else {
                    //������һ����������� �򷵻�false
                    if (checkpoint.playerFighter.isDead()) {
                        return false;
                    }
                    return true;
                }
            }
            //�ж���һ���û��Ѫ
            if (checkpoint.playerFighter.isDead()) {
                return false;
            }
        }
    }
    return true;
}

inline void showDetails(int x, int y, int cnt, string message) {
    stringstream ss;
    string str;
    ss << message;
    ss << cnt;
    ss >> str;
    //���ʣ��Ѫ��
    outtextxy(x, y, str.c_str());
}

inline void showDetails(int x, int y, double cnt, string message) {
    stringstream ss;
    string str;
    ss << message;
    ss << cnt;
    ss >> str;
    //���ʣ��Ѫ��
    outtextxy(x, y, str.c_str());
}

int menu() {
    cleardevice();//����֮ǰ������
    settextcolor(0x0000AA);
    settextstyle(screan * 2, 0, _T("Consolas"));
    outtextxy(raw / 2, topMargin, "��ѡ��ؿ�");

    outtextxy(raw / 2 - 10 * screan, topMargin + 3 * screan, "�ؿ�1");
    outtextxy(raw / 2, topMargin + 3 * screan, "�ؿ�2");
    outtextxy(raw / 2 + 10 * screan, topMargin + 3 * screan, "�ؿ�3");
    FlushBatchDraw();
    MOUSEMSG option;
    while (true) {
        PeekMouseMsg(&option, true);
        if (option.uMsg == WM_LBUTTONUP) {
            if (option.x >= raw / 2 - 10 * screan && option.x <= raw / 2 && option.y >= topMargin + 3 * screan &&
                option.y <= topMargin + 5 * screan)
                return 1;
            else if (option.x > raw / 2 && option.x <= raw / 2 + 10 * screan && option.y >= topMargin + 3 * screan &&
                     option.y <= topMargin + 5 * screan)
                return 2;
            else if (option.x > raw / 2 + 10 * screan && option.x <= raw / 2 + 20 * screan &&
                     option.y >= topMargin + 3 * screan && option.y <= topMargin + 5 * screan)
                return 3;
        }
    }

}