           //
//  MainView.cpp
//  TestGame
//
//  Created by lh on 13-9-11.
//  www.9miao.com
//

#include "MainView.h"
#include "Strengthen.h"
#include "BattleScene.h"
#include "BattleLayer.h"
#include "SelectFB.h"
#include "PersonTemplate.h"
#include "WelfareLayer.h"
#include "TeamScene.h"
//#include "FriendlyRole.h"
//#include "CureRole.h"
//#include "MasterRole.h"

//hu495  2:25
//950718    6

CCScene * MainView::scene(){
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainView *layer = MainView::create();
    layer->setTag(101);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool MainView::init(){
    if(!CCLayer::init()){
        return false;
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MainView::enterIntensifyLoading), "loadingremove", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MainView::receiveWelfareMsg), "welfare", NULL);
    
    size= CCDirector::sharedDirector()->getWinSize();
    color= CCLayerColor::create(ccc4(0, 0, 0, 150), size.width, size.height);
    this->addChild(color,10);
    intensifyLoading();
    SecondLayer();
    setTouchEnabled(true);
    
    return true;
}
void MainView::onEnter(){
    CCLayer::onEnter();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
   // CCTextureCache::sharedTextureCache()->removeAllTextures();
}

void MainView::SecondLayer(){
    slayer=CCLayer::create();
    
    
    CCSprite *m_pBgSprite=CCSprite::create(M_MAINVIEW_BG);
    m_pBgSprite->setPosition(ccp(size.width/2, size.height/2));
    slayer->addChild(m_pBgSprite);
    
    
    
    drag_sp=CCSprite::create(M_MVDRAG_BG);
    drag_sp->setAnchorPoint(ccp(0, 0));
    drag_sp->setPosition(ccp(0, 0));
    slayer->addChild(drag_sp,0);

    namelabel= CCLabelTTF::create("这里是名字", SELF_TTF_ONE, 60);
    namelabel->setPosition(ccp(size.width/4, size.height-size.height/6));
    namelabel->setColor(fontColor);
    this->addChild(namelabel,2);
    
    coinLabel= CCLabelTTF::create("0", SELF_TTF_ONE, 60, CCSizeMake(500, 70), kCCTextAlignmentLeft);
    coinLabel->setPosition(ccp(size.width-2*size.width/5, size.height-size.height/6));
    coinLabel->setColor(fontColor);
    this->addChild(coinLabel,2);
    
    goldLabel= CCLabelTTF::create("0", SELF_TTF_ONE, 60, CCSizeMake(500, 70), kCCTextAlignmentLeft);
    goldLabel->setColor(fontColor);
    goldLabel->setPosition(ccp(size.width-size.width/9, size.height-size.height/6));
    this->addChild(goldLabel,2);

    CCMenuItemImage * fire_Btn=CCMenuItemImage::create(M_KZBTN_UP, M_KZBTN_DOWN, this,NULL);
    CCMenuItemImage * team_Btn=CCMenuItemImage::create(M_TDBTN_UP, M_TDBTN_DOWN,this,NULL);
    CCMenuItemImage * strengthening_Btn=CCMenuItemImage::create(M_QHBTN_UP, M_QHBTN_DOWN,this,NULL);
    CCMenuItemImage * welfareBtn= CCMenuItemImage::create(M_FLBTN_UP, M_FLBTN_DOWN, this, NULL);
    welfareBtn->setAnchorPoint(ccp(0, 0));
    welfareBtn->setPosition(ccp(0, 0));
    strengthening_Btn->setPosition(ccp(0, 0));
    strengthening_Btn->setAnchorPoint(ccp(0, 0));
    team_Btn->setPosition(ccp(0, 0));
    team_Btn->setAnchorPoint(ccp(0, 0));
    fire_Btn->setAnchorPoint(ccp(0, 0));
    
    m_pWelfareMenu=CCMenu::create(welfareBtn,NULL);
    m_pWelfareMenu->setPosition(ccp(0, 70));
    m_pWelfareMenu->setAnchorPoint(ccp(0, 0));
    slayer->addChild(m_pWelfareMenu,1);
    
    intensify_Menu= CCMenu::create(strengthening_Btn,NULL);
    intensify_Menu->setPosition(ccp(0, 250));
    intensify_Menu->setAnchorPoint(ccp(0, 0));
    slayer->addChild(intensify_Menu,1);
    
    m_pTeamMenu= CCMenu::create(team_Btn,NULL);
    m_pTeamMenu->setPosition(ccp(0, 430));
    m_pTeamMenu->setAnchorPoint(ccp(0, 0));
    slayer->addChild(m_pTeamMenu,1);
    
    m_pFireMenu=CCMenu::create(fire_Btn,NULL);
    m_pFireMenu->setPosition(ccp(0, 610));
    m_pFireMenu->setAnchorPoint(ccp(0, 0));

    slayer->addChild(m_pFireMenu,1);
    slayer->setPosition(ccp(0, 0));
    this->addChild(slayer,1);
    
}
void MainView::setCoin(){
    char coins[40]="";
    sprintf(coins, "%d",GameData::shareGameData()->coin);
    char golds[40]="";
    sprintf(golds, "%d",GameData::shareGameData()->gold);
    coinLabel->setString(coins);
    goldLabel->setString(golds);
}
void  MainView::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-99900, true);
    CCLayer::registerWithTouchDispatcher();
}
bool MainView::ccTouchBegan(CCTouch* touch, CCEvent* event){
    if (intensify_Flag||fire_menu_flag||team_flag) {
        return false;
    }
    
    
    
    intensify_Flag=intensify_Menu->ccTouchBegan(touch, event);
    //team_flag=m_pTeamMenu->ccTouchBegan(touch, event);
    fire_menu_flag=m_pFireMenu->ccTouchBegan(touch, event);
    flmenuFlag=m_pWelfareMenu->ccTouchBegan(touch, event);
    fpoint=touch->getLocation();
    
    if(fire_menu_flag){
        dis= touch->getLocation().x-m_pFireMenu->boundingBox().origin.x;
        m_pFireMenu->setScale(1.1);
        
    }else if(intensify_Flag){
        dis=touch->getLocation().x-intensify_Menu->boundingBox().origin.x;
        intensify_Menu->setScale(1.1);
    }else if (team_flag){
        dis=touch->getLocation().x-m_pTeamMenu->boundingBox().origin.x;
        m_pTeamMenu->setScale(1.1);
    }else if(flmenuFlag){
        dis=touch->getLocation().x-m_pWelfareMenu->boundingBox().origin.x;
        m_pWelfareMenu->setScale(1.1);
    }
    return true;
}
void MainView::intensifyLoading(){
    LoadingLayer *Layer=LoadingLayer::create();
    intensifLayer=Layer;
    intensifLayer->setVisible(false);
    intensifLayer->setPosition(ccp(-size.width, 0));
    this->addChild(Layer,6);
}
void MainView::ccTouchMoved(CCTouch* touch, CCEvent* event){
    if(intensify_Flag){
        intensify_Menu->ccTouchMoved(touch, event);
        m_pWelfareMenu->setZOrder(0);
        m_pFireMenu->setZOrder(0);
        m_pTeamMenu->setZOrder(0);
        drag_sp->setZOrder(1);
        intensify_Menu->setPosition(ccp(touch->getLocation().x-dis, intensify_Menu->getPosition().y));
        drag_sp->setPosition(ccp(touch->getLocation().x-dis, drag_sp->getPosition().y));
        intensifLayer->setPosition(ccp(touch->getLocation().x-size.width-dis, 0));
    }
   else  if (fire_menu_flag) {
        
        m_pFireMenu->ccTouchMoved(touch, event);
       m_pWelfareMenu->setZOrder(0);
        intensify_Menu->setZOrder(0);
        m_pTeamMenu->setZOrder(0);
        drag_sp->setZOrder(1);
        drag_sp->setPosition(ccp(touch->getLocation().x-dis, drag_sp->getPosition().y));
        m_pFireMenu->setPosition(ccp(touch->getLocation().x-dis, m_pFireMenu->getPosition().y));
        intensifLayer->setPosition(ccp(touch->getLocation().x-size.width-dis, 0));
    }
    if (team_flag) {
        m_pTeamMenu->ccTouchMoved(touch, event);
        m_pWelfareMenu->setZOrder(0);
        m_pFireMenu->setZOrder(0);
        intensify_Menu->setZOrder(0);
        drag_sp->setZOrder(1);
        drag_sp->setPosition(ccp(touch->getLocation().x-dis, drag_sp->getPosition().y));
        m_pTeamMenu->setPosition(ccp(touch->getLocation().x-dis, m_pTeamMenu->getPosition().y));
        intensifLayer->setPosition(ccp(touch->getLocation().x-size.width-dis, 0));
    }
    if(flmenuFlag){
        m_pWelfareMenu->ccTouchMoved(touch, event);
        m_pFireMenu->setZOrder(0);
        intensify_Menu->setZOrder(0);
        m_pTeamMenu->setZOrder(0);
        drag_sp->setZOrder(1);
        drag_sp->setPosition(ccp(touch->getLocation().x-dis, drag_sp->getPosition().y));
        m_pWelfareMenu->setPosition(ccp(touch->getLocation().x-dis, m_pWelfareMenu->getPosition().y));
        intensifLayer->setPosition(ccp(touch->getLocation().x-size.width-dis, 0));
    }
}

