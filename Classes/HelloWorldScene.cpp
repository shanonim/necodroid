#include <iostream>
#include <random>

#include "HelloWorldScene.h"
#include "TitleScene.h"
#include "SimpleAudioEngine.h"

#define forceValue 2
#define X_FORCE 350
#define Y_FORCE 350

#define kModalLayerPriority -1
#define WINSIZE Director::getInstance()->getWinSize()

USING_NS_CC;

const float COUNTDOWN_TO_START = 4;
const float TIME_LIMIT_SECOND = 20;
const char* HISCORE_KEY = "hiscoreKey";
const char* SCORE_KEY = "scoreKey";
const char* GAME_CATEGORY = "Haneneco";

int gameOverFlg;

HelloWorld::HelloWorld()
: _cat(NULL)
, _score(0)
, _scoreLabel(NULL)
, _second(COUNTDOWN_TO_START)
, _secondLabel(NULL)
{

}

HelloWorld::~HelloWorld()
{
    CC_SAFE_RELEASE_NULL(_cat);
    CC_SAFE_RELEASE_NULL(_scoreLabel);
    CC_SAFE_RELEASE_NULL(_secondLabel);
}

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();

    //物理オブジェクトを可視化するデバッグモード
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    // 画面の初期色変更
    if(!LayerColor::initWithColor(Color4B(204, 204, 204, 204)))
    {
    return false;
    }

    // カウント
    scheduleUpdate();
    // ラベル表示
    showScoreLabel();
    showSecondLabel();

    return true;
}

void HelloWorld::onEnter()
{
    Layer::onEnter();

    //重力セット
    Vect gravity;
    gravity.setPoint(0, -50);
    auto scene = dynamic_cast<Scene*>(this->getParent());
    scene->getPhysicsWorld()->setGravity(gravity);
    scene->getPhysicsWorld()->setSpeed(6.0f);

    //反発係数・摩擦係数
    auto material = PHYSICSBODY_MATERIAL_DEFAULT;
    material.restitution = 2.0f;
    material.friction = 0.0f;

    // 背景の生成
    createBackground();
    createManager();
}

void HelloWorld::touchEventInitialize()
{
    //衝突判定
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 10);

    // Touchイベント用
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::showScoreLabel()
{
    TTFConfig config("fonts/SouciSansNF.ttf", 48);
    auto scoreLabel = Label::createWithTTF(config, StringUtils::format("Fish: %d", _score), TextHAlignment::CENTER);
    scoreLabel->setColor(Color3B::BLACK);
    scoreLabel->setPosition(WINSIZE.width - 200, WINSIZE.height -100);
    scoreLabel->setVisible(false);
    this->setScoreLabel(scoreLabel);
    this->addChild(_scoreLabel, 2);
}

void HelloWorld::showSecondLabel()
{
    int second = static_cast<int>(_second);
    TTFConfig config("fonts/SouciSansNF.ttf", 48);
    auto secondLabel = Label::createWithTTF(config, StringUtils::format("Time: %d", second), TextHAlignment::CENTER);
    secondLabel->setColor(Color3B::BLACK);
    secondLabel->setPosition(WINSIZE.width - 200, WINSIZE.height -200);
    secondLabel->setVisible(false);
    this->setSecondLabel(secondLabel);
    this->addChild(_secondLabel, 2);
}

void HelloWorld::createBackground()
{
    auto background = Sprite::create("images/background.png");
    background->setPosition(Point(WINSIZE.width / 2, WINSIZE.height / 2));
    background->setTag(T_Background);
    Size bgsize = background->getContentSize();
    background->setScale(WINSIZE.width / bgsize.width);
    addChild(background, 1);
}

void HelloWorld::createCat()
{
    //猫画像
    setCat(Sprite::create("images/cat.png"));
    _cat->setPosition(Point(WINSIZE.width / 6 , WINSIZE.height / 2));

    PhysicsBody* pCat = PhysicsBody::createCircle(40);
    pCat->setMass(1.0f);
    pCat->setContactTestBitmask(1);
    _cat->setPhysicsBody(pCat);
    _cat->setTag(T_Cat);
    addChild(_cat, 1);
}

