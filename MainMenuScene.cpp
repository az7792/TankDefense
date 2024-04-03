#include "MainMenuScene.h"
#include "ClassicModeScene.h"
#include "FreeModeScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

// 打印有用的错误信息，而不是在文件不存在时发生隔离故障。
static void problemLoading(const char* filename)
{
    printf("加载时出错：%s\n", filename);
    printf("根据你的编译方式，你可能需要在HelloWorldScene.cpp的文件名前加上'Resources/'。\n");
}

// 在 "init "中你需要初始化你的实例
bool MainMenu::init()
{
    //////////////////////////////
    // 1.首先是超级启动
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. 添加一个带有 "X "图像的菜单项，点击它可以退出程序。
    //    你可以修改它。

    // 添加一个 "关闭 "图标以退出进度，它是一个自动释放的对象。
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

    // 创建菜单，它是一个自动释放的对象
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3.在下面添加你的代码...

    auto GameName = Label::createWithTTF("Tank Defense", "fonts/Marker Felt.ttf", 100);
    GameName->setPosition(Vec2(origin.x + visibleSize.width / 2,origin.y + visibleSize.height - GameName->getContentSize().height));
    this->addChild(GameName, 1);
    GameName->setColor(Color3B(0, 122, 204));

    auto BackGround = Sprite::create("BackGroundImage.png");
    BackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(BackGround, 0);

    auto Read_file = ui::Button::create("StartNormal.png", "StartSelect.png", "StartDisable.png");//按钮
    auto Free_Mode = ui::Button::create("StartNormal.png", "StartSelect.png", "StartDisable.png");
    auto Classic_Mode = ui::Button::create("StartNormal.png", "StartSelect.png", "StartDisable.png");

    Read_file->setTitleText("Read Tour Archive");//按钮文本
    Read_file->setTitleFontSize(32);//按钮字体大小
    Free_Mode->setTitleText("Free_Mode");
    Free_Mode->setTitleFontSize(32);
    Classic_Mode->setTitleText("Classic_Mode");
    Classic_Mode->setTitleFontSize(32);

    Read_file->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + Read_file->getContentSize().height + 15));//按钮位置
    Free_Mode->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - Read_file->getContentSize().height - 15));
    Classic_Mode->setPosition(Vec2(visibleSize/2));

    //Read_file->setEnabled(false);

    Read_file->addClickEventListener([&](Ref* sender) {//按钮点击后的回调函数
        //CCLOG("Read_file");//打印日志


        });
    Free_Mode->addClickEventListener([&](Ref* sender) {//开自由典模式
       // CCLOG("Free_Mode");
        auto FreeScene = FreeModeScene::createScene();
        auto transition = TransitionFade::create(0.5f, FreeScene);
        Director::getInstance()->replaceScene(transition);
        });
    Classic_Mode->addClickEventListener([&](Ref* sender) {//开始经典模式
        //CCLOG("Classic_Mode");
        auto ClassicScene = ClassicModeScene::createScene();
        auto transition = TransitionFade::create(0.5f, ClassicScene);
        Director::getInstance()->replaceScene(transition);
        });

    this->addChild(Read_file,1);//添加到场景中
    this->addChild(Free_Mode,1);
    this->addChild(Classic_Mode,1);

    return true;
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
    //关闭cocos2d-x游戏场景并退出应用程序。
    Director::getInstance()->end();

    /*为了在不退出应用程序的情况下导航回原生iOS屏幕（如果存在的话），不要使用上面给出的Director::getInstance()->end()，而是触发一个在RootViewController.mm中创建的自定义事件，如下所示*/

        //EventCustom customEndEvent("game_scene_close_event")；
        //_eventDispatcher->dispatchEvent（&customEndEvent）；


}
