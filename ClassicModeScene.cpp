#include "ClassicModeScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include "InGameMenu.h"  // 在这里包含 InGameMenu 类的头文件

USING_NS_CC;


ClassicModeScene* ClassicModeScene::createScene()
{
    return ClassicModeScene::create();    
}

// 打印有用的错误信息，而不是在文件不存在时发生隔离故障。
static void problemLoading(const char* filename)
{
    printf("加载时出错：%s\n", filename);
    printf("根据你的编译方式，你可能需要在HelloWorldScene.cpp的文件名前加上'Resources/'。\n");
}


// 在 "init "中你需要初始化你的实例
bool ClassicModeScene::init()
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
        CC_CALLBACK_1(ClassicModeScene::menuCloseCallback, this));

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
    listener->onMouseDown = CC_CALLBACK_1(ClassicModeScene::onMouseDown, this);
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
            this->schedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::CheckEnemy), 0.1f);//打开检测敌人是否全部死亡的开关

            Start->setTouchEnabled(false);//复选框设置为不可互动
            GameState = true;//游戏状态为运行中
            //刷新敌人
            this->schedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::SetEnemySequence), 0.2f, EnemyNum-1, 0.0f);//给所有敌人添加动作
            break;
        default:
            break;
        }
        });

    //this->schedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::CheckEnemy), 0.1f);//每0.1秒检测敌人是否全部死亡

    menuLayer = InGameMenu::create();//菜单
    menuLayer->setVisible(false);//菜单不可见
    menuLayer->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
    this->addChild(menuLayer,10);
    menuLayer->setClassicModeScene(this);//传入当前场景方便菜单进行各种操作

    ui::Button*  GameMenu = ui::Button::create("InGameMenu.png", "InGameMenu.png");//菜单按钮
    GameMenu->setPosition(XYtoxy(Vec2(13, 7)));
    GameMenu->addClickEventListener([&](Ref* sender) {//按钮点击后的回调函数
        menuLayer->OpenMenu();//打开菜单
        });
    this->addChild(GameMenu, 2);


    return true;
}



void ClassicModeScene::menuCloseCallback(Ref* pSender)
{
    //关闭cocos2d-x游戏场景并退出应用程序。
    Director::getInstance()->end();

    /*为了在不退出应用程序的情况下导航回原生iOS屏幕（如果存在的话），不要使用上面给出的Director::getInstance()->end()，而是触发一个在RootViewController.mm中创建的自定义事件，如下所示*/

        //EventCustom customEndEvent("game_scene_close_event")；
        //_eventDispatcher->dispatchEvent（&customEndEvent）；
}


Vec2 ClassicModeScene::xytoXY(Vec2 Postion)//世界坐标转格子坐标
{
    Rect anchorPoint = BackGround->getBoundingBox();
    //格子91*91
    Vec2 XY;
    XY.x = (int)((Postion.x - anchorPoint.origin.x)/91) + 1;
    XY.y = (int)((Postion.y - anchorPoint.origin.y)/91) + 1;
    return XY;
}

Vec2 ClassicModeScene::XYtoxy(Vec2 Postion)//格子坐标转世界坐标
{
    Rect anchorPoint = BackGround->getBoundingBox();
    return Vec2(anchorPoint.origin.x + 91 * Postion.x - 91/2, anchorPoint.origin.y + 91 * Postion.y - 91 / 2);
}

void ClassicModeScene::CreateEnemy()//创建敌人
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

void ClassicModeScene::SetEnemySequence(float dt)//给敌人[EnemyP]添加动作
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

void ClassicModeScene::NextLevel()//进行下一关
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

void ClassicModeScene::takeDamage(int damage)//给基地扣血
{
    Health -= damage;
    HealthStr->setString("Health:" + std::to_string(Health));
    if (Health <= 0)//基地死亡后
    {
        //在这而添加死亡后处理事件        
        CCLOG("gameover");
    }
}

