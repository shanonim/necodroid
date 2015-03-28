//
//  TutorialScene.cpp
//  Haneneco
//
//  Created by Hiromitsu Shinohara on 2015/02/18.
//
//

#include "TutorialScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#define WINSIZE Director::getInstance()->getWinSize()

USING_NS_CC;

Scene* TutorialScene::createScene() {
    auto scene = Scene::create();
    auto layer = TutorialScene::create();
    scene->addChild(layer);
    return scene;
}

TutorialScene::TutorialScene()
{
    
}

TutorialScene::~TutorialScene()
{
    
}

bool TutorialScene::init() {
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto layer = Layer::create();
    ui::ScrollView* scrollView = ui::ScrollView::create();
    
    scrollView->setContentSize(WINSIZE);
    scrollView->setContentSize(Size(WINSIZE.width, WINSIZE.height));
    scrollView->setBackGroundColor(Color3B::BLACK);
    scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    scrollView->setOpacity(200);
    scrollView->setBounceEnabled(true);
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setPosition(Vec2(0, 0));
    layer->addChild(scrollView, 3);
    addChild(layer, 3);
    
    cocos2d::ui::Button* tutorialEndButton = cocos2d::ui::Button::create("images/tutorial.png");
    tutorialEndButton->setPosition(Vec2(WINSIZE.width / 2, WINSIZE.height / 2));
    Size bgsize = tutorialEndButton->getContentSize();
    tutorialEndButton->setScale(WINSIZE.width / bgsize.width);
    tutorialEndButton->addTouchEventListener(CC_CALLBACK_2(TutorialScene::tutorialEndButton, this));
    addChild(tutorialEndButton, 3);
    
    return true;
}

void TutorialScene::tutorialEndButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/start01.mp3");
    
    auto scene = HelloWorld::createScene();
    auto transition = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(transition);
}

void TutorialScene::onEnterTransitionDidFinish()
{
    
}