void MainView::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if(intensify_Flag){
        number=2;
        intensify_Menu->ccTouchEnded(touch, event);
        intensify_Menu->setScale(1);
        m_pWelfareMenu->setZOrder(2);
        m_pFireMenu->setZOrder(2);
        m_pTeamMenu->setZOrder(2);
        drag_sp->setZOrder(0);
        LayerEaseIn(intensifLayer,intensify_Menu);
        intensify_Flag=false;
    }
    if(fire_menu_flag){
        number=1;
        m_pFireMenu->setScale(1);
        intensify_Menu->setZOrder(2);
        m_pWelfareMenu->setZOrder(2);
        m_pTeamMenu->setZOrder(2);
        drag_sp->setZOrder(0);
        m_pFireMenu->ccTouchEnded(touch, event);
        LayerEaseIn(intensifLayer,m_pFireMenu);
        fire_menu_flag=false;
        
    }
    if (team_flag) {
        number=3;
        m_pTeamMenu->setScale(1);
        m_pWelfareMenu->setZOrder(2);
        intensify_Menu->setZOrder(2);
        drag_sp->setZOrder(0);
        m_pFireMenu->setZOrder(0);
        m_pTeamMenu->ccTouchEnded(touch, event);
        LayerEaseIn(intensifLayer, m_pTeamMenu);
        team_flag=false;
    }
    if (flmenuFlag) {
        number=4;
        m_pWelfareMenu->ccTouchEnded(touch, event);
        m_pWelfareMenu->setScale(1);
        intensify_Menu->setZOrder(2);
        m_pTeamMenu->setZOrder(2);
        drag_sp->setZOrder(0);
        m_pFireMenu->setZOrder(2);
        LayerEaseIn(intensifLayer, m_pWelfareMenu);
        flmenuFlag=false;
    }
}
void MainView::LayerEaseIn(CCNode * Layersender,CCMenu * menu){
    if (Layersender->getPosition().x>-(size.width/2)) {
        CCMoveTo * move=CCMoveTo::create(0.3, ccp(0, 0));
        CCEaseIn * easein=CCEaseIn::create(move, 1);
        CCCallFunc * func=CCCallFunc::create(this, callfunc_selector(MainView::loadFBTexture));
        CCSequence * actions=CCSequence::create(easein,NULL);
        Layersender->runAction(actions);
        MoveAndEase(ccp(size.width, drag_sp->getPosition().y), drag_sp);
        CCMoveTo * menu_Move=CCMoveTo::create(0.3, ccp(size.width, menu->getPosition().y));
        CCEaseIn * menu_EaseIn=CCEaseIn::create(menu_Move, 1);
        ((CCLayer *)Layersender)->setTouchEnabled(true);
        CCSequence * menu_Seq=CCSequence::create(menu_EaseIn,func,NULL);
        menu->runAction(menu_Seq);
    }
    else if(Layersender->getPosition().x<-(size.width/2)){
        CCMoveTo * move=CCMoveTo::create(0.3, ccp(-size.width, 0));
        CCEaseIn * easein=CCEaseIn::create(move, 1);
        Layersender->runAction(easein);
        MoveAndEase(ccp(0, drag_sp->getPosition().y), drag_sp);
        MoveAndEase(ccp(0, m_pFireMenu->getPosition().y), m_pFireMenu);
        MoveAndEase(ccp(0, m_pTeamMenu->getPosition().y), m_pTeamMenu);
        MoveAndEase(ccp(0, intensify_Menu->getPosition().y), intensify_Menu);
        MoveAndEase(ccp(0, m_pWelfareMenu->getPosition().y), m_pWelfareMenu);
    }
}
void MainView::loadFBTexture(){
    if (number==1) {
        fbtexture.push_back("FBLayer/guanqia_bg.png");
        fbtexture.push_back("FBLayer/huodong_down.png");
        fbtexture.push_back("FBLayer/huodong_up.png");
        fbtexture.push_back("FBLayer/icons.png");
        fbtexture.push_back("FBLayer/jingji_dwon.png");
        fbtexture.push_back("FBLayer/jingji_up.png");
        fbtexture.push_back("FBLayer/tiaozhan_down.png");
        fbtexture.push_back("FBLayer/tiaozhan_up.png");
        fbtexture.push_back("FBLayer/wujin_down.png");
        fbtexture.push_back("FBLayer/wujin_up.png");
        fbtexture.push_back("FBLayer/xiaodui_down.png");
        fbtexture.push_back("FBLayer/xiaodui_up.png");
        fbtexture.push_back("FBLayer/zhuxian_down.png");
        fbtexture.push_back("FBLayer/zhuxian_up.png");
    }else if(number==2){
        fbtexture.push_back("qianghua_1/attack_down.png");
        fbtexture.push_back("qianghua_1/attack_up.png");
        fbtexture.push_back("qianghua_1/dujie_down.png");
        fbtexture.push_back("qianghua_1/dujie_up.png");
        fbtexture.push_back("qianghua_1/qh_bg.png");
        fbtexture.push_back("qianghua_1/shengming_down.png");
        fbtexture.push_back("qianghua_1/shengming_up.png");

    }else if(number==3){
        fbtexture.push_back("team_layer_1/persons01.png");
        fbtexture.push_back("team_layer_1/team_bg.png");
        fbtexture.push_back("team_layer_1/team_locked.png");
        fbtexture.push_back("team_layer_1/team_zhegai.png");
        fbtexture.push_back("team_layer_1/team_unlocked.png");
    }
    else if (number==4){
        fbtexture.push_back("welfareBg.png");
        fbtexture.push_back("welfarecell_Bg.png");
        fbtexture.push_back("receive_down.png");
        fbtexture.push_back("receive_up.png");
        fbtexture.push_back("receive_dis.png");
    }
    
    intensifLayer->addImage(fbtexture,this);
    
}
void MainView::enterIntensifyLoading(CCObject *obj){
    if (obj==this) {
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "loadingremove");
    }else{
        return;
    }
    
    this->setTouchEnabled(false);
    if(number==1){
        CCScene *scene= CCDirector::sharedDirector()->getRunningScene();
        SelectFB *fb= SelectFB::create();
        //fb->sendJsonData();
        scene->addChild(fb,1);
        this->removeFromParentAndCleanup(true);
    }else if(number==3){
        
        CCScene *teamScene=TeamScene::scene();
        CCDirector::sharedDirector()->replaceScene(teamScene);
    }else if(number==2){
        CCScene *strenScene=Strengthen::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1,strenScene));
    }else if(number==4){
        CCScene *welfareScene=WelfareLayer::scene();
        
        CCDirector::sharedDirector()->replaceScene(welfareScene);
        MessageManager::shareMessageManager()->sendWelfareMessage();
    }
        
}
void MainView::receiveWelfareMsg(CCObject *pObj){
    Message *msg=(Message *)pObj;
    GameData::shareGameData()->welfare_Str=msg->data;
    
    
    this->scheduleOnce(schedule_selector(MainView::getWelfareMsg), 0.001);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "welfare");
    
}
void MainView::getWelfareMsg(){
    WelfareLayer * layer=(WelfareLayer *)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(100000);
    layer->setData();
}
void MainView::MoveAndEase(cocos2d::CCPoint pos,CCNode * sender){
    CCMoveTo *menu_Move=CCMoveTo::create(0.3, pos);
    CCEaseIn *menu_EaseIn=CCEaseIn::create(menu_Move, 1);
    
    CCSequence *menu_Seq=CCSequence::create(menu_EaseIn,NULL);
    sender->runAction(menu_Seq);
}
void MainView::setLayerVisible(){
    drag_sp->setPosition(ccp(0,0));
    m_pFireMenu->setPosition(ccp(0, 530));
    this->setTouchEnabled(false);
    this->setVisible(false);
}

