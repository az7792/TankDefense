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

    void setEnemies(std::vector < Enemy* >tmp);//���µ���    


    void update(float dt);//�ж��Ƿ񹥻�����

    static Tank* create();

    
    void Openschedule();

    void Downschedule();

private:
    float TankAngle=0.f;             // ̹�˽Ƕ�
    float Scope = 1.5f * 91.f;                 // ������Χ
    float Attack = 1/60.f;                   // ̹�˵Ĺ�����
    int sale_price  = 10;               // ���ۼ۸�

    std::vector < Enemy* > Enemies;
};