void HelloWorld::createManager()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> randw(100, visibleSize.width - 100);
    std::uniform_int_distribution<> randh(400, visibleSize.height- 100);

    createMaterial(MaterialType::roof, Point(visibleSize.width /2 , visibleSize.height), 0);
    createMaterial(MaterialType::floor, Point(visibleSize.width /2 , 0), 0);
    createMaterial(MaterialType::wall, Point(0, visibleSize.height/2), 0);
    createMaterial(MaterialType::wall, Point(visibleSize.width, visibleSize.height/2), 0);
    
    createMaterial(MaterialType::fish, Point(randw(mt), randh(mt)), 0);
    createMaterial(MaterialType::fish, Point(randw(mt), randh(mt)), 0);
    createMaterial(MaterialType::fish, Point(randw(mt), randh(mt)), 0);
    createMaterial(MaterialType::fish, Point(randw(mt), randh(mt)), 0);
    createMaterial(MaterialType::fish, Point(randw(mt), randh(mt)), 0);
}

void HelloWorld::createFish()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> randw(100, visibleSize.width - 100);
    std::uniform_int_distribution<> randh(200, visibleSize.height- 100);

    createMaterial(MaterialType::fish, Point(randw(mt), randh(mt)), 0);
}

void HelloWorld::createMaterial(MaterialType type, Point position, float angle)
{
    std::string fileName;

    switch (type) {
        case MaterialType::roof:
        {
            fileName = "images/roof01.png";
            break;
        }
        case MaterialType::floor:
        {
            fileName = "images/floor01.png";
            break;
        }
        case MaterialType::wall:
        {
            fileName = "images/wall.png";
            break;
        }
        case MaterialType::fish:
        {
            fileName = "images/fish.png";
            break;
        }
        default:
            break;
    }
    // オブジェクトの生成
    auto material = Sprite::create(fileName.c_str());
    material->setPosition(position);
    material->setRotation(angle);
    material->setTag(T_Material);

    // 魚の生成
    auto fish = Sprite::create(fileName.c_str());
    fish->setPosition(position);
    fish->setRotation(angle);
    fish->setTag(T_Fish);
    
    PhysicsBody* body;
    
    switch (type) {
        case MaterialType::roof:
        {
            body = PhysicsBody::createBox(material->getContentSize(), PhysicsMaterial(0.5, 0.5, 0.3));
            body->setDynamic(false);

            Size bgsize = material->getContentSize();
            material->setScale(WINSIZE.width / bgsize.width);
            material->setPhysicsBody(body);
            addChild(material, (int)ZOrder::Z_Material);
            break;
        }
        case MaterialType::floor:
        {
            body = PhysicsBody::createBox(material->getContentSize(), PhysicsMaterial(0.5, 0.5, 0.3));
            body->setDynamic(false);

            Size bgsize = material->getContentSize();
            material->setScale(WINSIZE.width / bgsize.width);
            material->setPhysicsBody(body);
            addChild(material, (int)ZOrder::Z_Material);
            break;
        }
        case MaterialType::wall:
        {
            body = PhysicsBody::createBox(material->getContentSize(), PhysicsMaterial(0.5, 0.5, 0.3));
            body->setDynamic(false);

            material->setPhysicsBody(body);
            addChild(material, (int)ZOrder::Z_Material);
            break;
        }
        case MaterialType::fish:
        {
            body = PhysicsBody::createBox(fish->getContentSize(), PhysicsMaterial(0.5, 0.5, 0.3));
            body->setContactTestBitmask(1);
            body->setDynamic(false);

            fish->setPhysicsBody(body);
            fish->setOpacity(0);
            addChild(fish, (int)ZOrder::Z_Fish);

            auto action = FadeIn::create(0.5);
            fish->runAction(action);
            break;
        }
        default:
            break;
    }
}

