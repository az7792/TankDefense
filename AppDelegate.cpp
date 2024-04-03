#include "AppDelegate.h"
#include "MainMenuScene.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
//static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
//static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

// �������Ҫһ����ͬ�ı��������޸�glContextAttrs��ֵ��
// ��ֵ������Ӱ�����е�ƽ̨
void AppDelegate::initGLContextAttrs()
{
    //����OpenGL���������ԣ��졢�̡���������������ȡ�ģ�桢���������
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// �������ʹ�����������������װ������������  
// ��Ҫ�޸Ļ�ɾ���������
static int register_all_packages()
{
    return 0; //���������ı�־
}

bool AppDelegate::applicationDidFinishLaunching() {
    //��ʼ������
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("TankDefense", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("TankDefense");
#endif
        director->setOpenGLView(glview);
    }

    // ������ʾFPS
    director->setDisplayStats(false);

    // ����㲻���������Ĭ��ֵ��1.0/60��
    director->setAnimationInterval(1.0f / 60);

    // ������Ʒֱ���
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
     //�����ܵĸ߶ȴ����еȳߴ�ĸ߶ȡ�
    //if (frameSize.height > mediumResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    //}
    //// �����ܵĸ߶ȴ���С�ߴ�ĸ߶ȡ�
    //else if (frameSize.height > smallResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    //}
    //// �����ܵĸ߶�С���еȳߴ�ĸ߶ȡ�
    //else
    //{        
    //    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    //}

    register_all_packages();

    //����һ������������һ���Զ��ͷŵĶ���
    auto scene = MainMenu::createScene(); 

    // run
    director->runWithScene(scene);//���ڿ�ʼ��Ϸ�����ص�һ��������ֻ���ڵ�һ��������

    return true;
}

// �����������Ӧ�ó��򲻻ʱ�����á�ע�⣬���ӵ�һ���绰ʱ�����ᱻ���á�
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

//��Ӧ�ó����ٴα�����ʱ����������������á�
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
