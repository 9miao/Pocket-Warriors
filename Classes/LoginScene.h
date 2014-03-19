//
//  LoginScene.h
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#ifndef __TestGame__LoginScene__
#define __TestGame__LoginScene__

#include <iostream>
#include "HeaderBase.h"
#include "LoadingLayer.h"
#include "SocketClient.h"
#include "RegisterScene.h"
#include "message.h"
class LoginScene:public CCLayer{
public:
    SocketClient *g_pSocket;
    CCSize g_cSize;
    LoadingLayer *m_pLoading;
    
    virtual bool init();
    void loadUI(CCObject *obj);
    static CCScene *scene();
    void addRegisterLayer();
    CREATE_FUNC(LoginScene);
private:
    vector<string> m_vResourceArray;
    int m_nPid ;
    bool m_bRegisterBtn;
    float g_fGap;//点击btn跟锚点之间的差值
    float g_fDis;
    bool m_bLoginFlag;
    CCMenu *m_pLoginMenu;
    CCPoint m_beginPoint;
    CCPoint m_oldPoint;
    CCMenu *m_pRegisterMenu;
    CCLayer *m_pLoadLayer;
    virtual void  registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void onExit();
    ~LoginScene();
    void mainScene();
    void receiveMainScene(CCObject *pObj);
    void MoveAndEase(cocos2d::CCPoint pos,CCNode * sender);
    void LayerEaseIn(CCNode * Layersender);
    void addLoadingLayer();
    void loginMenuCallBack(CCObject * obj);
    void personalBack(CCObject * obj);
    void registerMenuCallBack(CCObject * obj);
};
#endif /* defined(__TestGame__LoginScene__) */
