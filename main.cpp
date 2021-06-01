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
static int screan = 20;// raw/screan作为图片的大小

//设置外边距
static int leftMargin = screan * 10;
static int rightMargin = screan * 5 * 0;
static int topMargin = screan * 5 * 0;
static int bottomMargin = screan * 5 * 0;

static vector<IMAGE> bulletImages;//所有子弹的图标
static vector<IMAGE> enemyEmages;//所有敌人的图标
static vector<Bullet> bullets;//当前游戏中存在的所有子弹
static vector<Enemy> enemys;//当前游戏中存在的所有敌人
static EnemyFactory *enemyFactory;
static int shootSpeed = 7;//玩家子弹发射速度 0-100 同时影响子弹飞行的速度
static int enemyMoveSpeed = 2;//越大敌人移动得越快，同时影响敌人子弹飞行的速度



inline void showDetails(int x, int y, int cnt, string message);

inline void showDetails(int x, int y, double cnt, string message);

void flyBullets(vector<Bullet> &bs);//在循环中改变子弹的位置
void showBullets(vector<Bullet> &bs);//显示子弹
bool flushEnemy(Timer &t, vector<Enemy> &ems, Checkpoint &checkpoint);//刷新新的敌人 false:游戏还未结束 true:玩家获胜
void EnemyMove(vector<Enemy> &e);//在循环中改变敌人的位置
bool clearEnemy(Checkpoint &checkpoint);//在循环中判断哪些敌人该被“杀死”或者受到伤害 同时清算玩家受到的伤害 玩家没hp时返回false
int menu();//显示菜单
bool run(Checkpoint &checkpoint);//主程序
static int flushTime = 1000 / 100;//用于控制整体运行速度。单位毫秒
static int screanflushTime = 1000 / 90;//帧数上限 单位毫秒




