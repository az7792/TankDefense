#include "Tank.h"
#include "cocos2d.h"
USING_NS_CC;

Tank::Tank()
{
}
Tank::~Tank()
{
}
void Tank::setEnemies(std::vector < Enemy*> tmp)
{
    Enemies.clear();
    Enemies = tmp;
}
void Tank::update(float dt)
{
    cocos2d::Vec2 direction;    
    for (auto v : Enemies)
    {        
        if (v->isVisible() == false)
            continue;     
        if (v->GetEnemyGenerationSuccess() == false)//敌人还未生成
            continue;
        //CCLOG("%f %f %f %f", v->getPosition().x, v->getPosition().y, this->getPosition().x, this->getPosition().y);
        if ( v->getPosition().distance(this->getPosition()) <= Scope)
        {
            direction = v->getPosition() - this->getPosition();
            float angle = 360.f - (atan2(direction.y, direction.x) * 180 / M_PI);
            this->setRotation(angle);
            if (v->takeDamage(Attack) == false)
            {
                v->setVisible(false);
            }
            break;
        }
    }
}
Tank* Tank::create()
{
    Tank* tank = new Tank();
    if (tank && tank->initWithFile("Tank101.png")) // 使用坦克的纹理文件进行初始化
    {
        tank->autorelease();
        return tank;
    }
    CC_SAFE_DELETE(tank);
    return nullptr;
}

void Tank::Openschedule()
{
    this->schedule(CC_SCHEDULE_SELECTOR(Tank::update), 1.f/60);
}

void Tank::Downschedule()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(Tank::update));
}

