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

// 如果你想要一个不同的背景，请修改glContextAttrs的值。
// 的值，它将影响所有的平台
void AppDelegate::initGLContextAttrs()
{
    //设置OpenGL上下文属性：红、绿、蓝、阿尔法、深度、模版、多采样数。
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// 如果你想使用软件包管理器来安装更多的软件包、  
// 不要修改或删除这个函数
static int register_all_packages()
{
    return 0; //包管理器的标志
}

bool AppDelegate::applicationDidFinishLaunching() {
    //初始化导演
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

    // 开启显示FPS
    director->setDisplayStats(false);

    // 如果你不调用这个，默认值是1.0/60。
    director->setAnimationInterval(1.0f / 60);

    // 设置设计分辨率
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
     //如果框架的高度大于中等尺寸的高度。
    //if (frameSize.height > mediumResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    //}
    //// 如果框架的高度大于小尺寸的高度。
    //else if (frameSize.height > smallResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    //}
    //// 如果框架的高度小于中等尺寸的高度。
    //else
    //{        
    //    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    //}

    register_all_packages();

    //创建一个场景。它是一个自动释放的对象
    auto scene = MainMenu::createScene(); 

    // run
    director->runWithScene(scene);//用于开始游戏，加载第一个场景。只用于第一个场景！

    return true;
}

// 这个函数将在应用程序不活动时被调用。注意，当接到一个电话时，它会被调用。
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

//当应用程序再次被激活时，这个函数将被调用。
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
