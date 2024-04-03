#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "ClassicModeScene.h"
class ClassicModeScene;  // ǰ������ ClassicModeScene ��
class FreeModeScene;  // ǰ������ ClassicModeScene ��

class InGameMenu : public cocos2d::Layer {
public:
    virtual bool init();

    void OpenMenu();//�򿪲˵�

    void OpenGameOverMenu();//����Ϸ�����˵�

    void CloseMenu();//�رղ˵���������Ϸ

    void setClassicModeScene(ClassicModeScene* scene);
    void setFreeModeScene(FreeModeScene* scene);

    CREATE_FUNC(InGameMenu);
private:
    ClassicModeScene *classicModeScene;
    FreeModeScene* freeModeScene;
    cocos2d::MenuItemLabel* ReturnGame;
    cocos2d::MenuItemLabel* SaveGame;
    cocos2d::Menu* menu;
    int Mode;
};