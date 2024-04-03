#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
@brief cocos2d的应用程序。

这里的私有继承对Director隐藏了部分接口。
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief 在这里实现Director和Scene init代码。
    @return true 初始化成功，应用继续。
    @return false 初始化失败，应用程序终止。
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief 当应用程序移动到后台时被调用
    @param 应用程序的指针
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief 当应用程序重新进入前台时被调用
    @param 应用程序的指针
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