void MainView::setRole(){
    GameData::shareGameData()->roleArr->removeAllObjects();
    std::string str= GameData::shareGameData()->str;
    CSJson::Reader reader;
    CSJson::Value root;
    if (reader.parse(str.c_str(), root)) {
        CSJson::Value pfinfo;
        CSJson::Value petinfo;
        pfinfo=root["pfinfo"];
    
        for (int i=0; i<pfinfo.size(); i++) {
            Person *mainsp=NULL;
            petinfo=pfinfo[i]["petinfo"];
            int btype=pfinfo[i]["btype"].asInt();
            if (btype==2) {
                mainsp= MasterRole::create();
            }else if(btype==1){
                mainsp= FriendlyRole::create();
            }
            else if(btype==3){
                mainsp= CureRole::create();
            }
            mainsp->m_btype=btype;
            mainsp->m_defense=petinfo["defe_base"].asInt();
            mainsp->m_attack=petinfo["att_base"].asInt();
            mainsp->m_hp=petinfo["hp_base"].asInt();
            mainsp->cur_hp=mainsp->m_hp;
            mainsp->m_petid=petinfo["petid"].asInt();
            mainsp->m_level=petinfo["level"].asInt();
            mainsp->m_id=petinfo["ppid"].asInt();
            mainsp->m_hittarget=petinfo["dex_base"].asInt();//命中
            mainsp->m_tenacity=petinfo["tou_base"].asInt();
            mainsp->m_dodge=petinfo["agl_base"].asInt();
            mainsp->m_parry=petinfo["par_base"].asInt();
            mainsp->skillId=petinfo["skill"][0].asInt();
            mainsp->skill=SkillsInfo::create(petinfo["skill"][0].asInt());
            mainsp->m_strike=petinfo["cri_base"].asInt();
            mainsp->m_strike_result=petinfo["crp_base"].asInt();
            mainsp->m_name=petinfo["name"].asCString();
            mainsp->m_range=petinfo["rng"].asInt();
            mainsp->m_speed=petinfo["mov"].asInt();
            mainsp->m_attack_speed=petinfo["spd"].asInt();
            mainsp->res_icon=petinfo["headid"].asInt();
            mainsp->res_resource=petinfo["resourceid"].asInt();
            GameData::shareGameData()->roleArr->addObject(mainsp);
        }
    }
}
void MainView::onExit(){
    CCLayer::onExit();
}
MainView::~MainView(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_MAINVIEW_BG);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_MVDRAG_BG);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_KZBTN_UP);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_KZBTN_DOWN);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_TDBTN_UP);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_TDBTN_DOWN);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_QHBTN_UP);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_QHBTN_DOWN);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_FLBTN_UP);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_FLBTN_DOWN);
}