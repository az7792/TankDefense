#include "FreeModeScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include <ctime>
#include <vector>
#include <queue>
#include <cstdlib>
#include <string>
#include "InGameMenu.h"  // ��������� InGameMenu ���ͷ�ļ�

USING_NS_CC;


FreeModeScene* FreeModeScene::createScene()
{
    return FreeModeScene::create();    
}

// ��ӡ���õĴ�����Ϣ�����������ļ�������ʱ����������ϡ�
static void problemLoading(const char* filename)
{
    printf("����ʱ����%s\n", filename);
    printf("������ı��뷽ʽ���������Ҫ��HelloWorldScene.cpp���ļ���ǰ����'Resources/'��\n");
}


// �� "init "������Ҫ��ʼ�����ʵ��
bool FreeModeScene::init()
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
    listener->onMouseDown = CC_CALLBACK_1(FreeModeScene::onMouseDown, this);
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
            this->schedule(CC_SCHEDULE_SELECTOR(FreeModeScene::CheckEnemy), 0.1f);//�򿪼������Ƿ�ȫ�������Ŀ���

            Start->setTouchEnabled(false);//��ѡ������Ϊ���ɻ���
            GameState = true;//��Ϸ״̬Ϊ������
            //ˢ�µ���
            this->schedule(CC_SCHEDULE_SELECTOR(FreeModeScene::SetEnemySequence), 0.2f, EnemyNum-1, 0.0f);//�����е�����Ӷ���
            break;
        default:
            break;
        }
        });

    //this->schedule(CC_SCHEDULE_SELECTOR(FreeModeScene::CheckEnemy), 0.1f);//ÿ0.1��������Ƿ�ȫ������

    menuLayer = InGameMenu::create();//�˵�
    menuLayer->setVisible(false);//�˵����ɼ�
    menuLayer->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
    this->addChild(menuLayer,10);
    menuLayer->setFreeModeScene(this);//���뵱ǰ��������˵����и��ֲ���

    ui::Button*  GameMenu = ui::Button::create("InGameMenu.png", "InGameMenu.png");//�˵���ť
    GameMenu->setPosition(XYtoxy(Vec2(13, 7)));
    GameMenu->addClickEventListener([&](Ref* sender) {//��ť�����Ļص�����
        menuLayer->OpenMenu();//�򿪲˵�
        });
    this->addChild(GameMenu, 2);


    return true;
}



void FreeModeScene::menuCloseCallback(Ref* pSender)
{
    //�ر�cocos2d-x��Ϸ�������˳�Ӧ�ó���
    Director::getInstance()->end();

    /*Ϊ���ڲ��˳�Ӧ�ó��������µ�����ԭ��iOS��Ļ��������ڵĻ�������Ҫʹ�����������Director::getInstance()->end()�����Ǵ���һ����RootViewController.mm�д������Զ����¼���������ʾ*/

        //EventCustom customEndEvent("game_scene_close_event")��
        //_eventDispatcher->dispatchEvent��&customEndEvent����
}


Vec2 FreeModeScene::xytoXY(Vec2 Postion)//��������ת��������
{
    Rect anchorPoint = BackGround->getBoundingBox();
    //����91*91
    Vec2 XY;
    XY.x = (int)((Postion.x - anchorPoint.origin.x)/91) + 1;
    XY.y = (int)((Postion.y - anchorPoint.origin.y)/91) + 1;
    return XY;
}

Vec2 FreeModeScene::XYtoxy(Vec2 Postion)//��������ת��������
{
    Rect anchorPoint = BackGround->getBoundingBox();
    return Vec2(anchorPoint.origin.x + 91 * Postion.x - 91/2, anchorPoint.origin.y + 91 * Postion.y - 91 / 2);
}

void FreeModeScene::CreateEnemy()//��������
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

void FreeModeScene::SetEnemySequence(float dt)//������[EnemyP]��Ӷ���
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

void FreeModeScene::NextLevel()//������һ��
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

void FreeModeScene::takeDamage(int damage)//�����ؿ�Ѫ
{
    Health -= damage;
    HealthStr->setString("Health:" + std::to_string(Health));
    if (Health <= 0)//����������
    {
        //�����������������¼�        
        CCLOG("gameover");
    }
}

void FreeModeScene::ReduceEnemy(Enemy *tmp)//ɱ��ָ������
{    
    tmp->setVisible(false);
}

void FreeModeScene::CheckEnemy(float dt)//�������Ƿ�ȫ�������������Ƿ�����
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
            tmpMap[i][j] = 0; //�յ�
        }
    }

    for (int i = 1; i < Path.size() - 2; ++i)
    {
        if (Path[i] == XY)
        {
            f = false;
        }
       // tmpMap[(int)Path[i].x][(int)Path[i].y] = 1; //�ϰ���
    }

    for (auto v : Tanks)
    {
        Vec2 nowxy = xytoXY(v->getPosition());
        if (nowxy == XY)
        {
            f = false;
        }
        tmpMap[(int)nowxy.x][(int)nowxy.y] = 1; //�ϰ���
    }

    if (f)
    {
        return f; //̹�˰�װ�ڷ�·����̹���ϣ�����ֱ�Ӱ�װ��������������
    }

    tmpMap[(int)XY.x][(int)XY.y] = 1; //���赱ǰ���԰�װ̹��

    //BFS
    std::vector<cocos2d::Vec2> tmpPath; //��¼·��
    Vec2 path[13][8];                 //��¼��ǰ���Ǵ��Ǹ��������

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
                // �����յ�
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

void FreeModeScene::onMouseDown(cocos2d::EventMouse* event)//������¼�
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

void FreeModeScene::setInputEnabled(bool IsInputEnabled)//������Ϸ�Ƿ�����
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

void FreeModeScene::ReturnMainScene()//�������˵�
{    
    auto mainMenuScene = MainMenu::createScene();
    auto transition = TransitionFade::create(0.5f, mainMenuScene);
    Director::getInstance()->replaceScene(transition);
    Director::getInstance()->resume();//�ָ���Ϸ����
}

void FreeModeScene::RestartGame()//�ؿ���Ϸ
{
    auto newFreeModeScene = FreeModeScene::createScene();//���´�����ǰ����
    auto transition = TransitionFade::create(0.5f, newFreeModeScene);//ƽ������
    Director::getInstance()->replaceScene(transition);//�������´����ĳ���
    Director::getInstance()->resume();//�ָ���Ϸ����
}

void FreeModeScene::setTank(Vec2 XY)//��װ̹��
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




void FreeModeScene::setPath()//��ʼ·��
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