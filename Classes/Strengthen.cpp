//
//  Strengthen.cpp
//  TestGame
//
//  Created by lh on 13-9-18.
//  www.9miao.com
//

#include "Strengthen.h"
#include "message.h"
#include "MainView.h"
#include "MessageManager.h"
Strengthen::~Strengthen(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}
bool Strengthen::init(){
    if(!CCLayer::init()){
        return false;
    }
    //strengthenInfo
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Strengthen::receiveNotice), "changePosition", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Strengthen::finishedInit), "strengthenInfo", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Strengthen::reciveStrengthen), "StrengthenPet", NULL);
    setTouchEnabled(true);
    strengtenInfo= CCArray::create();
    strengtenInfo->retain();
    m_nCurPage=1;
    oldnumflag=1;
    m_nPageCount=6;
    numflag=1;
    scrollFlag=false;
    size= CCDirector::sharedDirector()->getWinSize();
    socket=GameData::shareGameData()->socket;
    layer= UILayer::create();
    this->addChild(layer,0);
    layer->addWidget(CCUIHELPER->createWidgetFromJsonFile("qianghua_1/qianghua_1.json"));
    CCSprite *zhegai = CCSprite::create("zhegai.png");
    zhegai->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(zhegai,1);
    UIButton *hpBtn= dynamic_cast<UIButton * >(layer->getWidgetByTag(201));
    UIButton *attackBtn= dynamic_cast<UIButton *>(layer->getWidgetByTag(202));
    UIButton *dujieBtn= dynamic_cast<UIButton *>(layer->getWidgetByTag(203));
    attackBtn->addReleaseEvent(this, coco_releaseselector(Strengthen::sendStrengthen));
    hpBtn->addReleaseEvent(this, coco_releaseselector(Strengthen::sendStrengthen));
    dujieBtn->addReleaseEvent(this, coco_releaseselector(Strengthen::sendStrengthen));
    CCMenuItemImage * goback_btn= CCMenuItemImage::create("drag_goback_up.png", "drag_goback_down.png", this, NULL);
    goback_btn->setAnchorPoint(ccp(0, 1));
    goback_menu= CCMenu::create(goback_btn,NULL);
    goback_menu->setPosition(ccp(1800, 1080));
    this->addChild(goback_menu,4);
    initLoadingLayer();
    getStrengt();
    return true;
}
void Strengthen::receiveNotice(cocos2d::CCObject *obj){
    CCString *number=(CCString *)obj;
    numflag=number->intValue();
    this->scheduleOnce(schedule_selector(Strengthen::initBaseProperty), 0);
}
void Strengthen::initBaseProperty(){
    StengthenList *sList=(StengthenList *)strengtenInfo->objectAtIndex(numflag-1);
    UILabel *nameLabel= dynamic_cast<UILabel * >(layer->getWidgetByTag(100));
    UILabel *luckLabel= dynamic_cast<UILabel *>(layer->getWidgetByTag(101));
    UILabel *hpLbael= dynamic_cast<UILabel *>(layer->getWidgetByTag(102));
    UILabel *hpXhLabel=dynamic_cast<UILabel *>(layer->getWidgetByTag(103));
    UILabel *attackLabel= dynamic_cast<UILabel *>(layer->getWidgetByTag(104));
    UILabel *attackXhLabel=dynamic_cast<UILabel *>(layer->getWidgetByTag(105));
    UILabel *djXhLabel=dynamic_cast<UILabel *>(layer->getWidgetByTag(106));
    char luck[20]="";
    char hp[20]= "";
    char hpxl[20]="";
    char attack[20]="";
    char attackxh[20]="";
    char djxh[20]="";
    sprintf(hp, "%d",sList->hp);
    sprintf(hpxl, "%d",sList->hpmoney);
    sprintf(attackxh, "%d",sList->attmoney);
    sprintf(attack, "%d",sList->att);
    sprintf(djxh, "%d",100);
    sprintf(luck, "%d",sList->luck);
    hpLbael->setText(hp);
    hpXhLabel->setText(hpxl);
    attackLabel->setText(attack);
    attackXhLabel->setText(attackxh);
    djXhLabel->setText(djxh);
    luckLabel->setText(luck);
    nameLabel->setText(sList->name.c_str());
}

