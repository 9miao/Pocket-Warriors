//
//  MainView.h
//  TestGame
//
//  Created by lh on 13-9-11.
//  www.9miao.com
//

#ifndef __TestGame__MainView__
#define __TestGame__MainView__

#include <iostream>
#include "HeaderBase.h"
#include "LoadingLayer.h"
#include "MessageManager.h"
#include "message.h"

class MainView:public CCLayer{
public:
    vector<string> fbtexture;
    int dis;
    bool fflag;
    bool team_flag;
    bool fire_menu_flag;
    bool flmenuFlag;
    
    CCMenu *m_pWelfareMenu;//福利按钮
    CCMenu *m_pFireMenu;//战斗按钮
    CCMenu *m_pTeamMenu;//团队按钮
    CCLayerColor *color;
    CCLabelTTF *namelabel;
    CCLabelTTF *coinLabel;
    CCLabelTTF *goldLabel;
    CCSprite *drag_sp;
    
    CCPoint fpoint;
    LoadingLayer *intensifLayer;
    CCLayer *slayer;
    CCSize size;//窗口大小
    CCMenu *fmenu;
    
    static CCScene * scene();
    virtual bool init();
    void receiveWelfareMsg(CCObject *pObj);//接收消息事件
    void getWelfareMsg();//获取消息事件
    void SecondLayer();
    void setCoin();//初始化金钱信息
    void setRole();//初始化人物信息
    void loadFBTexture();
    CREATE_FUNC(MainView);
    ~MainView();
private:
    int number;
    CCMenu * intensify_Menu;
    bool intensify_Flag;
    void intensifyLoading();
    void enterIntensifyLoading(CCObject *obj);
    void MoveAndEase(CCPoint pos,CCNode * sender);
    void LayerEaseIn(CCNode * Layersender,CCMenu * menu);
    void setLayerVisible();
    virtual void  registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void onExit();
    void onEnter();
};
#endif /* defined(__TestGame__MainView__) */
