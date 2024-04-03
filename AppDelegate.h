#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
@brief cocos2d��Ӧ�ó���

�����˽�м̳ж�Director�����˲��ֽӿڡ�
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief ������ʵ��Director��Scene init���롣
    @return true ��ʼ���ɹ���Ӧ�ü�����
    @return false ��ʼ��ʧ�ܣ�Ӧ�ó�����ֹ��
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief ��Ӧ�ó����ƶ�����̨ʱ������
    @param Ӧ�ó����ָ��
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief ��Ӧ�ó������½���ǰ̨ʱ������
    @param Ӧ�ó����ָ��
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