void Strengthen::analyzeData(std::string data){
    CSJson::Reader reader;
    CSJson::Value root;
    if (reader.parse(data.c_str(), root)) {
        std::string message= root["message"].asString();
        CSJson::Value datas;
        datas= root["data"];
        int silvermoney=datas[0]["silvermoney"].asInt();
        int fighting=datas[0]["fighting"].asInt();
        char money[50]="";
        char fight[50]="";
        sprintf(fight, "%d",fighting);
        sprintf(money, "%d",silvermoney);
        UILabel * label=dynamic_cast<UILabel *>(layer->getWidgetByTag(108));
        label->setText(money);
        UILabel * fightLabel= dynamic_cast<UILabel *>(layer->getWidgetByTag(107));
        teamInfo=datas[0]["teaminfo"];
        for (int i=0; i<teamInfo.size(); i++) {
            
            StengthenList* list= new StengthenList();
            list->ppid=teamInfo[i]["ppid"].asInt();
            list->hp= teamInfo[i]["hp"].asInt();
            list->level=teamInfo[i]["level"].asInt();
            list->levellimit=teamInfo[i]["levellimit"].asInt();
            list->resource= teamInfo[i]["resource"].asInt();
            list->headid= teamInfo[i]["headid"].asInt();
            list->color=teamInfo[i]["color"].asInt();
            list->name=teamInfo[i]["name"].asString();
            list->hpmoney= teamInfo[i]["hpmoney"].asInt();
            list->att=teamInfo[i]["att"].asInt();
            list->attmoney=teamInfo[i]["attmoney"].asInt();
            list->status=teamInfo[i]["status"].asInt();
            strengtenInfo->addObject(list);
        }
        initBaseProperty();
        hasRole= true;
        fightLabel->setText(fight);
    }
}
void Strengthen::sendStrengthen(CCObject *obj){
    UIWidget *wig= (UIWidget *)obj;
    CCLOG("%d",numflag);
    StengthenList *stl=(StengthenList *)strengtenInfo->objectAtIndex(numflag-1);
    
    MessageManager::shareMessageManager()->sendStrenghtenInfoMessage(GameData::shareGameData()->chaId, stl->ppid, wig->getTag()-200);

}
void Strengthen::reciveStrengthen(CCObject *obj){
    Message * msg= (Message *)obj;
    //GameData::shareGameData()->msg_dic->objectForKey(CMID_STRENGTHPET);
    if (msg) {
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "StrengthenPet");
        //addObserver(this, callfuncO_selector(Strengthen::reciveStrengthen), "StrengthenPet", NULL);
       // this->unschedule(schedule_selector(Strengthen::reciveStrengthen));
        CSJson::Reader reader;
        CSJson::Value root;
        CSJson::Value datas;
        if (reader.parse(msg->data, root)) {
            if (root["result"].asBool()) {
                datas=root["data"];
                bool intensifyResult= datas[0]["intensifyResult"].asBool();
                if (intensifyResult) {
                int hpmoney= datas[0]["hpmoney"].asInt();
                int addhp=datas[0]["addhp"].asInt();
                int newcolor= datas[0]["newcolor"].asInt();
                int attmoney= datas[0]["attmoney"].asInt();
                int addatt= datas[0]["addatt"].asInt();
                int newlevellimit= datas[0]["newlevellimit"].asInt();
                int djmoney=datas[0]["djmoney"].asInt();
                int newfighting= datas[0]["newfighting"].asInt();
                int luck= datas[0]["luck"].asInt();
                StengthenList * sList= (StengthenList *)strengtenInfo->objectAtIndex(numflag-1);
                sList->setNewProperties(djmoney, hpmoney, attmoney, addhp, addatt, luck, newfighting, newlevellimit, newcolor);
                    this->scheduleOnce(schedule_selector(Strengthen::initBaseProperty), 0.01);
                }
            }
        }
    }
}
void Strengthen::getStrengt(){
    MessageManager::shareMessageManager()->sendStrengthenMessage();

}
void Strengthen::finishedInit(CCObject *obj){
    Message * msg=(Message *)obj;
    if (msg) {
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "strengthenInfo");
        json_strs= msg->data;
        this->scheduleOnce(schedule_selector(Strengthen::setStentgt), 0.001);
    }
    
}
void Strengthen::setStentgt(CCObject *obj){
    analyzeData(json_strs);
    scrollview= CCScrollViewX::create(CCSizeMake((388*teamInfo.size()+4), 488),CCSizeMake(size.width, 488));
    for (int i=0; i<teamInfo.size(); i++) {
        CCSprite * person=CCSprite::create("person.png");
        person->setPosition(ccp(388*2+388*(i), 0));
        scrollview->addChild(person, 1, i+100);
    }
    scrollview->setDirection(kCCScrollViewDirectionHorizontal);
    scrollview->setPosition(ccp(30, 250));
    scrollview->unscheduleAllSelectors();
    scrollview->setBounceable(true);
    this->addChild(scrollview);
}
void Strengthen::initLoadingLayer(){
    loadlayer= LoadingLayer::create();
    loadlayer->setPosition(ccp(0, size.height));
    this->addChild(loadlayer,20);
}
void Strengthen::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-99900, true);
    CCLayer::registerWithTouchDispatcher();
}
bool Strengthen::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    scrollFlag= scrollview->ccTouchBegan(touch, event);
    m_touchOffset=scrollview->getContentOffset();
    m_touchPoint= CCDirector::sharedDirector()->convertToGL(touch->getLocation());
    gobackflag= goback_menu->ccTouchBegan(touch, event);
    m_beginPoint= touch->getLocation();
    goback_dis= touch->getLocation().y-goback_menu->boundingBox().origin.y;
    if (touch->getLocation().y<300) {
        return false;
    }
    
    return true;
}
void Strengthen::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    
    if (scrollFlag) {
        scrollview->ccTouchMoved(touch, event);
    }
    if (gobackflag) {
        goback_menu->ccTouchMoved(touch, event);
        loadlayer->setPosition(ccp(0, touch->getLocation().y-goback_dis));
        goback_menu->setPosition(ccp(goback_menu->getPosition().x, touch->getLocation().y-goback_dis));
    }
}
void Strengthen::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    if (scrollFlag) {

        scrollview->ccTouchEnded(touch, event);
        scrollFlag=false;
    }
    if (gobackflag) {
        
        goback_menu->ccTouchEnded(touch, event);
        EaseInAndOut();
        gobackflag=false;
    }
}
void Strengthen::EaseInAndOut(){
    CCMoveTo * move1;
    CCMoveTo * move2;
    CCCallFunc * func;

    if (goback_menu->getPosition().y<800) {
        move1= CCMoveTo::create(0.5, ccp(goback_menu->getPosition().x, 0));
        move2= CCMoveTo::create(0.5, ccp(0, 0));
        func= CCCallFunc::create(this, callfunc_selector(Strengthen::changeScene));
        
    }else{
        move1= CCMoveTo::create(0.5, ccp(goback_menu->getPosition().x, size.height));
        move2= CCMoveTo::create(0.5, ccp(0, size.height));
        func = CCCallFunc::create(this, NULL);
    }
    CCEaseInOut * ease1= CCEaseInOut::create(move1, 1);
    CCEaseInOut * ease2= CCEaseInOut::create(move2, 1);
    
    CCSequence * seq= CCSequence::create(ease1,func,NULL);
    goback_menu->runAction(seq);
    loadlayer->runAction(ease2);
}

