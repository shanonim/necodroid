//
//  TitleScene.cpp
//  Haneneco
//
//  Created by Hiromitsu Shinohara on 2015/01/24.
//
//

#include "TitleScene.h"
#include "TutorialScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

const char* SKIP_TUTORIAL = "skipTutorial";

Scene* TitleScene::createScene() {
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

TitleScene::TitleScene()
{
    
}

TitleScene::~TitleScene()
{
    
}

bool TitleScene::init() {
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto director = Director::getInstance();
    auto winSize = director->getWinSize();
    
    // 背景を設置
    auto background = Sprite::create("images/mainTitle.png");
    background->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    Size bgsize = background->getContentSize();
    background->setScale(winSize.width / bgsize.width);
    this->addChild(background);
    
    auto listener = EventListenerTouchOneByOne::create();
    auto tutorialFlg = UserDefault::getInstance()->getIntegerForKey(SKIP_TUTORIAL);
    
    if (tutorialFlg != 1) {
        UserDefault::getInstance()->setIntegerForKey(SKIP_TUTORIAL, 1);
        listener->onTouchBegan = [this](Touch* touch, Event* event) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/start01.mp3");
            this->getEventDispatcher()->removeAllEventListeners();
            auto delay = DelayTime::create(0.5);
            auto startGame = CallFunc::create([]{
                auto scene = TutorialScene::createScene();
                auto transition = TransitionFade::create(0.5f, scene);
                Director::getInstance()->replaceScene(transition);
            });
            this->runAction(Sequence::create(delay, startGame, NULL));
            return true;
        };
    } else {
        listener->onTouchBegan = [this](Touch* touch, Event* event) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/start01.mp3");
            this->getEventDispatcher()->removeAllEventListeners();
            auto delay = DelayTime::create(0.5);
            auto startGame = CallFunc::create([]{
                auto scene = HelloWorld::createScene();
                auto transition = TransitionFade::create(0.5f, scene);
                Director::getInstance()->replaceScene(transition);
            });
            this->runAction(Sequence::create(delay, startGame, NULL));
            return true;
        };
    }
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void TitleScene::onEnterTransitionDidFinish()
{
    
}
