#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // һ��ѡ�����ص�
    void menuCloseCallback(cocos2d::Ref* pSender);


    //�ֶ�ʵ�� "��̬����() "����
    CREATE_FUNC(MainMenu);
};

