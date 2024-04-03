#include "FreeModeScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include <ctime>
#include <vector>
#include <queue>
#include <cstdlib>
#include <string>
#include "InGameMenu.h"  // 在这里包含 InGameMenu 类的头文件

USING_NS_CC;


FreeModeScene* FreeModeScene::createScene()
{
    return FreeModeScene::create();    
}

// 打印有用的错误信息，而不是在文件不存在时发生隔离故障。
static void problemLoading(const char* filename)
{
    printf("加载时出错：%s\n", filename);
    printf("根据你的编译方式，你可能需要在HelloWorldScene.cpp的文件名前加上'Resources/'。\n");
}


// 在 "init "中你需要初始化你的实例
bool FreeModeScene::init()
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
        CC_CALLBACK_1(FreeModeScene::menuCloseCallback, this));

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

    auto WhiteBackground = Sprite::create("WhiteBackground.png");//白色背景
    this->addChild(WhiteBackground, 0);
    WhiteBackground->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

    BackGround = Sprite::create("BackgroundMap.png");//地图背景
    BackGround->setAnchorPoint(Vec2(0, 0));
    BackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x - BackGround->getContentSize().width/2, 0));
    this->addChild(BackGround, 1);

    HealthStr = Label::createWithTTF("Health:" + std::to_string(Health), "fonts/Marker Felt.ttf", 30);//血量显示
    HealthStr->setTextColor(Color4B::BLACK);
    HealthStr->setPosition(XYtoxy(Vec2(12,8)));
    this->addChild(HealthStr, 2);

    LevelStr = Label::createWithTTF("Level:1", "fonts/Marker Felt.ttf", 30);//关卡等级显示
    LevelStr->setTextColor(Color4B::BLACK);
    this->addChild(LevelStr, 2);
    LevelStr->setPosition(XYtoxy(Vec2(1, 8)));

    setPath();//生成路径

    auto listener = cocos2d::EventListenerMouse::create();//注册地图鼠标点击事件
    listener->onMouseDown = CC_CALLBACK_1(FreeModeScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, BackGround);

    Start = cocos2d::ui::CheckBox::create("NotStarted.png", "Start.png");//准备完后开始游戏
    Start->setPosition(XYtoxy(Vec2(0,1)));
    this->addChild(Start,1);

    CreateEnemy();//生成敌人

    Start->addEventListener([=](Ref* sender, cocos2d::ui::CheckBox::EventType type) {//复选框(开始游戏)点击触发的事件
        switch (type) {
        case cocos2d::ui::CheckBox::EventType::SELECTED://复选框被选中状态(游戏开始)
            for (auto v : Tanks)//更新坦克状态
            {
                v->setEnemies(Enemies);//更新坦克的敌人列表
                v->Openschedule();//开启坦克检测敌人
            }
            this->schedule(CC_SCHEDULE_SELECTOR(FreeModeScene::CheckEnemy), 0.1f);//打开检测敌人是否全部死亡的开关

            Start->setTouchEnabled(false);//复选框设置为不可互动
            GameState = true;//游戏状态为运行中
            //刷新敌人
            this->schedule(CC_SCHEDULE_SELECTOR(FreeModeScene::SetEnemySequence), 0.2f, EnemyNum-1, 0.0f);//给所有敌人添加动作
            break;
        default:
            break;
        }
        });

    //this->schedule(CC_SCHEDULE_SELECTOR(FreeModeScene::CheckEnemy), 0.1f);//每0.1秒检测敌人是否全部死亡

    menuLayer = InGameMenu::create();//菜单
    menuLayer->setVisible(false);//菜单不可见
    menuLayer->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
    this->addChild(menuLayer,10);
    menuLayer->setFreeModeScene(this);//传入当前场景方便菜单进行各种操作

    ui::Button*  GameMenu = ui::Button::create("InGameMenu.png", "InGameMenu.png");//菜单按钮
    GameMenu->setPosition(XYtoxy(Vec2(13, 7)));
    GameMenu->addClickEventListener([&](Ref* sender) {//按钮点击后的回调函数
        menuLayer->OpenMenu();//打开菜单
        });
    this->addChild(GameMenu, 2);


    return true;
}



void FreeModeScene::menuCloseCallback(Ref* pSender)
{
    //关闭cocos2d-x游戏场景并退出应用程序。
    Director::getInstance()->end();

    /*为了在不退出应用程序的情况下导航回原生iOS屏幕（如果存在的话），不要使用上面给出的Director::getInstance()->end()，而是触发一个在RootViewController.mm中创建的自定义事件，如下所示*/

        //EventCustom customEndEvent("game_scene_close_event")；
        //_eventDispatcher->dispatchEvent（&customEndEvent）；
}


