#include "ClassicModeScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include "InGameMenu.h"  // ��������� InGameMenu ���ͷ�ļ�

USING_NS_CC;


ClassicModeScene* ClassicModeScene::createScene()
{
    return ClassicModeScene::create();    
}

// ��ӡ���õĴ�����Ϣ�����������ļ�������ʱ����������ϡ�
static void problemLoading(const char* filename)
{
    printf("����ʱ����%s\n", filename);
    printf("������ı��뷽ʽ���������Ҫ��HelloWorldScene.cpp���ļ���ǰ����'Resources/'��\n");
}


// �� "init "������Ҫ��ʼ�����ʵ��
bool ClassicModeScene::init()
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

    // �����˵�������һ���Զ��ͷŵĶ���
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3.�����������Ĵ���...

    auto WhiteBackground = Sprite::create("WhiteBackground.png");//��ɫ����
    this->addChild(WhiteBackground, 0);
    WhiteBackground->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

    BackGround = Sprite::create("BackgroundMap.png");//��ͼ����
    BackGround->setAnchorPoint(Vec2(0, 0));
    BackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x - BackGround->getContentSize().width/2, 0));
    this->addChild(BackGround, 1);

    HealthStr = Label::createWithTTF("Health:" + std::to_string(Health), "fonts/Marker Felt.ttf", 30);//Ѫ����ʾ
    HealthStr->setTextColor(Color4B::BLACK);
    HealthStr->setPosition(XYtoxy(Vec2(12,8)));
    this->addChild(HealthStr, 2);

    LevelStr = Label::createWithTTF("Level:1", "fonts/Marker Felt.ttf", 30);//�ؿ��ȼ���ʾ
    LevelStr->setTextColor(Color4B::BLACK);
    this->addChild(LevelStr, 2);
    LevelStr->setPosition(XYtoxy(Vec2(1, 8)));

    setPath();//����·��

    auto listener = cocos2d::EventListenerMouse::create();//ע���ͼ������¼�
    listener->onMouseDown = CC_CALLBACK_1(ClassicModeScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, BackGround);

    Start = cocos2d::ui::CheckBox::create("NotStarted.png", "Start.png");//׼�����ʼ��Ϸ
    Start->setPosition(XYtoxy(Vec2(0,1)));
    this->addChild(Start,1);

    CreateEnemy();//���ɵ���

    Start->addEventListener([=](Ref* sender, cocos2d::ui::CheckBox::EventType type) {//��ѡ��(��ʼ��Ϸ)����������¼�
        switch (type) {
        case cocos2d::ui::CheckBox::EventType::SELECTED://��ѡ��ѡ��״̬(��Ϸ��ʼ)
            for (auto v : Tanks)//����̹��״̬
            {
                v->setEnemies(Enemies);//����̹�˵ĵ����б�
                v->Openschedule();//����̹�˼�����
            }
            this->schedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::CheckEnemy), 0.1f);//�򿪼������Ƿ�ȫ�������Ŀ���

            Start->setTouchEnabled(false);//��ѡ������Ϊ���ɻ���
            GameState = true;//��Ϸ״̬Ϊ������
            //ˢ�µ���
            this->schedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::SetEnemySequence), 0.2f, EnemyNum-1, 0.0f);//�����е�����Ӷ���
            break;
        default:
            break;
        }
        });

    //this->schedule(CC_SCHEDULE_SELECTOR(ClassicModeScene::CheckEnemy), 0.1f);//ÿ0.1��������Ƿ�ȫ������

    menuLayer = InGameMenu::create();//�˵�
    menuLayer->setVisible(false);//�˵����ɼ�
    menuLayer->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
    this->addChild(menuLayer,10);
    menuLayer->setClassicModeScene(this);//���뵱ǰ��������˵����и��ֲ���

    ui::Button*  GameMenu = ui::Button::create("InGameMenu.png", "InGameMenu.png");//�˵���ť
    GameMenu->setPosition(XYtoxy(Vec2(13, 7)));
    GameMenu->addClickEventListener([&](Ref* sender) {//��ť�����Ļص�����
        menuLayer->OpenMenu();//�򿪲˵�
        });
    this->addChild(GameMenu, 2);


    return true;
}



void ClassicModeScene::menuCloseCallback(Ref* pSender)
{
    //�ر�cocos2d-x��Ϸ�������˳�Ӧ�ó���
    Director::getInstance()->end();

    /*Ϊ���ڲ��˳�Ӧ�ó��������µ�����ԭ��iOS��Ļ��������ڵĻ�������Ҫʹ�����������Director::getInstance()->end()�����Ǵ���һ����RootViewController.mm�д������Զ����¼���������ʾ*/

        //EventCustom customEndEvent("game_scene_close_event")��
        //_eventDispatcher->dispatchEvent��&customEndEvent����
}


Vec2 ClassicModeScene::xytoXY(Vec2 Postion)//��������ת��������
{
    Rect anchorPoint = BackGround->getBoundingBox();
    //����91*91
    Vec2 XY;
    XY.x = (int)((Postion.x - anchorPoint.origin.x)/91) + 1;
    XY.y = (int)((Postion.y - anchorPoint.origin.y)/91) + 1;
    return XY;
}

