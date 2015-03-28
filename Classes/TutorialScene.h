//
//  TutorialScene.h
//  Haneneco
//
//  Created by Hiromitsu Shinohara on 2015/02/18.
//
//

#ifndef Haneneco_TutorialScene_h
#define Haneneco_TutorialScene_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

class TutorialScene : public cocos2d::Layer
{
protected:
    TutorialScene();
    virtual ~TutorialScene();
    bool init() override;
    
    // tutorial終了ボタン
    void tutorialEndButton(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
public:
    static cocos2d::Scene* createScene();
    void onEnterTransitionDidFinish() override;
    CREATE_FUNC(TutorialScene);
};

#endif
