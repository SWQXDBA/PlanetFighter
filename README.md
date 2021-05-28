# PlanetFighter
# 飞机大战
# C++的期末大作业
# 可以简单地自己定制新的关卡、资源。
# 可以手动设置游戏的进行频率与帧数 缓解硬件压力。

# init()用于加载所有关卡所需要的bullet(子弹)资源,位于main.cpp中

# checkpoint
checkpoint用于定制想要的关卡，位于checkpoint.h。然后在menu()中进行相应的设置 以进行关卡的选择，menu()位于main.cpp中


# 以下内容位于model.h
# EnemyFactory
EnemyFactory中配置了所有的敌人类型 同时可以根据getEnemy(int loc)和getEnemy(int loc, int hp, int attack)获取想要的敌人 。同一个loc的敌人有同样的图标

# Enemy 
Enemy定义了一个敌人 他拥有一个自己的bullet vector(叫做myBullets的成员)用于存放自己的bullet 当调用shoot()，他会遍历myBullets，把每一个bullet添加到主程序的bullets容器中。

# Timer
Timer是一个定时器类 bool passedtime(int ms)：
判断距离上次调用这个函数是否经过了想要的时间，第一次调用返回true 。
int getPassedTime() :
返回上次调用这个函数到本次调用经过的时间 单位毫秒(ms)