Rect HelloWorld::getRect(Node* node)
{
    Point point = node->getPosition();
    int width = node->getContentSize().width;
    int height = node->getContentSize().height;
    return Rect(point.x - (width / 2), point.y - (height / 2), width, height);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    Sprite* touchBegan = (Sprite*)this->getChildByTag(T_Background);
    Rect touchBeganRect = getRect(touchBegan);
    touchPoint = touch->getLocation();
    return touchBeganRect.containsPoint(touchPoint);
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    Sprite* character = (Sprite*)this->getChildByTag(T_Cat);
    Point endPoint = touch->getLocation();

    Vect force = Vect(touchPoint.x - endPoint.x, touchPoint.y - endPoint.y) * forceValue;
    if(force.x > X_FORCE){
        force.x = X_FORCE;
    }
    if(force.x < -X_FORCE)
        force.x = -X_FORCE;

    if(force.y > Y_FORCE){
        force.y = Y_FORCE;
    }
    if(force.y < -Y_FORCE){
        force.y = -Y_FORCE;
    }

    character->getPhysicsBody()->applyImpulse(force);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/jump01.mp3");

    // パーティクル生成
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("particles/catReleaseParticle.plist");
    particle->resetSystem();
    particle->setPosition(_cat->getPosition());
    addChild(particle, Z_Fish);

    log("force.x: %f", force.x);
    log("force.y: %f", force.y);
}

bool HelloWorld::onContactBegin(PhysicsContact& contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();
    
    if(bodyA->getNode() && bodyB->getNode())
    {
        //猫の取得
        PhysicsBody* cat = nullptr;

        if(bodyA->getNode()->getTag() == T_Cat)
        {
            cat = bodyA;
        }
        else if (bodyB->getNode()->getTag() == T_Cat)
        {
            cat = bodyB;
        }

        //魚の取得
        Node* fish = nullptr;
        PhysicsBody* object = nullptr;

        if(bodyA->getNode()->getTag() == T_Fish)
        {
            fish = bodyA->getNode();
            object = bodyB;
        }
        else if (bodyB->getNode()->getTag() == T_Fish)
        {
            fish = bodyB->getNode();
            object = bodyA;
        }

        removingFish(fish);
    }
    return true;
}

void HelloWorld::removingFish(Node* fish)
{
    // スコア加算
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/kotsudumi01.mp3");
    _score += 1;
    _scoreLabel->setString(StringUtils::format("Fish: %d", _score));

    // スプライト消去
    auto removingFish = Sprite::create("images/fish.png");
    removingFish->setPosition(fish->getPosition());
    addChild(removingFish, Z_Fish);

    // パーティクル生成
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("particles/contactParticle.plist");
    particle->resetSystem();
    particle->setPosition(fish->getPosition());
    addChild(particle, Z_Fish);

    auto sequenceForFish = Sequence::create(ScaleTo::create(0.3, 0), RemoveSelf::create(), nullptr);
    removingFish->runAction(sequenceForFish);
    fish->removeFromParent();

    createFish();
}

void HelloWorld::update(float dt)
{
    if (gameOverFlg != 1) {
        _second -= dt;
        int second = static_cast<int>(_second);
        _secondLabel->setString(StringUtils::format("Time: %d", second));

        if (_second < 4 && _second > 3) {
            countdownLayer(3);
        } else if (_second < 3 && _second > 2) {
            countdownLayer(2);
        } else if (_second < 2 && _second > 1) {
            countdownLayer(1);
        } else if (_second < 1) {
            removeChildByTag(T_COUNTDOWN);
            _second = TIME_LIMIT_SECOND + 1;
            _secondLabel->setVisible(true);
            _scoreLabel->setVisible(true);
            touchEventInitialize();
            createCat();
            gameOverFlg = 1;
        }
    } else {
        _second -= dt;
        int second = static_cast<int>(_second);
        _secondLabel->setString(StringUtils::format("Time: %d", second));

        if (_second < 4 && _second > 3) {
            countdownLayer(3);
        } else if (_second < 3 && _second > 2) {
            countdownLayer(2);
        } else if (_second < 2 && _second > 1) {
            countdownLayer(1);
        } else if (_second < 1) {
            unscheduleUpdate();
            gameOver();
        }
    }
}

