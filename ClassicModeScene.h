#pragma once

#include "cocos2d.h"
#include <vector>
#include <utility>
#include "Enemy.h"
#include "Tank.h"
#include <unordered_map>
#include "ui/CocosGUI.h"
//#include "InGameMenu.h"

class InGameMenu;  // 前向声明 InGameMenu 类
struct MyNode
{
    int x, y, f;//x y 方向(0下 1左 2右)
};
class ClassicModeScene : public cocos2d::Scene
{
public:

    static ClassicModeScene* createScene();

    virtual bool init();
    // 一个选择器回调
    void menuCloseCallback(cocos2d::Ref* pSender);

    void setPath();//生成随机路径，并刷新到地图上
    void dfs(MyNode node);
    int getF(MyNode node);

    cocos2d::Vec2 xytoXY(cocos2d::Vec2 Postion);//坐标转换到格子的坐标
    cocos2d::Vec2 XYtoxy(cocos2d::Vec2 Postion);//格子坐标转换到世界坐标
    void CreateEnemy();

    void SetEnemySequence(float dt);//给敌人初始动作

    void NextLevel();//进入下一个

    void takeDamage(int damage);//给基地扣血

    void ReduceEnemy(Enemy* tmp);//删除一个敌人

    bool TankIsok(cocos2d::Vec2 XY);//判断当前位置是否可以安装坦克

    void setTank(cocos2d::Vec2 XY);//安装一个新坦克

    void CheckEnemy(float dt);//检测敌人是否全部死亡

    void onMouseDown(cocos2d::EventMouse* event);//鼠标左键点击事件

    void setInputEnabled(bool IsInputEnabled);//是否开始游戏

    void ReturnMainScene();//返回主菜单

    void RestartGame();//重新开始游戏

    //手动实现"静态创建() "方法
   CREATE_FUNC(ClassicModeScene);
protected:
   int Health = 20;//基地血量
   int EnemyNum = 10;
   int EnemyP = 0;//待处理敌人在Vector中的位置
   bool GameState = false;//游戏运行状态 false未开始 true开始中
   int NowEnemyNum =10;
   int Level=1;//敌人等级
   cocos2d::Sprite* BackGround;//背景
   cocos2d::ui::CheckBox* Start;//开始按钮

   cocos2d::Label *LevelStr;//等级文本
   cocos2d::Label *HealthStr;//血量文本
   InGameMenu* menuLayer;//菜单
   bool isInputEnabled = true;//是否开始游戏
   std::vector < Enemy* >Enemies;//敌人
   std::vector < Tank* >Tanks;//坦克
   
   int tx[3] = { 1,0,0 };//方向
   int ty[3] = { 0,-1,1 };
   int EndX = 0;
   MyNode p = { 0,0,0 };
   std::vector<cocos2d::Vec2>Path;
};
