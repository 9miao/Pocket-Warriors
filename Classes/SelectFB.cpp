//
//  SelectFB.cpp
//  TestGame
//
//  Created by lh on 13-11-14.
//  www.9miao.com
//

#include "SelectFB.h"
#include "BattleScene.h"
#include "message.h"
#include "PersonTemplate.h"
#include "MessageManager.h"

bool SelectFB::init(){
    if (!CCLayer::init()) {
        return false;
    }
    socket=GameData::shareGameData()->socket;
    size= CCDirector::sharedDirector()->getWinSize();
    addLoadingLayer();
    addUILayer();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SelectFB::enterBattleScene), "loadingremove", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SelectFB::receiveJsonData), "battleInfo", NULL);
    
    return true;
}
void SelectFB::addUILayer(){
    layer= UILayer::create();
    layer->setTouchEnabled(true);
    this->addChild(layer,0);
    UIWidget *widget=CCUIHELPER->createWidgetFromJsonFile(M_SELECTFB_BGJSON);
    widget->setTag(1111);
    layer->addWidget(widget);
    mainBtn= dynamic_cast<UIButton *>(layer->getWidgetByTag(201));
    mainBtn->setTouchEnabled(true);
    mainBtn->addPushDownEvent(this,  coco_pushselector(SelectFB::UIBtnTouchBeganEvent));
    mainBtn->addMoveEvent(this, coco_pushselector(SelectFB::UIBtnTouchMovedEvent));
    mainBtn->addReleaseEvent(this, coco_pushselector(SelectFB::UIBtnTouchEndedEvent));
    mainBtn->addCancelEvent(this, coco_pushselector(SelectFB::UIBtnTouchEndedEvent));
    sendJsonData();
}
void SelectFB::UIBtnTouchBeganEvent(cocos2d::CCObject *sender){
    UIButton* wig= (UIButton * )sender;
    diss= wig->getTouchStartPos().x-wig->getRect().origin.x;
}
void SelectFB::UIBtnTouchMovedEvent(cocos2d::CCObject *sender){
    UIWidget * wig= (UIWidget * )sender;
    CCPoint pos= wig->getTouchMovePos();
    wig->setPosition(ccp(pos.x-diss, wig->getPosition().y));
    loading->setPosition(ccp(wig->getPosition().x-loading->getContentSize().width, 0));

}
void SelectFB::UIBtnTouchEndedEvent(cocos2d::CCObject *sender){
    UIButton * btn= (UIButton *)sender;
    UIBtnFadeIn(btn, loading);
}


void SelectFB::addLoadingLayer(){
    loading= LoadingLayer::create();
    loading->setPosition(ccp(-size.width, 0));
    this->addChild(loading,2);
    
}
void SelectFB::addBattleTexture(){
    battleRes.push_back(M_BATTLE_BG);
    battleRes.push_back(LINE);
    battleRes.push_back(DIRECTION);
    battleRes.push_back(CIRCLES);
    battleRes.push_back("dihuo.png");
    battleRes.push_back("huoqiu1.png");
    battleRes.push_back("fuzinv0.png");
    battleRes.push_back("fuzinv1.png");
    
    battleRes.push_back("fuzinvrun0.png");
    battleRes.push_back("fuzinvrun1.png");
    
    battleRes.push_back("boss/daozhangrun0.png");
    battleRes.push_back("boss/daozhangrun1.png");
    
    battleRes.push_back("boss/daozhang1.png");
    battleRes.push_back("boss/daozhang0.png");
    
    battleRes.push_back("man0/shanzinan0.png");
    battleRes.push_back("man0/shanzinan1.png");
    
    battleRes.push_back("man0/shanzinanrun0.png");
    battleRes.push_back("man0/shanzinanrun1.png");
    
    loading->addImage(battleRes, this);
}
void SelectFB::UIBtnFadeIn(cocos2d::extension::UIButton *btn, cocos2d::CCLayer *layer){
    if (btn->getPosition().x>size.width/2) {
        CCMoveTo *move=CCMoveTo::create(0.3, ccp(size.width, btn->getPosition().y));
        CCEaseOut *easeout=CCEaseOut::create(move, 1);
        CCMoveTo *move1=CCMoveTo::create(0.3, ccp(0, 0));
        CCEaseOut *easeout1=CCEaseOut::create(move1, 1);
        CCCallFunc *func = CCCallFunc::create(this,callfunc_selector(SelectFB::addBattleTexture));
        CCSequence *seq= CCSequence::create(easeout1,func,NULL);
        btn->runAction(easeout);
        layer->runAction(seq);
    }else if(btn->getPosition().x<size.width/2){
        CCMoveTo *move=CCMoveTo::create(0.3, ccp(0, btn->getPosition().y));
        CCEaseOut *easeout=CCEaseOut::create(move, 1);
        CCMoveTo *move2=CCMoveTo::create(0.3, ccp(-size.width, 0));
        CCEaseOut *easeout2=CCEaseOut::create(move2, 1);
        btn->runAction(easeout);
        layer->runAction(easeout2);
    }
    
    
}

