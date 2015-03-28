#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

class HelloWorld : public cocos2d::LayerColor
{
protected:
    enum ZOrder
    {
        Z_Background,
        Z_Cat,
        Z_Fish,
        Z_Material,
    };

    enum Tag
    {
        T_Background,
        T_Cat,
        T_Fish,
        T_Material,
        T_LABEL_TIMER,
        T_LABEL_FISH,
        T_COUNTDOWN,
    };

    enum class MaterialType
    {
        roof,
        floor,
        wall,
        fish
    };

    HelloWorld();
    virtual ~HelloWorld();
    bool init() override;

public:
    static cocos2d::Scene* createScene();

    CC_SYNTHESIZE(int, _score, Score);
    CC_SYNTHESIZE(float, _second, Second);
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _cat, Cat);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _scoreLabel, ScoreLabel);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _secondLabel, SecondLabel);

    virtual void onEnter();

    void touchEventInitialize();
    void createBackground(); //背景
    void createCat();
    void createManager();
    void createFish();
    void createMaterial(MaterialType type, cocos2d::Point position, float angle); //オブジェクトの生成
    void removingFish(cocos2d::Node* fish); //魚の消滅
    float _timer; // 経過時間
    float _countDownFlg;
    void gameOver(); // ゲームオーバー
    void countdownLayer(int cnt);

    void showScoreLabel();
    void showSecondLabel();

    // 毎フレーム呼ばれる関数
    void update(float dt) override;

    // Touchイベントの開始地点
    cocos2d::Point touchPoint;

    // Touchイベント用
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    //　ButtonUIのTouchイベント
    void retryButton(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

    // NodeのRectを取得する
    cocos2d::Rect getRect(cocos2d::Node* node);
    
    // 衝突処理
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
