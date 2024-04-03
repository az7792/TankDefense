#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // 一个选择器回调
    void menuCloseCallback(cocos2d::Ref* pSender);


    //手动实现 "静态创建() "方法
    CREATE_FUNC(MainMenu);
};

