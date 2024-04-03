#pragma once

#include "cocos2d.h"
#include <vector>
#include <utility>
#include "Enemy.h"
#include "Tank.h"
#include "ui/CocosGUI.h"
//#include "InGameMenu.h"

class InGameMenu;  // ǰ������ InGameMenu ��
class FreeModeScene : public cocos2d::Scene
{
public:

    static FreeModeScene* createScene();

    virtual bool init();
    // һ��ѡ�����ص�
    void menuCloseCallback(cocos2d::Ref* pSender);

    void setPath();//�������·������ˢ�µ���ͼ��

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
    CREATE_FUNC(FreeModeScene);
protected:
    int Health = 20;//����Ѫ��
    int EnemyNum = 10;
    int EnemyP = 0;//�����������Vector�е�λ��
    bool GameState = false;//��Ϸ����״̬ falseδ��ʼ true��ʼ��
    int NowEnemyNum = 10;
    int Level = 1;//���˵ȼ�
    cocos2d::Sprite* BackGround;//����
    cocos2d::ui::CheckBox* Start;//��ʼ��ť

    cocos2d::Label* LevelStr;//�ȼ��ı�
    cocos2d::Label* HealthStr;//Ѫ���ı�
    InGameMenu* menuLayer;//�˵�
    bool isInputEnabled = true;//�Ƿ�ʼ��Ϸ
    std::vector < Enemy* >Enemies;//����
    std::vector < Tank* >Tanks;//̹��

    int tx[4] = { 1,0,0,-1 };//����
    int ty[4] = { 0,-1,1 ,0};
    std::vector<cocos2d::Vec2>Path;
};
