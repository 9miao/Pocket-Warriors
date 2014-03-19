//
//  Strengthen.h
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#ifndef __TestGame__Strengthen__
#define __TestGame__Strengthen__

#include <iostream>
#include "HeaderBase.h"
#include "SocketClient.h"
#include "LoadingLayer.h"
#include "SimpleAudioEngine.h"
#include "CCScrollViewX.h"

class Strengthen:public CCLayer {
public:
    static CCScene *scene();
    SocketClient *socket;
    CCScrollViewX *scrollview;
    bool scrollFlag;
    void initLoadingLayer();//add 加载页面
    void changeScene();//更换场景 回到主界面
    void EaseInAndOut();//切换动画
    LoadingLayer *loadlayer;
    CCPoint m_beginPoint;
    CCMenu *goback_menu;
    float goback_dis;
    bool gobackflag;
    int numflag;
    int oldnumflag;
    bool hasRole;
    void getStrengt();//发送强化界面数据
    void initBaseProperty();//初始化强化界面
    void analyzeData(std::string data);//解析数据
    void setStentgt(CCObject *obj);//设置数据
    void sendStrengthen(CCObject *obj);//发送强化数据
    void reciveStrengthen(CCObject *obj);//接收强化数据
    void receiveNotice(CCObject *obj);
    virtual bool init();
    const char *json_strs;
    UILayer *layer;
    CCArray *strengtenInfo;
    CCPoint m_touchPoint;
    CCPoint m_touchOffset;
    int m_nCurPage;
    CSJson::Value teamInfo;
    int  m_nPageCount;
    void finishedInit(CCObject *obj);//初始化完成
    virtual void  registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    ~Strengthen();
    CREATE_FUNC(Strengthen);
private:
    CCSize size;
};
class StengthenList:public CCObject{
public:
    int djcostMoney;//本次强化（渡劫）消耗金钱
    int hpcostMoney;//强化生命到下一等级所需金钱 0表示已到上限 1表示不变
    int attcostMoney;//强化攻击到下一等级所需金钱   规则同上
    int addHp;//本次操作生命所加属性
    int addAtt;//本次操作攻击所加属性
    int luck;//幸运值
    int newfighting;//强化后小队战斗力  0表示不变
    int newlevellimit;//渡劫后强化等级上限 0表示不变
    int newcolor;//渡劫后的名字颜色 0表示不变 1白 2蓝 3紫 4橙
    void setNewProperties(int djmoney,int hpmoney1,int attmoney1,int addhp,int addatt,int luck,int newfighting,int newlevellimit,int newcolor);
    std::string name;//伙伴名称
    int level;//当前强化等级
    int levellimit;//强化等级上限
    int resource;//资源id
    int headid;//头像id
    int color;//名称颜色 1白 2蓝 3紫 4橙
    int hp;//强化增加的声明
    int hpmoney;//强化生命到下一级所需金钱 0表示达到上限
    int att;//强化增加的攻击
    int attmoney;//强化攻击到下一等级所需要的金钱  0表示达到上限
    int status;//渡劫所需金钱  0表示已到上限
    int ppid;
    StengthenList();
    ~StengthenList();
};
#endif /* defined(__TestGame__Strengthen__) */
