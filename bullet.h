//
// Created by SWQXDBA on 2021/5/24.
//

#ifndef EASYXTEST_BULLET_H
#define EASYXTEST_BULLET_H
class Bullet{
public:
    IMAGE picture;
    int x;
    int y;
    int xmove;
    int ymove;
    int ATTACK =3;
};
Bullet getPlayerBullet(int X,int Y,IMAGE image){
    Bullet nBullet;
    nBullet.picture=image;
    nBullet.x=X;
    nBullet.y=Y;
    nBullet.xmove=0;
    nBullet.ymove=-5;
    return nBullet;
}
#endif //EASYXTEST_BULLET_H
