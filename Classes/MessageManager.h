//
//  MessageManager.h
//  TestGame
//
//  Created by lh on 14-1-26.
//  www.9miao.com
//

#ifndef __TestGame__MessageManager__
#define __TestGame__MessageManager__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SocketClient.h"
USING_NS_CC;
USING_NS_CC_EXT;
class MessageManager:public CCObject{
public:
    MessageManager();
    ~MessageManager();
    SocketClient *g_pSocket;
    static MessageManager *shareMessageManager();
    void sendLoginMessage();
    void receiveLoginMessage(CCObject *obj);
    void sendPersonal(int chaId);
    void receivePersonal(CCObject *obj);
    void sendRegisterMessage(const char *uname,const char *pwd);
    void receiveRegisterMessage(CCObject *pObj);
    void sendTeamMessage();
    void receiveTeamMessage();
    void sendPetMessage();
    void receivePetMessage(CCObject *pObj);
    void sendBattleMesssage();
    void receiveBattleMessage(CCObject *pObj);
    void sendStrengthenMessage();
    void receiveStrengthenMessage(CCObject *pObj);
    void sendStrenghtenInfoMessage(int pid,int petid,int typeids);
    void receiveStrengthenInfoMessage(CCObject *pObj);
    void sendWelfareMessage();
    void receiveWelfareMessage(CCObject *pObj);
    void sendeGetWelfareMessage(int typeId);
    void receiveGetWelfareMessage(CCObject *pObj);
};
#endif /* defined(__TestGame__MessageManager__) */
