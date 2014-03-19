//
//  TeamScene.h
//  KouDaiXianXia
//
//  Created by lh on 14-3-11.
//  www.9miao.com
//

#ifndef __KouDaiXianXia__TeamScene__
#define __KouDaiXianXia__TeamScene__

#include <iostream>
#include "HeaderBase.h"
#include "CCScrollViewX.h"
class HeadIcon:public CCSprite{
public:
    virtual bool init();
    CCSprite *icon;
    int petId;
    bool lockFlag;
    int m_nLevel;
    int m_nHp;
    int m_nAtt;
    
    bool touchEvent(CCTouch *pTouch);
    CREATE_FUNC(HeadIcon);
};
class TeamScene:public CCLayer{
public:
    virtual bool init();
    void addScrollView();
    float dragX;
    float dragY;
    bool moveflag;
    CCDictionary *allPet;
    HeadIcon *touchIcon;
    HeadIcon *touchHead;
    CCPoint oldPoint;
    UILayer *m_pUILayer;
    int currentCount;
    int totalCount;
    CCScrollViewX *scrollview;
    CCSize winSize;
    CCArray *pRoleArray;
    void initProperties(float dt);
    string petData;
    static CCScene *scene();
    void analyzePetMessage();
    void receivePetMessage(CCObject *sender);
    void receiveNoticFunc(CCObject *sender);
    void initHeadIcon();
    virtual void  registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    ~TeamScene();
    CREATE_FUNC(TeamScene);
};

#endif /* defined(__KouDaiXianXia__TeamScene__) */
