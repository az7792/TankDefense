#include "Enemy.h"
#include <cstdlib>
USING_NS_CC;
std::vector<std::pair<cocos2d::Vec3, std::string>> Enemy::G;

Enemy::Enemy(float h, float s, int a): Health(h),Speed(s),Attack(a)
{    
}

Enemy::~Enemy()
{
}

Enemy* Enemy::create(float h, float s, int a)
{
    Enemy* enemy = new Enemy(h,s,a);
    if (enemy && enemy->initWithFile("Enemy1.png")) // ʹ�õ��˵������ļ����г�ʼ��
    {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

Enemy* Enemy::create(int Level)
{    
    //Level = 2;
    initVector();
    int p = rand() % G.size();
    Enemy* enemy = new Enemy(G[p].first.x * Level * 0.5, G[p].first.y * Level * 0.5, (int)G[p].first.z* Level * 0.5);
    if (enemy && enemy->initWithFile(G[p].second)) // ʹ�õ��˵������ļ����г�ʼ��
    {
        enemy->autorelease();
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}


bool Enemy::takeDamage(float damage)
{
    Health -= damage; // ���ٵ��˵Ľ���ֵ

    if (Health <= 0.0f) // ������˵Ľ���ֵС�ڵ���0
    {    
        return false;
    }
    return true;
}

void Enemy::initVector()
{
    G = { {cocos2d::Vec3(1,200,3),"Enemy1.png"},{cocos2d::Vec3(1,150,3),"Enemy2.png"},{cocos2d::Vec3(1,100,3),"Enemy3.png"} };
}
