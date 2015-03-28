//
//  TitleScene.h
//  Haneneco
//
//  Created by Hiromitsu Shinohara on 2015/01/24.
//
//

#ifndef Haneneco_TitleScene_h
#define Haneneco_TitleScene_h

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer
{
protected:
    TitleScene();
    virtual ~TitleScene();
    bool init() override;
public:
    static cocos2d::Scene* createScene();
    void onEnterTransitionDidFinish() override;
    CREATE_FUNC(TitleScene);
};

#endif
