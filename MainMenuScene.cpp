#include "MainMenuScene.h"
#include "ClassicModeScene.h"
#include "FreeModeScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

// ��ӡ���õĴ�����Ϣ�����������ļ�������ʱ����������ϡ�
static void problemLoading(const char* filename)
{
    printf("����ʱ����%s\n", filename);
    printf("������ı��뷽ʽ���������Ҫ��HelloWorldScene.cpp���ļ���ǰ����'Resources/'��\n");
}

// �� "init "������Ҫ��ʼ�����ʵ��
bool MainMenu::init()
{
    //////////////////////////////
    // 1.�����ǳ�������
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. ���һ������ "X "ͼ��Ĳ˵������������˳�����
    //    ������޸�����

    // ���һ�� "�ر� "ͼ�����˳����ȣ�����һ���Զ��ͷŵĶ���
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // �����˵�������һ���Զ��ͷŵĶ���
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3.�����������Ĵ���...

    auto GameName = Label::createWithTTF("Tank Defense", "fonts/Marker Felt.ttf", 100);
    GameName->setPosition(Vec2(origin.x + visibleSize.width / 2,origin.y + visibleSize.height - GameName->getContentSize().height));
    this->addChild(GameName, 1);
    GameName->setColor(Color3B(0, 122, 204));

    auto BackGround = Sprite::create("BackGroundImage.png");
    BackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(BackGround, 0);

    auto Read_file = ui::Button::create("StartNormal.png", "StartSelect.png", "StartDisable.png");//��ť
    auto Free_Mode = ui::Button::create("StartNormal.png", "StartSelect.png", "StartDisable.png");
    auto Classic_Mode = ui::Button::create("StartNormal.png", "StartSelect.png", "StartDisable.png");

    Read_file->setTitleText("Read Tour Archive");//��ť�ı�
    Read_file->setTitleFontSize(32);//��ť�����С
    Free_Mode->setTitleText("Free_Mode");
    Free_Mode->setTitleFontSize(32);
    Classic_Mode->setTitleText("Classic_Mode");
    Classic_Mode->setTitleFontSize(32);

    Read_file->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + Read_file->getContentSize().height + 15));//��ťλ��
    Free_Mode->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - Read_file->getContentSize().height - 15));
    Classic_Mode->setPosition(Vec2(visibleSize/2));

    //Read_file->setEnabled(false);

    Read_file->addClickEventListener([&](Ref* sender) {//��ť�����Ļص�����
        //CCLOG("Read_file");//��ӡ��־


        });
    Free_Mode->addClickEventListener([&](Ref* sender) {//�����ɵ�ģʽ
       // CCLOG("Free_Mode");
        auto FreeScene = FreeModeScene::createScene();
        auto transition = TransitionFade::create(0.5f, FreeScene);
        Director::getInstance()->replaceScene(transition);
        });
    Classic_Mode->addClickEventListener([&](Ref* sender) {//��ʼ����ģʽ
        //CCLOG("Classic_Mode");
        auto ClassicScene = ClassicModeScene::createScene();
        auto transition = TransitionFade::create(0.5f, ClassicScene);
        Director::getInstance()->replaceScene(transition);
        });

    this->addChild(Read_file,1);//��ӵ�������
    this->addChild(Free_Mode,1);
    this->addChild(Classic_Mode,1);

    return true;
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
    //�ر�cocos2d-x��Ϸ�������˳�Ӧ�ó���
    Director::getInstance()->end();

    /*Ϊ���ڲ��˳�Ӧ�ó��������µ�����ԭ��iOS��Ļ��������ڵĻ�������Ҫʹ�����������Director::getInstance()->end()�����Ǵ���һ����RootViewController.mm�д������Զ����¼���������ʾ*/

        //EventCustom customEndEvent("game_scene_close_event")��
        //_eventDispatcher->dispatchEvent��&customEndEvent����


}