void Strengthen::changeScene(){
    CCScene * scene = MainView::scene();
    MainView * m= (MainView *)scene->getChildByTag(101);
    m->setCoin();
    m->setRole();
    m->intensifLayer->setVisible(true);
    m->color->setOpacity(GLubyte(0));
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,scene));
}
CCScene * Strengthen::scene(){
    CCScene * scene=CCScene::create();
    CCLayer * layer= Strengthen::create();
    scene->addChild(layer);
    return scene;
}

StengthenList::StengthenList(){
    hpmoney=0;
    att=0;
    attmoney=0;
    hp=0;
    levellimit=0;
    resource=0;
    headid=0;
    color=0;
    level=0;
    name="";
    status=0;
    newcolor=0;
    newlevellimit=0;
    djcostMoney=0;
    hpcostMoney=0;
    attcostMoney=0;
    addHp=0;
    addAtt=0;
    luck=0;
    newfighting=0;
    ppid=0;
}

StengthenList::~StengthenList(){
    
}
void StengthenList::setNewProperties(int djmoney, int hpmoney1, int attmoney1, int addhp, int addatt, int luck, int newfighting, int newlevellimit, int newcolor){
    hp= hp+addhp;
    att= att+addatt;
    if (hpmoney1==1) {
        
    }else if(hpmoney1==0){
        
    }else{
        hpmoney=hpmoney1;
    }
    
    if (attmoney1==1) {
        
    }else if(attmoney1==0){
        
    }else{
        attmoney=attmoney1;
    }
    
}