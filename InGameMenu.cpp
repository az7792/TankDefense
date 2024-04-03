#include "cocos2d.h"
#include "InGameMenu.h"
#include "ClassicModeScene.h"  // ��������� ClassicModeScene ���ͷ�ļ�
#include "FreeModeScene.h"
USING_NS_CC;
bool InGameMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto layer = cocos2d::LayerColor::create(cocos2d::Color4B(128, 128, 128, 128));//�˵���ɫ����
    layer->setContentSize(cocos2d::Size(1280, 720));
    layer->setPosition(Vec2(-640,-360));
    this->addChild(layer);

    auto Sp = Sprite::create("MenuBackground.png");
    Sp->setPosition(Vec2::ANCHOR_MIDDLE_TOP);
    this->addChild(Sp);

    // ������Ϸ
        ReturnGame = MenuItemLabel::create(
        Label::createWithTTF("Return", "fonts/Marker Felt.ttf", 34),
        [this](Ref* sender) {
            // �رղ˵���
            CloseMenu();            
        }
    );

    //�浵
        SaveGame = MenuItemLabel::create(
        Label::createWithTTF("Save game", "fonts/Marker Felt.ttf", 34),
        [this](Ref* sender) {
            //�浵            
        }
    );

    //�������˵�
    auto RetuenMainScene = MenuItemLabel::create(
        Label::createWithTTF("Return to main menu", "fonts/Marker Felt.ttf", 34),
        [this](Ref* sender) {
            //
            if (Mode == 1)
                classicModeScene->ReturnMainScene();
            else
                freeModeScene->ReturnMainScene();
        }
    );

    //���¿�ʼ
    auto RestartGame = MenuItemLabel::create(
        Label::createWithTTF("Restart the game", "fonts/Marker Felt.ttf", 34),
        [this](Ref* sender) {
            //   
            if (Mode == 1)
                classicModeScene->RestartGame();
            else
                freeModeScene->RestartGame();
        }
    );
    
    // �����˵�
    menu = Menu::create(ReturnGame, SaveGame, RestartGame, RetuenMainScene, nullptr);
    menu->alignItemsVertically(); // ��ֱ���в˵���
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    return true;
}

void InGameMenu::OpenMenu()
{        
    if (Mode == 1)
        classicModeScene->setInputEnabled(false);
    else
        freeModeScene->setInputEnabled(false);
    this->setVisible(true);
    Director::getInstance()->pause();
}

void InGameMenu::OpenGameOverMenu()
{
    OpenMenu();
    SaveGame->setEnabled(false);
    ReturnGame->setEnabled(false);
}

void InGameMenu::CloseMenu()
{
    if (Mode == 1)
        classicModeScene->setInputEnabled(true);
    else
        freeModeScene->setInputEnabled(true);
    this->setVisible(false);
    Director::getInstance()->resume();
}

void InGameMenu::setClassicModeScene(ClassicModeScene* scene)
{
    Mode = 1;
    classicModeScene = scene;
}

void InGameMenu::setFreeModeScene(FreeModeScene* scene)
{
    Mode = 2;
    freeModeScene = scene;
}


