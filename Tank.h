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

    void setEnemies(std::vector < Enemy* >tmp);//¸üÐÂµÐÈË    


    void update(float dt);//ÅÐ¶ÏÊÇ·ñ¹¥»÷µÐÈË

    static Tank* create();

    
    void Openschedule();

    void Downschedule();

private:
    float TankAngle=0.f;             // Ì¹¿Ë½Ç¶È
    float Scope = 1.5f * 91.f;                 // ¹¥»÷·¶Î§
    float Attack = 1/60.f;                   // Ì¹¿ËµÄ¹¥»÷Á¦
    int sale_price  = 10;               // ³öÊÛ¼Û¸ñ

    std::vector < Enemy* > Enemies;
};

