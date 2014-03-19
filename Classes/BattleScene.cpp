//
//  BattleScene.cpp
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#include "BattleScene.h"
#include "Voiceover.h"
#include "message.h"

CCScene * BattleScene::scene(){
    CCScene * scene= CCScene :: create();
    CCLayer * layer= BattleScene::create();
    scene->addChild(layer);
    return scene;
}
bool BattleScene::init(){
    if (!CCLayer::init()) {
        return false;
        
    }

    line= BattleLayer::create();
    this->addChild(line);
    socket=GameData::shareGameData()->socket;
    sendEnterBattle();
    
        return true;
}

bool HeadLayer::init(){
    if(!CCLayer::init()){
        return false;
    }
    
    
    return true;
}
void BattleScene::sendEnterBattle(){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=GameData::shareGameData()->chaId;
    std::string str=writer.write(root);
    Message * msg= (Message *)socket->constructMessage(str, CMID_GETPERMITENTERBATTLE);
    socket->sendMessage_(msg, false);
    this->schedule(schedule_selector(BattleScene::receiveEnterBattle));
}
void BattleScene::receiveEnterBattle(){
    Message * msg= (Message *)GameData::shareGameData()->msg_dic->objectForKey(CMID_GETPERMITENTERBATTLE);
    if (msg) {
        this->unschedule(schedule_selector(BattleScene::receiveEnterBattle));
        GameData::shareGameData()->msg_dic->removeObjectForKey(CMID_GETPERMITENTERBATTLE);
    }
    
    
    
}

void BattleScene::onExit(){
    CCLayer::onExit();

}
