#pragma once
#include "Enemy.h"
#include "cocos2d.h"
#include <vector>
#include <utility>
class Tank : public cocos2d::Sprite
{
public:
    Tank();
    virtual ~Tank();

    void setEnemies(std::vector < Enemy* >tmp);//更新敌人    


    void update(float dt);//判断是否攻击敌人

    static Tank* create();

    
    void Openschedule();

    void Downschedule();

private:
    float TankAngle=0.f;             // 坦克角度
    float Scope = 1.5f * 91.f;                 // 攻击范围
    float Attack = 1/60.f;                   // 坦克的攻击力
    int sale_price  = 10;               // 出售价格

    std::vector < Enemy* > Enemies;
};