void init() {
    //初始化工作路径
    char *buffer;
    //也可以将buffer作为输出参数
    if ((buffer = getcwd(nullptr, 0)) == nullptr) {
        perror("getcwd error");
    }

    stringstream ss;
    ss << buffer;
    ss << "\\icons";
    ss >> MainPath;


//初始化弹道组
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
    // 初始化绘图窗口
    initgraph(raw + leftMargin + rightMargin, column + topMargin + bottomMargin);
    BeginBatchDraw();//开始批量绘图 用于缓存
    while (true) {

        Checkpoint checkpoint(menu(), *enemyFactory);
        if (run(checkpoint)) {
            cleardevice();//清理之前的内容
            settextcolor(0x0000AA);
            settextstyle(screan * 2, 0, _T("Consolas"));
            outtextxy(leftMargin, column / 2, "恭喜获胜 按下空格继续游戏，esc关闭游戏");
            FlushBatchDraw();
        } else {
            cleardevice();//清理之前的内容
            settextcolor(0x0000AA);
            settextstyle(screan * 2, 0, _T("Consolas"));
            outtextxy(leftMargin, column / 2, "您失败了! 按下空格继续游戏，esc关闭游戏");
            FlushBatchDraw();
        }
        while (true) {
            if (_kbhit())        //键盘输入值时
            {
                int key;
                key = _getch();
                //空格
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

        //清理上一把的内容
        enemys.clear();
        bullets.clear();

    }


}

bool run(Checkpoint &checkpoint) {


    //加载背景图
    IMAGE B;
    loadimage(&B,
              (MainPath + "\\background1.png").c_str(),
              raw, column);
    putimage(leftMargin, topMargin, &B);


    Timer time;



//主逻辑循环

    while (true) {
        //程序主计时器
        static Timer mainTime;

        //接收鼠标信号
        static MOUSEMSG option;
        while (MouseHit()) {
            PeekMouseMsg(&option, true);
        }


        static int runSpeed;
        static clock_t passed = 0;
        static clock_t now = 0;

// 开始刷新系统
        if (mainTime.passedtime(flushTime)) {
            //判断游戏是否结束了
            if (enemys.empty() && checkpoint.nowTime == checkpoint.cd.size())
                return true;

            //计算运行频率
            now = mainTime.getPassedTime();
            runSpeed = 1000 / (now - passed);
            passed = mainTime.getPassedTime();


            //玩家飞机的位置 处理边缘情况
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


            //无论如何，让子弹飞一会儿
            flyBullets(bullets);
            //敌人随机移动
            EnemyMove(enemys);
            //把子弹加入刷新队列中
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
//            if (_kbhit())        //键盘输入值时
//            {
//                int key;
//                key = _getch();
//                //空格
//                if (key == 32) {
//                }
//                //esc
//                if (key == 27) {
//                    closegraph();
//                    break;
//                }
//            }

            //刷新敌人
            if (flushEnemy(time, enemys, checkpoint)) {
                return true;
            }
            //清理敌人
            if (!clearEnemy(checkpoint)) {
                return false;
            }
        }

        //////////////////////////////////////////////////////////////////开始绘制//////////////////////////////////////////
//计算帧数
        static Timer mainScreanFlushTimer;
        static int screanSpeed;
        static clock_t screanpassed = 0;
        static clock_t screannow = 0;


        if (mainScreanFlushTimer.passedtime(screanflushTime)) {
            screannow = mainScreanFlushTimer.getPassedTime();
            screanSpeed = 1000 / (screannow - screanpassed);
            screanpassed = mainScreanFlushTimer.getPassedTime();


            cleardevice();//清理之前的内容
            putimage(leftMargin, topMargin, &checkpoint.background);


            //加载玩家飞机
            putimage(checkpoint.playerFighter.x, checkpoint.playerFighter.y, &checkpoint.playerFighter.picture);


            //加载敌人飞机
            for (auto i = enemys.begin(); i < enemys.end(); i++) {
                putimage(i->x, i->y, &i->picture);
            }

//输出剩余血量
            settextcolor(0x0000AA);
            settextstyle(screan * 1.5, 0, _T("Consolas"));
            showDetails(screan, topMargin, checkpoint.playerFighter.HP, "HP:");
            //输出运行频率
            showDetails(screan, topMargin + screan * 2, (double) runSpeed / (1000 / flushTime), "模拟速率:");
            //输出运行频率
            showDetails(screan, topMargin + screan * 4, runSpeed, "运行频率:");
            //输出帧数
            showDetails(screan, topMargin + screan * 6, screanSpeed, "帧数:");
            //遍历容器 加载所有弹道
            showBullets(bullets);
            //执行未完成的绘制任务
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
    //清理触碰了屏幕边缘的子弹
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
//关卡中的刷新组都刷新过了
    if (checkpoint.nowTime >= checkpoint.cd.size()) {
        //关卡中的敌人都没了
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
                //判断撞击
                if (abs(j->x - i->x) < raw / screan && abs(j->y - i->y) < raw / screan) {
                    //进行扣血
                    i->HP -= j->ATTACK;
                    if (i->isDead()) {
                        //如果没血了 删除
                        i = enemys.erase(i);
                        if (i > enemys.begin() && !enemys.empty()) {
                            i--;
                        } else {
                            break;
                        }
                    }
                    j = bullets.erase(j);
                    //要删除碰撞过的子弹
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
                //要删除碰撞过的子弹
                j = bullets.erase(j);
                if (j > bullets.begin() && !bullets.empty()) {
                    j--;
                } else {
                    //如果最后一发打死了玩家 则返回false
                    if (checkpoint.playerFighter.isDead()) {
                        return false;
                    }
                    return true;
                }
            }
            //判断玩家还有没有血
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
    //输出剩余血量
    outtextxy(x, y, str.c_str());
}

inline void showDetails(int x, int y, double cnt, string message) {
    stringstream ss;
    string str;
    ss << message;
    ss << cnt;
    ss >> str;
    //输出剩余血量
    outtextxy(x, y, str.c_str());
}

int menu() {
    cleardevice();//清理之前的内容
    settextcolor(0x0000AA);
    settextstyle(screan * 2, 0, _T("Consolas"));
    outtextxy(raw / 2, topMargin, "请选择关卡");

    outtextxy(raw / 2 - 10 * screan, topMargin + 3 * screan, "关卡1");
    outtextxy(raw / 2, topMargin + 3 * screan, "关卡2");
    outtextxy(raw / 2 + 10 * screan, topMargin + 3 * screan, "关卡3");
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