Vec2 ClassicModeScene::XYtoxy(Vec2 Postion)//��������ת��������
{
    Rect anchorPoint = BackGround->getBoundingBox();
    return Vec2(anchorPoint.origin.x + 91 * Postion.x - 91/2, anchorPoint.origin.y + 91 * Postion.y - 91 / 2);
}

void ClassicModeScene::CreateEnemy()//��������
{
    Enemy* tmp;
    for (int i = 0; i < EnemyNum; ++i)//����EnemyNum������
    {
        tmp = Enemy::create(Level);//���ݵȼ��������һ������    
        tmp->setPosition(XYtoxy(Vec2(0, 4)));
        Enemies.push_back(tmp );        
        this->addChild(tmp, 2);
    }        
}

void ClassicModeScene::SetEnemySequence(float dt)//������[EnemyP]��Ӷ���
{
    Vector<FiniteTimeAction*> actions;
    Enemy* tmp = Enemies[EnemyP];
    Vec2 startPosition, direction;
    actions.clear();
    startPosition = XYtoxy(Path[0]);
    for (int i = 1; i < Path.size(); ++i)
    {
        direction = XYtoxy(Path[i]) - startPosition;
        float angle = 360.f - (atan2(direction.y, direction.x) * 180 / M_PI);//��ת�Ƕ�
        // ����һ���ƶ�����
        float duration = startPosition.distance(XYtoxy(Path[i])) / tmp->GetSpeed(); // ���ݾ���������ʱ��
        auto moveAction = MoveTo::create(duration, XYtoxy(Path[i]));
        // ����һ���ص���������ÿһ��ֱ���˶���ʼʱ���þ������ת�Ƕ�
        auto rotateAction = CallFunc::create([=]() {
            if (Path[i].x - 14 == 0)//�ж��Ƿ񵽴����
            {
                if (tmp->isVisible())
                {
                    takeDamage(tmp->GetAttack());       
                    ReduceEnemy(tmp);
                }
            }
            tmp->setRotation(angle);
            });
        // ���ƶ������ͻص�����������ӵ�����������
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

void ClassicModeScene::NextLevel()//������һ��
{

    //��ʼ��
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

void ClassicModeScene::takeDamage(int damage)//�����ؿ�Ѫ
{
    Health -= damage;
    HealthStr->setString("Health:" + std::to_string(Health));
    if (Health <= 0)//����������
    {
        //�����������������¼�        
        CCLOG("gameover");
    }
}

void ClassicModeScene::ReduceEnemy(Enemy *tmp)//ɱ��ָ������
{    
    tmp->setVisible(false);
}

void ClassicModeScene::CheckEnemy(float dt)//�������Ƿ�ȫ�������������Ƿ�����
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

bool ClassicModeScene::TankIsok(Vec2 XY)//�жϵ�ǰ�����Ƿ���԰�װ̹��
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

void ClassicModeScene::onMouseDown(cocos2d::EventMouse* event)//������¼�
{    
    if (!isInputEnabled)
        return;
    cocos2d::Vec2 eventXY = event->getLocation();
    eventXY.y = Director::getInstance()->getVisibleSize().height - eventXY.y;
    if (BackGround->getBoundingBox().containsPoint(eventXY))
    {
        Vec2 pii = xytoXY(eventXY);
        if(GameState==false)
            setTank(pii);//��װ̹��
    }
}

void ClassicModeScene::setInputEnabled(bool IsInputEnabled)//������Ϸ�Ƿ�����
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

void ClassicModeScene::ReturnMainScene()//�������˵�
{    
    auto mainMenuScene = MainMenu::createScene();
    auto transition = TransitionFade::create(0.5f, mainMenuScene);
    Director::getInstance()->replaceScene(transition);
    Director::getInstance()->resume();//�ָ���Ϸ����
}

void ClassicModeScene::RestartGame()//�ؿ���Ϸ
{
    auto newClassicModeScene = ClassicModeScene::createScene();//���´�����ǰ����
    auto transition = TransitionFade::create(0.5f, newClassicModeScene);//ƽ������
    Director::getInstance()->replaceScene(transition);//�������´����ĳ���
    Director::getInstance()->resume();//�ָ���Ϸ����
}

void ClassicModeScene::setTank(Vec2 XY)//��װ̹��
{
    if (TankIsok(XY))//�ȼ�⵱ǰλ���Ƿ���԰�װ
    {
        Tank* tmp = Tank::create();
        tmp->setEnemies(Enemies);
        tmp->setPosition(XYtoxy(XY));
        Tanks.push_back(tmp);
        this->addChild(tmp, 2);
    }
}

//�������·����������������������������������������������������������


int ClassicModeScene::getF(MyNode node)
{
    //Ϊ�����������Ȩ��
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
    int rn = rand() % sum + 1;//����Ȩ�����ѡ��һ������
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
    int f = getF(node);//��ȡ��һ�η���
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
    for (int i = 1; i < Path.size() - 2; ++i)//ˢ�µ�ͼ
    {
        tmp = Sprite::create("Path.png");
        this->addChild(tmp, 2);
        tmp->setPosition(XYtoxy(Path[i]));
    }
}
//�������·������������������������������������������������������