//
//  MessageManager.cpp
//  TestGame
//
//  Created by lh on 14-1-26.
//  www.9miao.com
//

#include "MessageManager.h"
#include "ResourceName.h"
#include "message.h"
#include "GameData.h"
static MessageManager * messageManager=NULL;
MessageManager::MessageManager(){
    g_pSocket=new SocketClient(ip,ports,1,1,NULL);
    g_pSocket->start();
}
MessageManager::~MessageManager(){
    
}
MessageManager * MessageManager::shareMessageManager(){
    if (!messageManager) {
        messageManager= new MessageManager();
    }
    return messageManager;
}
void MessageManager::sendLoginMessage(){
    std::string str;
    
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["uname"]=CCUserDefault::sharedUserDefault()->getStringForKey("user").c_str();
    root["pwd"]=CCUserDefault::sharedUserDefault()->getStringForKey("password").c_str();
    str= writer.write(root);
    Message * msg= g_pSocket->constructMessage(str.c_str(), CMID_LOGIN);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receiveLoginMessage), "1001", NULL);
    g_pSocket->sendMessage_(msg, true);
}
void MessageManager::receiveLoginMessage(CCObject *obj){
    CCAssert(obj, "RegisterSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1001");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("loginMessage", obj);
}
void MessageManager::sendPersonal(int chaId){
    CSJson::FastWriter writer;
    CSJson::Value writerJson;
    writerJson["pid"]=chaId;
    std::string json_str=writer.write(writerJson);
    Message *msg= (Message *)g_pSocket->constructMessage(json_str, CMID_TEAMINFO);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receivePersonal), "1103", NULL);
    g_pSocket->sendMessage_(msg, true);
    
}
void MessageManager::receivePersonal(CCObject *obj){
    CCAssert(obj, "RegisterSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1103");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("personMessage", obj);
}
void MessageManager::sendRegisterMessage(const char *uname,const char *pwd){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["uname"]=uname;
    root["pwd"]=pwd;
    std::string str= writer.write(root);
    Message * msg= g_pSocket->constructMessage(str.c_str(), CMID_REGISTER);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receiveRegisterMessage), "1002", NULL);
    g_pSocket->sendMessage_(msg, true);
    
    
}
void MessageManager::receiveRegisterMessage(CCObject *pObj){
    CCAssert(pObj, "RegisterSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1002");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("registerMessage", pObj);
}
void MessageManager::sendPetMessage(){//获取上场宠物信息
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=GameData::shareGameData()->chaId;
    std::string json_str=writer.write(root);
    Message* msg= (Message *)g_pSocket->constructMessage(json_str, CMID_GETPETINFO);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receivePetMessage), "1104", NULL);
    g_pSocket->sendMessage_(msg, false);
    
    
}
void MessageManager::receivePetMessage(CCObject *pObj){
    CCAssert(pObj, "PetSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1104");
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("petsInfo", pObj);
}
void MessageManager::sendTeamMessage(){
    
}
void MessageManager::receiveTeamMessage(){
    
}

void MessageManager::sendBattleMesssage(){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=GameData::shareGameData()->chaId;
    std::string json_str=writer.write(root);
    Message * msg=g_pSocket->constructMessage(json_str, CMID_GETAVAILABLEFB);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receiveBattleMessage), "1201", NULL);
    g_pSocket->sendMessage_(msg, true);
    
}
void MessageManager::receiveBattleMessage(CCObject *pObj){
    CCAssert(pObj, "BattleSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1201");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("battleInfo",pObj);
}
void MessageManager::sendStrengthenMessage(){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=GameData::shareGameData()->chaId;
    std::string str= writer.write(root);
    Message * msg= g_pSocket->constructMessage(str, CMID_GETSTRENGTHENINFO);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receiveStrengthenMessage), "1401", NULL);
    
    g_pSocket->sendMessage_(msg, false);
    
}
void MessageManager::receiveStrengthenMessage(cocos2d::CCObject *pObj){
    CCAssert(pObj, "StrengthenSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1401");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("strengthenInfo", pObj);
}
void MessageManager::sendStrenghtenInfoMessage(int pid,int petid,int typeids){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=pid;
    root["petid"]=petid;
    root["typeid"]=typeids;
    std::string str=writer.write(root);
    CCLOG("%s",str.c_str());
    Message * msg= g_pSocket->constructMessage(str, CMID_STRENGTHPET);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receiveStrengthenInfoMessage), "1402", NULL);
    g_pSocket->sendMessage_(msg, false);
    
}
void MessageManager::receiveStrengthenInfoMessage(CCObject *pObj){
    CCAssert(pObj, "StrengthenInfoSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1402");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("StrengthenPet", pObj);
}
void MessageManager::sendWelfareMessage(){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=GameData::shareGameData()->chaId;
    
    std::string jsonstr=writer.write(root);
    Message * msg= g_pSocket->constructMessage(jsonstr, CMID_WELFARE);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receiveWelfareMessage), "1501", NULL);
    g_pSocket->sendMessage_(msg, false);
}
void MessageManager::receiveWelfareMessage(CCObject *pObj){
    CCAssert(pObj, "StrengthenInfoSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1501");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("welfare", pObj);
}
void  MessageManager::sendeGetWelfareMessage(int typeId){
    CSJson::FastWriter writer;
    CSJson::Value root;
    root["pid"]=GameData::shareGameData()->chaId;
    root["typeid"]=typeId;
    std::string jsonstr=writer.write(root);
    Message * msg= g_pSocket->constructMessage(jsonstr, CMID_GETWELFARE);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MessageManager::receiveWelfareMessage), "1520", NULL);
    g_pSocket->sendMessage_(msg, false);
}
void  MessageManager::receiveGetWelfareMessage(CCObject *pObj){
    CCAssert(pObj, "StrengthenInfoSender is null");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "1520");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("getwelfare", pObj);
}