void HelloWorld::countdownLayer(int cnt)
{
    if (cnt == 3) {
        if (_countDownFlg != 3) {
            TTFConfig config("fonts/SouciSansNF.ttf", 144);
            auto countdownLabel = Label::createWithTTF(config, StringUtils::format("%d", cnt), TextHAlignment::CENTER);
            countdownLabel->setColor(Color3B::BLACK);
            countdownLabel->setPosition(WINSIZE.width / 2.0, WINSIZE.height / 2.0);
            countdownLabel->setTag(T_COUNTDOWN);
            addChild(countdownLabel, 2);
            _countDownFlg = 3;
        }
    } else if (cnt == 2) {
        if (_countDownFlg != 2) {
            removeChildByTag(T_COUNTDOWN);
            TTFConfig config("fonts/SouciSansNF.ttf", 144);
            auto countdownLabel = Label::createWithTTF(config, StringUtils::format("%d", cnt), TextHAlignment::CENTER);
            countdownLabel->setColor(Color3B::BLACK);
            countdownLabel->setPosition(WINSIZE.width / 2.0, WINSIZE.height / 2.0);
            countdownLabel->setTag(T_COUNTDOWN);
            addChild(countdownLabel, 2);
            _countDownFlg = 2;
        }
    } else if (cnt == 1) {
        if (_countDownFlg != 1) {
            removeChildByTag(T_COUNTDOWN);
            TTFConfig config("fonts/SouciSansNF.ttf", 144);
            auto countdownLabel = Label::createWithTTF(config, StringUtils::format("%d", cnt), TextHAlignment::CENTER);
            countdownLabel->setColor(Color3B::BLACK);
            countdownLabel->setPosition(WINSIZE.width / 2.0, WINSIZE.height / 2.0);
            countdownLabel->setTag(T_COUNTDOWN);
            addChild(countdownLabel, 2);
            _countDownFlg = 1;
        }
    }
}

void HelloWorld::gameOver()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/cat.mp3");
    _cat->setVisible(false);
    removeChildByTag(T_COUNTDOWN);
    gameOverFlg = 0;
    auto layer = Layer::create();
    auto size = Director::getInstance()->getVisibleSize();

    ui::ScrollView* sc = ui::ScrollView::create();
    sc->setContentSize(size);
    sc->setContentSize(Size(WINSIZE.width, WINSIZE.height));

    sc->setBackGroundColor(Color3B::BLACK);
    sc->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    sc->setOpacity(200);
    sc->setBounceEnabled(true);
    sc->setDirection(ui::ScrollView::Direction::VERTICAL);
    sc->setPosition(Vec2(0, 0));
    layer->addChild(sc, 3);
    addChild(layer, 3);

    cocos2d::ui::Button* retryButton = cocos2d::ui::Button::create("images/retryButton.png");
    retryButton->setPosition(Vec2(WINSIZE.width / 2, WINSIZE.height / 2));
    retryButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::retryButton, this));
    addChild(retryButton, 3);

    // 結果を設置
    auto userDefault = UserDefault::getInstance();
    userDefault->setIntegerForKey(SCORE_KEY, _score);
    int hiscore = userDefault->getIntegerForKey(HISCORE_KEY);
    if (_score > hiscore) {
        userDefault->setIntegerForKey(HISCORE_KEY, _score);

        TTFConfig config("fonts/SouciSansNF.ttf", 72);
        auto hiscoreLabel = Label::createWithTTF(config, "HISCORE!!", TextHAlignment::CENTER);
        hiscoreLabel->setPosition(Vec2(WINSIZE.width - 200, WINSIZE.height - 100));
        hiscoreLabel->setColor(Color3B::RED);
        this->addChild(hiscoreLabel, 4);
    }
    auto currentScore = userDefault->getIntegerForKey(SCORE_KEY);
    auto currentHiscore = userDefault->getIntegerForKey(HISCORE_KEY);

    TTFConfig config("fonts/SouciSansNF.ttf", 72);
    auto resultLabel = Label::createWithTTF(config, StringUtils::format("Score: %d", currentScore), TextHAlignment::CENTER);
    resultLabel->setPosition(Vec2(WINSIZE.width / 2.0, WINSIZE.height - 100));
    resultLabel->setColor(Color3B::WHITE);
    this->addChild(resultLabel, 4);

    auto hiscoreResultLabel = Label::createWithTTF(config, StringUtils::format("Hiscore: %d", currentHiscore), TextHAlignment::CENTER);
    hiscoreResultLabel->setPosition(Vec2(WINSIZE.width / 2.0, WINSIZE.height - 180));
    hiscoreResultLabel->setColor(Color3B::WHITE);
    this->addChild(hiscoreResultLabel, 4);
}

void HelloWorld::retryButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/start01.mp3");
    auto scene = TitleScene::createScene();
    auto transition = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(transition);
}
