#pragma once
#include "cocos2d.h"
#include <utility>
#include <vector>
#include <string>
class Enemy : public cocos2d::Sprite
{
public:
    Enemy(float h, float s, int a);
    virtual ~Enemy();

    static Enemy* create(float h, float s, int a);

    static Enemy* create(int Level);

    float GetSpeed() { return Speed; }

    float GetAttack() { return Attack; }

    bool takeDamage(float damage);

    bool GetEnemyGenerationSuccess() { return EnemyGenerationSuccess; }

    void SetEnemyGenerationSuccess(bool f) { EnemyGenerationSuccess = f; }

    static void initVector();

private:
    bool EnemyGenerationSuccess=false;//敌人是否生成成功
    float Health;
    float Speed;
    int Attack;
    static std::vector<std::pair<cocos2d::Vec3,std::string>>G;//敌人预设
};
