#pragma once

#include "cocos2d.h"
#include <vector>
#include <utility>
#include "Enemy.h"
#include "Tank.h"
#include <unordered_map>
#include "ui/CocosGUI.h"
//#include "InGameMenu.h"

class InGameMenu;  // ǰ������ InGameMenu ��
struct MyNode
{
    int x, y, f;//x y ����(0�� 1�� 2��)
};
class ClassicModeScene : public cocos2d::Scene
{
public:

    static ClassicModeScene* createScene();

    virtual bool init();
    // һ��ѡ�����ص�
    void menuCloseCallback(cocos2d::Ref* pSender);

    void setPath();//�������·������ˢ�µ���ͼ��
    void dfs(MyNode node);
    int getF(MyNode node);

    cocos2d::Vec2 xytoXY(cocos2d::Vec2 Postion);//����ת�������ӵ�����
    cocos2d::Vec2 XYtoxy(cocos2d::Vec2 Postion);//��������ת������������
    void CreateEnemy();

    void SetEnemySequence(float dt);//�����˳�ʼ����

    void NextLevel();//������һ��

    void takeDamage(int damage);//�����ؿ�Ѫ

    void ReduceEnemy(Enemy* tmp);//ɾ��һ������

    bool TankIsok(cocos2d::Vec2 XY);//�жϵ�ǰλ���Ƿ���԰�װ̹��

    void setTank(cocos2d::Vec2 XY);//��װһ����̹��

    void CheckEnemy(float dt);//�������Ƿ�ȫ������

    void onMouseDown(cocos2d::EventMouse* event);//����������¼�

    void setInputEnabled(bool IsInputEnabled);//�Ƿ�ʼ��Ϸ

    void ReturnMainScene();//�������˵�

    void RestartGame();//���¿�ʼ��Ϸ

    //�ֶ�ʵ��"��̬����() "����
   CREATE_FUNC(ClassicModeScene);
protected:
   int Health = 20;//����Ѫ��
   int EnemyNum = 10;
   int EnemyP = 0;//�����������Vector�е�λ��
   bool GameState = false;//��Ϸ����״̬ falseδ��ʼ true��ʼ��
   int NowEnemyNum =10;
   int Level=1;//���˵ȼ�
   cocos2d::Sprite* BackGround;//����
   cocos2d::ui::CheckBox* Start;//��ʼ��ť

   cocos2d::Label *LevelStr;//�ȼ��ı�
   cocos2d::Label *HealthStr;//Ѫ���ı�
   InGameMenu* menuLayer;//�˵�
   bool isInputEnabled = true;//�Ƿ�ʼ��Ϸ
   std::vector < Enemy* >Enemies;//����
   std::vector < Tank* >Tanks;//̹��
   
   int tx[3] = { 1,0,0 };//����
   int ty[3] = { 0,-1,1 };
   int EndX = 0;
   MyNode p = { 0,0,0 };
   std::vector<cocos2d::Vec2>Path;
};
