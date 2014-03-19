//
//  SelectFB.h
//  TestGame
//
//  Created by lh on 13-11-14.
//  www.9miao.com
//

#ifndef __TestGame__SelectFB__
#define __TestGame__SelectFB__

#include <iostream>
#include "HeaderBase.h"
#include "LoadingLayer.h"
#include "SocketClient.h"

class SelectFB:public CCLayer{
public:
    virtual bool init();
    vector<string> battleRes;
    ~SelectFB();
    void addBattleTexture();
    CCSize size;
    CCString * str;
    char *jsonData;
    std::string str_callback;
    SocketClient * socket;
    LoadingLayer * loading;
    UILayer * layer;
    bool mainBtnFlag;
    float diss;
    UIButton * mainBtn;
    void sendJsonData();
    void receiveJsonData(CCObject *obj);//
    void  setData();
    void initEnemys(int eneId,int i);
    void addLoadingLayer();
    void addUILayer();
    void aaaaa();
    void enterBattleScene(CCObject *obj);
    void UIBtnFadeIn(UIButton* btn,CCLayer * layer);
    void UIBtnFadeOut(UIButton* btn,CCLayer * layer);
    void UIBtnTouchBeganEvent(CCObject * sender);
    void UIBtnTouchMovedEvent(CCObject * sender);
    void UIBtnTouchEndedEvent(CCObject * sender);
    
    CREATE_FUNC(SelectFB);
};
#endif /* defined(__TestGame__SelectFB__) */
