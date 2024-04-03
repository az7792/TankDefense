#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "ClassicModeScene.h"
class ClassicModeScene;  // 前向声明 ClassicModeScene 类
class FreeModeScene;  // 前向声明 ClassicModeScene 类

class InGameMenu : public cocos2d::Layer {
public:
    virtual bool init();

    void OpenMenu();//打开菜单

    void OpenGameOverMenu();//打开游戏结束菜单

    void CloseMenu();//关闭菜单并返回游戏

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