Vec2 FreeModeScene::xytoXY(Vec2 Postion)//世界坐标转格子坐标
{
    Rect anchorPoint = BackGround->getBoundingBox();
    //格子91*91
    Vec2 XY;
    XY.x = (int)((Postion.x - anchorPoint.origin.x)/91) + 1;
    XY.y = (int)((Postion.y - anchorPoint.origin.y)/91) + 1;
    return XY;
}

Vec2 FreeModeScene::XYtoxy(Vec2 Postion)//格子坐标转世界坐标
{
    Rect anchorPoint = BackGround->getBoundingBox();
    return Vec2(anchorPoint.origin.x + 91 * Postion.x - 91/2, anchorPoint.origin.y + 91 * Postion.y - 91 / 2);
}

void FreeModeScene::CreateEnemy()//创建敌人
{
    Enemy* tmp;
    for (int i = 0; i < EnemyNum; ++i)//创建EnemyNum个敌人
    {
        tmp = Enemy::create(Level);//根据等级随机生成一个敌人    
        tmp->setPosition(XYtoxy(Vec2(0, 4)));
        Enemies.push_back(tmp );        
        this->addChild(tmp, 2);
    }     
}

void FreeModeScene::SetEnemySequence(float dt)//给敌人[EnemyP]添加动作
{
    Vector<FiniteTimeAction*> actions;
    Enemy* tmp = Enemies[EnemyP];
    Vec2 startPosition, direction;
    actions.clear();
    startPosition = XYtoxy(Path[0]);
    for (int i = 1; i < Path.size(); ++i)
    {
        direction = XYtoxy(Path[i]) - startPosition;
        float angle = 360.f - (atan2(direction.y, direction.x) * 180 / M_PI);//旋转角度
        // 创建一个移动动作
        float duration = startPosition.distance(XYtoxy(Path[i])) / tmp->GetSpeed(); // 根据距离计算持续时间
        auto moveAction = MoveTo::create(duration, XYtoxy(Path[i]));
        // 创建一个回调动作，在每一段直线运动开始时设置精灵的旋转角度
        auto rotateAction = CallFunc::create([=]() {
            if (Path[i].x - 14 == 0)//判断是否到达基地
            {
                if (tmp->isVisible())
                {
                    takeDamage(tmp->GetAttack());       
                    ReduceEnemy(tmp);
                }
            }
            tmp->setRotation(angle);
            });
        // 将移动动作和回调动作依次添加到动作序列中
        actions.pushBack(rotateAction);
        actions.pushBack(moveAction);
        startPosition = XYtoxy(Path[i]);
    }
    auto sequence = Sequence::create(actions);
    tmp->runAction(sequence);
    actions.clear();
    Enemies[EnemyP]->SetEnemyGenerationSuccess(true);
    EnemyP++;
}

void FreeModeScene::NextLevel()//进行下一关
{

    //初始化
    EnemyP = 0;
    GameState = false;
    Start->setSelected(false);
    Start->setTouchEnabled(true);
    NowEnemyNum = EnemyNum;
    ++Level;
    LevelStr->setString("Level:" + std::to_string(Level));
    for (auto v : Tanks)
        v->Downschedule();
    for (auto v : Enemies)
    {
        this->removeChild(v);
    }
    Enemies.clear();
    CreateEnemy();    
}

void FreeModeScene::takeDamage(int damage)//给基地扣血
{
    Health -= damage;
    HealthStr->setString("Health:" + std::to_string(Health));
    if (Health <= 0)//基地死亡后
    {
        //在这而添加死亡后处理事件        
        CCLOG("gameover");
    }
}

void FreeModeScene::ReduceEnemy(Enemy *tmp)//杀死指定敌人
{    
    tmp->setVisible(false);
}

void FreeModeScene::CheckEnemy(float dt)//检测敌人是否全部死亡及基地是否死亡
{
    if (Health <= 0)
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(FreeModeScene::CheckEnemy));
        menuLayer->OpenGameOverMenu();
        return;
    }
    for (auto v : Enemies)
    {
        if (v->isVisible())
        {
            return;
        }
    }
    this->unschedule(CC_SCHEDULE_SELECTOR(FreeModeScene::CheckEnemy));
    NextLevel();
}

