//
//  BattleScene.h
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#ifndef __TestGame__BattleScene__
#define __TestGame__BattleScene__

#include <iostream>
#include "HeaderBase.h"
#include "SocketClient.h"
#include "BattleLayer.h"
class BattleScene:public CCLayer{
public:
    static CCScene * scene();
    SocketClient * socket;
    virtual bool init();
    void sendEnterBattle();//发送进入战场信息
    void receiveEnterBattle();//接收进入战场信息
    CCLayerColor *voiceoverLayer;
    BattleLayer *line;
    void onExit();
    CREATE_FUNC(BattleScene);
    
};
class HeadLayer:public CCLayer{
public:
    virtual bool init();
    CREATE_FUNC(HeadLayer);
};
#endif /* defined(__TestGame__BattleScene__) */
