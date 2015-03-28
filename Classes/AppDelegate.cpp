#include "AppDelegate.h"
#include "TitleScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(1136.0f, 640.0f, ResolutionPolicy::SHOW_ALL);
    /*
    // Cocos2d-xでいい感じのDesignResolutionSizeを指定する
    // http://kyokomi.hatenablog.com/entry/2014/06/22/180213
    const float baseScale  = 1.00f;
    const float baseWidth  = 1136.0f;
    const float baseHeight = 640.0f;
    const float baseScaleWidth  = baseWidth * baseScale;
    const float baseScaleHeight = baseHeight * baseScale;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    const int widthDiff = (int)visibleSize.width % (int)baseWidth;
    const int heightDiff = (int)visibleSize.height % (int)baseHeight;

    if (widthDiff == 0 && heightDiff == 0) {
        // 縦横ともに480x320準拠ならそのまま
        glview->setDesignResolutionSize(baseScaleWidth, baseScaleHeight, ResolutionPolicy::SHOW_ALL);
    } else {
        // 縦横が480x320に準拠しない場合、いい感じにする
        float divX = visibleSize.width / baseWidth;
        float divY = visibleSize.height / baseHeight;
        glview->setDesignResolutionSize(baseScaleWidth + (widthDiff / divX), baseScaleHeight + (heightDiff / divY), ResolutionPolicy::SHOW_ALL);
    }
    // end.
    */

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = TitleScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