bool FreeModeScene::TankIsok(Vec2 XY)
{
    if (XY == Vec2(1, 4))
        return false;
    bool f = true;
    int tmpMap[13][8];
    for (int i = 1; i <= 12; ++i)
    {
        for (int j = 1; j <= 7; ++j)
        {
            tmpMap[i][j] = 0; //空地
        }
    }

    for (int i = 1; i < Path.size() - 2; ++i)
    {
        if (Path[i] == XY)
        {
            f = false;
        }
       // tmpMap[(int)Path[i].x][(int)Path[i].y] = 1; //障碍物
    }

    for (auto v : Tanks)
    {
        Vec2 nowxy = xytoXY(v->getPosition());
        if (nowxy == XY)
        {
            f = false;
        }
        tmpMap[(int)nowxy.x][(int)nowxy.y] = 1; //障碍物
    }

    if (f)
    {
        return f; //坦克安装在非路径和坦克上，可以直接安装，否则重新搜索
    }

    tmpMap[(int)XY.x][(int)XY.y] = 1; //假设当前可以安装坦克

    //BFS
    std::vector<cocos2d::Vec2> tmpPath; //记录路径
    Vec2 path[13][8];                 //记录当前点是从那个点过来的

    std::queue<Vec2> Q;
    Vec2 End = { 12, 4 };
    Q.push(Vec2(1, 4));

    while (!Q.empty())
    {
        Vec2 cur = Q.front();
        Q.pop();

        for (int i = 0; i <= 3; ++i)
        {
            int nx = cur.x + tx[i];
            int ny = cur.y + ty[i];

            if (nx >= 1 && nx <= 12 && ny >= 1 && ny <= 7 && tmpMap[nx][ny] == 0)
            {
                Q.push(Vec2(nx, ny));
                tmpMap[nx][ny] = 1;
                path[nx][ny] = cur;
            }
            else
                continue;
            if (Vec2(nx, ny) == End)
            {
                // 到达终点
                while (cur != Vec2(1, 4))
                {
                    tmpPath.push_back(cur);
                    cur = path[(int)cur.x][(int)cur.y];
                }
                tmpPath.push_back(Vec2(1, 4));
                tmpPath.push_back(Vec2(0, 4));
                std::reverse(tmpPath.begin(), tmpPath.end());
                tmpPath.push_back(End);
                tmpPath.push_back(Vec2(13, 4));
                tmpPath.push_back(Vec2(14, 4));
                Path.clear();
                Path = tmpPath;
                return true;                
            }
        }
    }
    return false; 
}

void FreeModeScene::onMouseDown(cocos2d::EventMouse* event)//鼠标点击事件
{    
    if (!isInputEnabled)
        return;
    cocos2d::Vec2 eventXY = event->getLocation();
    eventXY.y = Director::getInstance()->getVisibleSize().height - eventXY.y;
    if (BackGround->getBoundingBox().containsPoint(eventXY))
    {
        Vec2 pii = xytoXY(eventXY);
        if(GameState==false)
            setTank(pii);//安装坦克
    }
}

void FreeModeScene::setInputEnabled(bool IsInputEnabled)//设置游戏是否运行
{
    isInputEnabled = IsInputEnabled;
    if (!GameState)
    {
        if(IsInputEnabled)
            Start->setTouchEnabled(true);
        else
            Start->setTouchEnabled(false);
    }
}

void FreeModeScene::ReturnMainScene()//返回主菜单
{    
    auto mainMenuScene = MainMenu::createScene();
    auto transition = TransitionFade::create(0.5f, mainMenuScene);
    Director::getInstance()->replaceScene(transition);
    Director::getInstance()->resume();//恢复游戏运行
}

void FreeModeScene::RestartGame()//重开游戏
{
    auto newFreeModeScene = FreeModeScene::createScene();//重新创建当前场景
    auto transition = TransitionFade::create(0.5f, newFreeModeScene);//平滑过度
    Director::getInstance()->replaceScene(transition);//运行重新创建的场景
    Director::getInstance()->resume();//恢复游戏运行
}

void FreeModeScene::setTank(Vec2 XY)//安装坦克
{
    if (TankIsok(XY))//先检测当前位置是否可以安装
    {
        Tank* tmp = Tank::create();
        tmp->setEnemies(Enemies);
        tmp->setPosition(XYtoxy(XY));
        Tanks.push_back(tmp);
        this->addChild(tmp, 2);
    }
}




void FreeModeScene::setPath()//初始路径
{
    auto tmp = Sprite::create("StartPath.png");
    tmp->setPosition(XYtoxy(Vec2(0, 4)));
    this->addChild(tmp, 2);
    tmp = Sprite::create("EndPath.png");
    tmp->setPosition(XYtoxy(Vec2(13, 4)));
    this->addChild(tmp, 2);
    for (int i = 0; i <= 14; ++i)
        Path.emplace_back(Vec2(i, 4));
}