void SelectFB::enterBattleScene(CCObject *obj){
    if (obj==this) {
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "loadingremove");
    }else{
        return;
    }
    CCScene * scene= BattleScene::scene();
    CCDirector::sharedDirector()->replaceScene(scene);
}


void SelectFB::UIBtnFadeOut(cocos2d::extension::UIButton *btn, cocos2d::CCLayer *layer){
    
}
void SelectFB::setData(){
    
    
    
    

    GameData::shareGameData()->enemyArr->removeAllObjects();
    CSJson::Reader read;
    CSJson::Value roo;
    
    int instanceid;
    bool result;
    if (read.parse(jsonData, roo)) {
        result=roo["result"].asBool();
        if (result) {
            instanceid=roo["instanceid"].asInt();
        }
    }
    CSJson::Reader reader;
    CSJson::Value root;
    char insid[10]="";
    sprintf(insid, "%d",instanceid);
    CCString *str= CCString::createWithContentsOfFile(M_SELECTFB_MONSTER);
    CCLog("%s",str->getCString());
    if (reader.parse(str->getCString(), root)) {
        CSJson::Value data;
        data=root["data"];
        CSJson::Value row;
//        row=data["row"];
        CSJson::Value arr;
        std::string  name;
        CSJson::Value section;
        section=data[insid];
        name= section["iname"].asString();
        CCLog("%s",name.c_str());
        arr=section["mlist"];
        
        str_callback=name.c_str();
        this->scheduleOnce(schedule_selector(SelectFB::aaaaa), 0.001);

        for (int i=0; i<arr.size(); i++) {
            CSJson::Value mlist;
            mlist= arr[i];
            GameData::shareGameData()->onScreenCount=mlist[0].asInt();
            CSJson::Value eneArray=mlist[2];
            for (int i=0; i<eneArray.size(); i++) {//
                initEnemys(eneArray[i].asInt(),i);
            }
        }
    }
}


void SelectFB::initEnemys(int eneId,int i){
    
    CSJson::Reader reader;
    CSJson::Value root;
    char enId[10]="";
    sprintf(enId, "%d",eneId);
    str= CCString::createWithContentsOfFile(M_SELECTFB_MONSTERINFO);
    if (reader.parse(str->getCString(), root)) {
        CSJson::Value data;
        data=root["data"];
        CSJson::Value row;
        row=data[enId];
        EnemyRole *ene=EnemyRole::create();
        ene->m_id=10000+i;
        ene->m_name=row["pname"].asString();
        ene->m_attack=row["att"].asInt();
        ene->m_hp=row["hp"].asInt();
        ene->type=row["btype"].asInt();
        ene->cur_hp=ene->m_hp;
        ene->m_defense=row["def"].asInt();
        ene->m_tenacity=row["tou"].asInt();
        ene->m_hittarget=row["dex"].asInt();
        ene->m_dodge=row["agl"].asInt();
        ene->m_parry=row["par"].asInt();
        ene->m_strike=row["cri"].asInt();
        ene->m_strike_result=row["crp"].asInt();
        ene->m_range=row["rng"].asInt();
        ene->m_speed=row["mov"].asInt();
        ene->m_attack_speed=row["spd"].asInt();
        GameData::shareGameData()->enemyArr->addObject(ene);
    }
}



void SelectFB::sendJsonData(){
    
   MessageManager::shareMessageManager()->sendBattleMesssage();
    
}
void SelectFB::receiveJsonData(CCObject *obj){//
    CCAssert(obj, "obj is null");
    
    
   // this->scheduleOnce(schedule_selector(SelectFB::aaaaa), 2);
    
   // aaaaa();1201
    GameData::shareGameData()->msg_dic->removeObjectForKey(1201);
    Message *msg= (Message *)obj;
    jsonData=msg->data;
    this->scheduleOnce(schedule_selector(SelectFB::setData), 0.01);
}
void SelectFB::aaaaa(){
    UITextArea *names= dynamic_cast<UITextArea *>(layer->getWidgetByTag(301));
    names->setText(str_callback.c_str());
}
SelectFB::~SelectFB(){
    //layer->removeWidgetAndCleanUp(layer->getWidgetByTag(1111), true);
//    this->removeAllChildrenWithCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "battleInfo");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/guanqia_bg.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/huodong_down.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/huodong_up.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/icons.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/jingji_dwon.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/jingji_up.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/tiaozhan_down.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/tiaozhan_up.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/wujin_down.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/wujin_up.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/xiaodui_down.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/xiaodui_up.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/zhuxian_down.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FBLayer/zhuxian_up.png");
   // ccDrawFree();
}