void ClassicModeScene::ReduceEnemy(Enemy *tmp)//杀死指定敌人
{    
    tmp->setVisible(false);
}

void ClassicModeScene::CheckEnemy(float dt)//检测敌人是否全部死亡及基地是否死亡
{
    if (Health <= 0)
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::CheckEnemy));
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
    this->unschedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::CheckEnemy));
    NextLevel();
}

bool ClassicModeScene::TankIsok(Vec2 XY)//判断当前格子是否可以安装坦克
{
    for (int i = 1; i < Path.size() - 2; ++i)
    {
        if (Path[i] == XY)
            return false;
    }
    for (auto v : Tanks)
    {
        if (XY == xytoXY(v->getPosition()))
            return false;
    }
    return true;
}

void ClassicModeScene::onMouseDown(cocos2d::EventMouse* event)//鼠标点击事件
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

void ClassicModeScene::setInputEnabled(bool IsInputEnabled)//设置游戏是否运行
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

void ClassicModeScene::ReturnMainScene()//返回主菜单
{    
    auto mainMenuScene = MainMenu::createScene();
    auto transition = TransitionFade::create(0.5f, mainMenuScene);
    Director::getInstance()->replaceScene(transition);
    Director::getInstance()->resume();//恢复游戏运行
}

void ClassicModeScene::RestartGame()//重开游戏
{
    auto newClassicModeScene = ClassicModeScene::createScene();//重新创建当前场景
    auto transition = TransitionFade::create(0.5f, newClassicModeScene);//平滑过度
    Director::getInstance()->replaceScene(transition);//运行重新创建的场景
    Director::getInstance()->resume();//恢复游戏运行
}

void ClassicModeScene::setTank(Vec2 XY)//安装坦克
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

//生成随机路径————————————————————————————


int ClassicModeScene::getF(MyNode node)
{
    //为三个方向分配权重
    if (node.y == 1)
    {
        if (node.f == 1)
            return 0;
        p = { 3,0,2 };
    }
    else if (node.y == 7)
    {
        if (node.f == 2)
            return 0;
        p = { 3,2,0 };
    }
    else if (node.f == 0)
        p = { 3,1,1 };
    else if (node.f == 1)
        p = { 2,3,0 };
    else
        p = { 2,0,3 };
    int sum = p.x + p.y + p.f;
    int rn = rand() % sum + 1;//根据权重随机选择一个方向
    if (rn <= p.x)
        return 0;
    else if (rn <= p.x + p.y)
        return 1;
    else if (rn <= sum)
        return 2;
}
void ClassicModeScene::dfs(MyNode node)
{
    Path.emplace_back(Vec2(node.x, node.y));
    if (node.x >= 11)
    {
        EndX = node.y; return;
    }
    int f = getF(node);//获取下一次方向
    dfs({ node.x + tx[f],node.y + ty[f],f });
}
void ClassicModeScene::setPath()
{
    srand(time(nullptr));
    Path.emplace_back(Vec2(0, 4));
    dfs({ 1,4,0 });
    int t = EndX - 4 < 0 ? 1 : -1;
    for (int j = EndX; j != 4 + t; j += t)
        Path.emplace_back(Vec2(12, j));
    Path.emplace_back(Vec2(13, 4));
    Path.emplace_back(Vec2(14, 4));
    auto tmp = Sprite::create("StartPath.png");
    tmp->setPosition(XYtoxy(Vec2(0, 4)));
    this->addChild(tmp, 2);
    tmp = Sprite::create("EndPath.png");
    tmp->setPosition(XYtoxy(Vec2(13, 4)));
    this->addChild(tmp, 2);
    for (int i = 1; i < Path.size() - 2; ++i)//刷新地图
    {
        tmp = Sprite::create("Path.png");
        this->addChild(tmp, 2);
        tmp->setPosition(XYtoxy(Path[i]));
    }
}
//生成随机路径——————————————————————————