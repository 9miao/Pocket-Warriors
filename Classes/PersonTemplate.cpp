//
//  PersonTemplate.cpp
//  TestGame
//
//  Created by lh on 13-11-26.
//  www.9miao.com
//

#include "PersonTemplate.h"
#include "AnimatePacker.h"
#include "LoadRes.h"
#include "BattleLayer.h"
#include "CCTraceTo.h"
#include "message.h"
bool Person::init(){
    if (!CCSprite::init()) {
        return false;
    }
    target=NULL;
    skill=NULL;
    setStaticProperties(3000, 1000, 2, 2, 2, 2, 2, 0.4, 0.4, 0.4);
    size= CCDirector::sharedDirector()->getWinSize();
    isDead=false;
    aniDir=1;
    m_skillTouch=false;
    initBones();
    addBlood();
    AnimatePacker::getInstance()->loadAnimations(T_DG_TX_XML);
    return true;
}
void Person::initBones(){
   // m_role = CCArmature::create();
    m_role=CCSprite::create();
    m_role->setPosition(ccp(0, 0));
    m_role->setScale(1.5);
    addChild(m_role);
    CCSprite *shadow=CCSprite::create("shadow.png");
    shadow->setScale(1.5);
    shadow->setPosition(ccp(0, 0));
    addChild(shadow,-1);
    
}
void Person::setStaticProperties(int basehp,int baseattack,float defense_coff,float dodge_coff,
                         float tenacity_coff,float parry_coff,float deflect_coff,
                         float strike_coff,float strike_result_coff,float hitrate_coff){
    base_hp=basehp;
    base_attack=baseattack;
    defense_cf=defense_coff;
    dodge_cf=dodge_coff;
    tenacity_cf=tenacity_coff;
    parry_cf=parry_coff;
    deflect_cf=deflect_coff;
    strike_cf=strike_coff;
    strike_result_cf=strike_result_coff;
    hitrate_cf=hitrate_coff;

    
}
void Person::setDynamicProperties(int mAttack,int mHp,int mDefense,int mHitRate,int mDodge,
                          int mParry,int mStrike,int mStrikeResult){
    m_attack=mAttack;
    m_hp= mHp;
    m_defense=mDefense;
    m_hittarget=mHitRate;
    m_dodge=mDodge;
    m_parry=mParry;
    m_strike=mStrike;
    m_strike_result=mStrikeResult;
}
float Person::getAttackResult(){
    return m_attack* DamageRateMultiple() * DamageResultMultiple() * HitRateMultiple()*target->DamageRate();
}
void Person::loadPersonTexture(){

}
float Person::DamageRate(){
    damage_rate= (float)1/(float)(1+2.5*m_defense/m_hp);
    return damage_rate;
}
float Person::avoidRate(){
    avoid_rate= (float)1.0f-(float)1.0f/(float)(1.0f+(target->m_dodge-m_hittarget)*target->dodge_cf/target->base_hp);
    return avoid_rate;
}
float Person::parryRate(){
    parry_rate= (float)2*target->m_parry*target->parry_cf/(float)target->base_hp;
    return parry_rate;
}
float Person::strikeRate(){
    strike_rate= 2*((float)(m_strike-target->m_tenacity)*strike_cf/base_attack);
    return strike_rate;
}


float Person::DamageRateMultiple(){
    return 0.4*m_strike/base_attack+1;
}
float Person::DamageResultMultiple(){
    return 0.4*m_strike_result/base_attack+1;
}
float Person::HitRateMultiple(){
    return 0.4*m_hittarget/base_attack+1;
}

Attackmodel Person::attackModel(){

    if ( CCRANDOM_0_1()>1.0f+avoidRate()) {
        CCSprite *sp=CCSprite::createWithSpriteFrameName("shanbi_01.png");
        CCCallFuncN *funn=CCCallFuncN::create(this, callfuncN_selector(Person::animateCallBack));
        
        sp->runAction(CCSequence::create(AnimatePacker::getInstance()->getAnimate("shanbi"),funn,NULL));
        sp->setPosition(ccp(0, 70));
        this->target->addChild(sp,1);
        return AVOID;
    }else if(CCRANDOM_0_1()<strikeRate()){
        if (CCRANDOM_0_1()>1.0f-parryRate()) {
            
            CCSprite *sp1=CCSprite::createWithSpriteFrameName("baoji_01.png");
            CCCallFuncN *funn1=CCCallFuncN::create(this, callfuncN_selector(Person::animateCallBack));
            
            sp1->runAction(CCSequence::create(AnimatePacker::getInstance()->getAnimate("baoji"),funn1,NULL));
            sp1->setPosition(ccp(0, 70));
            this->addChild(sp1,1);
            
            CCSprite *sp=CCSprite::createWithSpriteFrameName("gedang_01.png");
            CCCallFuncN *funn=CCCallFuncN::create(this, callfuncN_selector(Person::animateCallBack));
            
            sp->runAction(CCSequence::create(AnimatePacker::getInstance()->getAnimate("gedang"),funn,NULL));
            sp->setPosition(ccp(0, 70));
            this->target->addChild(sp,1);
            return STRIKEANDPARRY;
        }
        CCSprite *sp1=CCSprite::createWithSpriteFrameName("baoji_01.png");
        CCCallFuncN *funn1=CCCallFuncN::create(this, callfuncN_selector(Person::animateCallBack));
        
        sp1->runAction(CCSequence::create(AnimatePacker::getInstance()->getAnimate("baoji"),funn1,NULL));
        sp1->setPosition(ccp(0, 70));
        this->addChild(sp1,1);
        return STRIKE;
    }else if(CCRANDOM_0_1()<parryRate()){
        CCSprite *sp=CCSprite::createWithSpriteFrameName("gedang_01.png");
        CCCallFuncN *funn=CCCallFuncN::create(this, callfuncN_selector(Person::animateCallBack));
        
        sp->runAction(CCSequence::create(AnimatePacker::getInstance()->getAnimate("gedang"),funn,NULL));
        sp->setPosition(ccp(0, 70));
        this->target->addChild(sp,1);
        return PARRY;
    }
    else {
        return NORMAL;
    }
}
float Person::bloodResult(Attackmodel model){
    float result=0;
    switch (model) {
        case NORMAL:
            result=1.0f;
            break;
        case AVOID:
            result=0;
            break;
        case STRIKE:
            result=1.5f+2.0f*(float)m_strike_result*strike_result_cf/(float)base_attack;
            break;
        case PARRY:
            result=0.5f;
            break;
        case STRIKEANDPARRY:
            result=(1.5f+2.0f*(float)m_strike_result*strike_result_cf/(float)base_attack)*0.5;
            break;
        default:
            break;
    }
    return result;
}
void Person::animateCallBack(CCObject *sender){
    CCSprite *sp=(CCSprite *)sender;
    sp->removeFromParentAndCleanup(true);
}
void Person::addBlood(){
    CCSprite *blood_bg= CCSprite::create(M_PERSONBLOOD_BG);
    blood_bg->setTag(BLOODBG);//BLOODBG
    blood_bg->setScale(0.5);
    
    blood_bg->setAnchorPoint(ccp(0, 0));
    CCSprite * blood= CCSprite::create(M_PERSON_BLOOD);
    blood->setAnchorPoint(ccp(0, 0));
    blood->setScale(0.5);
    blood->setTag(BLOOD);//BLOOD
    blood->setPosition(ccp(-40, 220));
    blood_bg->setPosition(ccp(-40, 220));
    blood->setVisible(false);
    blood_bg->setVisible(false);
    this->addChild(blood_bg);
    this->addChild(blood,1);
}
void Person::reduceBloodCallBack(CCObject *sender){
    CCSprite *sp=(CCSprite *)sender;
    sp->removeFromParentAndCleanup(true);
}
void Person::changeBlood(int changeBlood){
    CCSprite *sp= (CCSprite *)this->getChildByTag(BLOOD);
    CCSprite *bloodbg=(CCSprite *)this->getChildByTag(BLOODBG);
   
    
    bloodbg->setVisible(true);
    if (type==0) {
        CCSprite *daoguang=CCSprite::createWithSpriteFrameName("daoguang_01.png");
        daoguang->setTag(DAOGUANG);//DAOGUANG
        daoguang->runAction(AnimatePacker::getInstance()->getAnimate("daoguang"));
        this->addChild(daoguang,5);
    }
    
    sp->setVisible(true);
    if(cur_hp+changeBlood>0){

        if (cur_hp+changeBlood>=m_hp) {
            cur_hp = m_hp;
        }else{
            cur_hp = cur_hp+changeBlood;
        }
        
        CCScaleTo * scale= CCScaleTo::create(fabs(changeBlood)/(float)m_hp+0.1, ((float)cur_hp/(float)m_hp)*0.5, 0.5);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(Person::changeBloodCallBack));
        CCSequence *seq=CCSequence::create(scale,func,NULL);
        char cb[10]="";
        sprintf(cb, "%d",changeBlood);
        
        CCLabelTTF *reduceBlood=CCLabelTTF::create("", "黑体", 60);
        if (changeBlood>0) {
            reduceBlood->setColor(ccGREEN);
            sprintf(cb, "+%d",changeBlood);
        }else{
            reduceBlood->setColor(ccRED);
            sprintf(cb, "%d",changeBlood);
        }
        reduceBlood->setString(cb);
        reduceBlood->setPosition(ccp(m_role->getContentSize().width-250, m_role->getContentSize().height-200));
        CCMoveTo *move=CCMoveTo::create(0.8, ccp(m_role->getContentSize().width-250, m_role->getContentSize().height+100));
        CCFadeOut *fadeout=CCFadeOut::create(0.8);
        CCCallFuncN *func1=CCCallFuncN::create(this, callfuncN_selector(Person::reduceBloodCallBack));
        CCSequence *seq1=CCSequence::create(move,func1,NULL);
        reduceBlood->runAction(seq1);
        reduceBlood->runAction(fadeout);
        m_role->addChild(reduceBlood,1);
        sp->runAction(seq);
    }else if(cur_hp+changeBlood<=0){
        
        isDead=true;
        this->unscheduleAllSelectors();
        char cb[10]="";
        sprintf(cb, "%d",changeBlood);
        CCLabelTTF *reduceBlood=CCLabelTTF::create(cb, "黑体", 60);
        reduceBlood->setColor(ccRED);
        reduceBlood->setPosition(ccp(m_role->getContentSize().width-250, m_role->getContentSize().height-200));
        CCMoveTo *move=CCMoveTo::create(0.8, ccp(m_role->getContentSize().width-250, m_role->getContentSize().height+100));
        CCFadeOut *fadeout=CCFadeOut::create(0.8);
        CCCallFuncN *func1=CCCallFuncN::create(this, callfuncN_selector(Person::reduceBloodCallBack));
        CCSequence *seq1=CCSequence::create(move,func1,NULL);
        reduceBlood->runAction(seq1);
        reduceBlood->runAction(fadeout);
        m_role->addChild(reduceBlood,1);
        CCScaleTo * scale= CCScaleTo::create(changeBlood/m_hp, 0, 0.5);
        CCCallFunc * func= CCCallFunc::create(this,callfunc_selector(Person::roleDead));
        CCSequence * seq= CCSequence::create(scale,func,NULL);
        sp->runAction(seq);
    }
}
void Person::changeBloodCallBack(){
    if (type==0) {
        this->removeChildByTag(DAOGUANG,true);
    }
    
}
void Person::attacked(){
    
}
void Person::attackedCallBack(CCObject *sender){
    
}
void Person::setZ(){

}
void Person::motionType(PersonMotion per,int dir,const char* armname){
//    m_role->getAnimation()->stop();//骨骼动画
   
    //动作类。1跑步，2站立，3攻击，4受击  dir 1.向右  2.向左
//    if (dir==1) {
//        m_role->setRotationY(0);
//    }else{
//        m_role->setRotationY(180);
//    }
    CCDelayTime *delay0= CCDelayTime::create(0.416);
    CCCallFunc *func;
    CCSequence *seq;
    char spriteframename[20]="";
    CCActionInterval *rep;
    
    
    
    
    switch (per) {
        case RUN:
            m_role->stopActionByTag(00000);
            if (dir==1) {
                sprintf(spriteframename, "%s_runr",armname);
            }else{
                sprintf(spriteframename, "%s_runl",armname);
            }
            //AnimatePacker::getInstance()->getAnimate(spriteframename)
            rep=CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate(spriteframename));
            m_role->runAction(rep);
//            m_role->init(armname);//骨骼动画
//            m_role->getAnimation()->play("run");
            
            break;
        case STAND:
            m_role->stopActionByTag(00000);
            if (dir==1) {
                sprintf(spriteframename, "%s_standr",armname);
            }else{
                sprintf(spriteframename, "%s_standl",armname);
            }
            rep=CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate(spriteframename));
            m_role->runAction(rep);
//            m_role->init(armname);
//            m_role->getAnimation()->play("stand");
            break;
        case ATTACK:
             m_role->stopActionByTag(00000);
            m_role->stopActionByTag(ATTACKACTION);
            if (dir==1) {
                sprintf(spriteframename, "%s_attackr",armname);
            }else{
                sprintf(spriteframename, "%s_attackl",armname);
            }
            rep=CCRepeatForever::create(AnimatePacker::getInstance()->getAnimate(spriteframename));
            //m_role->runAction(AnimatePacker::getInstance()->getAnimate(spriteframename));
//            m_role->init(armname);
//            m_role->getAnimation()->play("attack");
            func=CCCallFunc::create(this, callfunc_selector(Person::attackCallBack));
            seq=CCSequence::create(AnimatePacker::getInstance()->getAnimate(spriteframename),func,NULL);
            seq->setTag(ATTACKACTION);//ATTACKACTION
            m_role->runAction(seq);
            break;
        case ATTACKED:
            m_role->stopActionByTag(00000);
            if (dir==1) {
                sprintf(spriteframename, "%s_woundr",armname);
            }else{
                sprintf(spriteframename, "%s_woundl",armname);
            }
            rep=AnimatePacker::getInstance()->getAnimate(spriteframename);
            m_role->runAction(rep);
//            m_role->init(armname);
//            m_role->getAnimation()->play("attacked");
            break;
        case DEAD:
            
            break;
        case TREATED:
            if (dir==1) {
                sprintf(spriteframename, "%s_attackr",armname);
            }else{
                sprintf(spriteframename, "%s_attackl",armname);
            }
            m_role->stopActionByTag(00000);
            m_role->stopActionByTag(ATTACKACTION);
            rep=AnimatePacker::getInstance()->getAnimate(spriteframename);
//            m_role->runAction(rep);
//            m_role->init(armname);
//            m_role->getAnimation()->play("attack");
            func=CCCallFunc::create(this, callfunc_selector(Person::attackCallBack));
            seq=CCSequence::create(AnimatePacker::getInstance()->getAnimate(spriteframename),func,NULL);
            seq->setTag(ATTACKACTION);//ATTACKACTION
            m_role->runAction(seq);

            break;
        default:
            break;
    }
    
    rep->setTag(00000);
}

void Person::attackCallBack(){
    
}
void Person::roleDead(){
    
}
void Person::onExit(){
    CCSprite::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
  //  CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
Person::~Person(){
  //  CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

bool FriendlyRole::init(){
    if (!Person::init()) {
        
        return false;
    }
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendlyRole::TargetDead), "roledead", NULL);
    return true;
}
void FriendlyRole::start(){
    target=NULL;
    isAttacking=false;
    type=1;
    m_maxTouchs=10;
    label=CCLabelTTF::create();
    label->setPosition(ccp(0, 200));
    this->addChild(label);
    selectedCircle=CCSprite::create("select.png");
    selectedCircle->setVisible(false);
    selectedCircle->setPosition(ccp(0, -20));
    this->addChild(selectedCircle,-1);
    m_maxTouchs=skill->m_skillCounts;
    skillbtns= SkillButton::create(skill->m_skillTimes);
    
    skillbtns->setPosition(ccp(-150, 60));
    skillbtns->setVisible(true);
    this->addChild(skillbtns,1);
    this->schedule(schedule_selector(FriendlyRole::setZs));
    this->schedule(schedule_selector(FriendlyRole::enterAttackRegion));
    isTouch=false;
    isRunning=false;
    beatBackFlag=true;
    enterNum=0;

}

void FriendlyRole::loadPersonTexture(){
//    LoadRes::shareLoadRes()->loadRolePersonTexture02();
}
void FriendlyRole::findTarget(){
    if (target==NULL) {
        findTarget();
    }else{
        
    }
}
FriendlyRole::~FriendlyRole(){
//    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "roledead");
//    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA1);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA0);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData (ARMATURE_ACTION_DATA1);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData(ARMATURE_ACTION_DATA0);
//    this->removeFromParentAndCleanup(true);
  //  CCTextureCache::sharedTextureCache()->removeAllTextures();
}
void FriendlyRole::traceTarge(){
    float distance=ccpDistance(this->getPosition(), target->getPosition());
    if (distance<=target->m_range) {
        judgePos();
        
        motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
    }else{
        judgePos();
        motionType(RUN,aniDir, ARMATURE_ACTION_DATA1);
        CCPoint pos;
      //  isAttacking=false;
        if (aniDir==2) {
            pos=ccp(target->getPosition().x-NEAR_MINATTACKRANGE, target->getPosition().y);
        }else if(aniDir==1){
            pos=ccp(target->getPosition().x+NEAR_MINATTACKRANGE, target->getPosition().y);
        }
        CCMoveTo *move=CCMoveTo::create(distance/m_speed, ccp(target->getPosition().x, target->getPosition().y));
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(FriendlyRole::callBackStand));
        CCSequence *seq=CCSequence::create(move,func,NULL);
        this->runAction(seq);
    }
}
void FriendlyRole::setZs(){
    float z=-this->getPositionY()/1;
    this->setZOrder(z);
    if (aniDir==1) {
        skillbtns->setPosition(ccp(-150, 60));
    }else if(aniDir==2){
        skillbtns->setPosition(ccp(150, 60));
    }
}
void FriendlyRole::touchEvent(cocos2d::CCTouch *touch,bool touch_flag){
    this->unscheduleAllSelectors();
    m_role->unscheduleAllSelectors();
    this->schedule(schedule_selector(FriendlyRole::setZs));
    CCNotificationCenter::sharedNotificationCenter()->postNotification("rolemove",this);
    this->stopAllActions();
    m_role->stopAllActions();
    if (touch_flag) {//攻击

        isAttacking=false;
        
        CCPoint touchPoint= touch->getLocation();
        
        BattleLayer *bl=(BattleLayer *)this->getParent();
        for (int i=0; i<bl->appearArr->count(); i++) {
            EnemyRole *enemy=(EnemyRole *)bl->appearArr->objectAtIndex(i);
            if (enemy->isInSprite(touch)) {
                target=enemy;
                target->schedule(schedule_selector(EnemyRole::getHostility));
                break;
            }
        }
        int number=0;
        if (touch->getLocation().x>this->getPosition().x) {
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            
            number=1;
        }else{
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            number=2;
            
        }
        this->unschedule(schedule_selector(FriendlyRole::moveToSelectTarget));
        this->schedule(schedule_selector(FriendlyRole::moveToSelectTarget));
        double distance=ccpDistance(this->getPosition(), touchPoint);
        CCTraceTo *trace=CCTraceTo::create(target, this, m_speed);
        CCMoveTo *move= CCMoveTo::create(distance/m_speed, touchPoint);
        CCCallFuncND* func= CCCallFuncND::create(this, callfuncND_selector(FriendlyRole::callBackAttack), (void *)number);
        CCSequence * seq= CCSequence::create(move,func,NULL);
        seq->setTag(TOUCHATTACKACTION);//TOUCHATTACKACTION
        trace->setTag(TOUCHATTACKACTION);
        this->runAction(trace);
        
    }else{//跑步

        CCPoint touchPoint= touch->getLocation();
        isRunning=true;
        isAttacking=false;
        int number=0;
        if (touch->getLocation().x>this->getPosition().x) {
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            number=1;
            
        }else{
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            number=2;
            
        }
        target=NULL;
        double distance=ccpDistance(this->getPosition(), touchPoint);
        CCMoveTo * move= CCMoveTo::create(distance/m_speed, touchPoint);
        CCCallFuncND* func= CCCallFuncND::create(this, callfuncND_selector(FriendlyRole::callBackStand), (void *)number);
        CCSequence * seq= CCSequence::create(move,func,NULL);
        seq->setTag(TOUCHMOVEACTION);//TOUCHMOVEACTION
        this->runAction(seq);
    }
}
bool FriendlyRole::isInSprite(CCTouch * touch){
    CCPoint touchPoint=touch->getLocation();
    CCPoint reallyPoint=this->getParent()->convertToNodeSpace(touchPoint);
    CCRect rect=CCRectMake(this->getPosition().x-100, this->getPosition().y-100, 200, 200);
    if(rect.containsPoint(reallyPoint)){
        return true;
    }
    return false;
}
void FriendlyRole::enterAttackOrgin(){
    return;
    if(target!=NULL&&ccpDistance(this->getPosition(), this->target->getPosition())<m_range &&!isAttacking){
        this->unschedule(schedule_selector(FriendlyRole::enterAttackOrgin));
        
        this->stopAllActions();
        judgePos();
        motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
        
    }else{
        this->unschedule(schedule_selector(FriendlyRole::enterAttackOrgin));
        return;
    }
}
void FriendlyRole::moveToSelectTarget(){
    if (target&&ccpDistance(this->getPosition(), target->getPosition())<m_range &&!isAttacking) {
        isRunning=false;
        this->unschedule(schedule_selector(FriendlyRole::moveToSelectTarget));
        this->stopActionByTag(TOUCHATTACKACTION);
        judgePos();
        motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
    }
}
void FriendlyRole::enterAttackRegion(){
    if (isAttacking) {
        return;
    }
    
    BattleLayer *bl=(BattleLayer *)this->getParent();
    isRunning=false;
    
    for (int i=0; i<bl->appearArr->count(); i++) {
        EnemyRole *enemy=(EnemyRole *)bl->appearArr->objectAtIndex(i);
        if (ccpDistance(this->getPosition(), enemy->getPosition())<=m_range ) {
            if (target) {
                this->stopActionByTag(TOUCHATTACKACTION);
                this->unschedule(schedule_selector(FriendlyRole::enterAttackRegion));
                judgePos();
                motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
                break;
            }else{
                this->stopActionByTag(TOUCHATTACKACTION);
                target=enemy;
                ((EnemyRole *)target)->beatBack();
                target->schedule(schedule_selector(EnemyRole::getHostility));
                this->unschedule(schedule_selector(FriendlyRole::enterAttackRegion));
                judgePos();
                motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
                break;
            }
        }else{
            continue;
        }
    }
}
void FriendlyRole::TargetDead(CCObject *sender){
    EnemyRole *ene= (EnemyRole *)sender;
    if (ene==target) {
        m_role->stopAllActions();
        this->unscheduleAllSelectors();
        this->stopAllActions();
        this->target=NULL;
        beatBackFlag=true;
        isAttacking=false;
        this->stopActionByTag(ATTACKCALLBACKFUNCACTION);
        callBackStand(NULL, &aniDir);
    }
}
void FriendlyRole::callBackAttack(cocos2d::CCNode *node, void *sender){
    this->unschedule(schedule_selector(FriendlyRole::setZs));
    CCNotificationCenter::sharedNotificationCenter()->postNotification("roleposmove");
    isRunning=false;
     ((EnemyRole *)target)->beatBack();
    motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
}
void FriendlyRole::attackStand(){
    
}
void FriendlyRole::callBackStand(cocos2d::CCNode *node, void *sender){
    beatBackFlag=true;
    isRunning=false;
    isAttacking=false;
    CCSprite *blood=(CCSprite *)this->getChildByTag(BLOOD);
    blood->setVisible(false);
    CCSprite *bloodbg=(CCSprite *)this->getChildByTag(BLOODBG);
    bloodbg->setVisible(false);
    this->unschedule(schedule_selector(FriendlyRole::enterAttackRegion));
    this->schedule(schedule_selector(FriendlyRole::enterAttackRegion));
    motionType(STAND, aniDir, ARMATURE_ACTION_DATA0);
}
void FriendlyRole::attackCallBack(){
    if (!isRunning) {
        if (target->m_hp>0 &&!target->isDead) {
            motionType(STAND, aniDir, ARMATURE_ACTION_DATA0);
            CCDelayTime *dalay0= CCDelayTime::create(0.42);
            CCCallFunc *func= CCCallFunc::create(this, callfunc_selector(FriendlyRole::traceTarge));
            CCSequence *seq= CCSequence::create(dalay0,func,NULL);
            seq->setTag(ATTACKCALLBACKFUNCACTION);//ATTACKCALLBACKFUNCACTION
            isAttacking=true;
         //   CCLog("%f",-getAttackResult());
            float percent=this->bloodResult(attackModel());
            CCLog("FriendlyRole::attackCallBack  %f",percent);
            target->changeBlood(-getAttackResult()*percent);
            this->runAction(seq);
        }else{
            isAttacking=false;
            target->roleDead();
            callBackStand(NULL, 0);
        }
    }
    
}
void FriendlyRole::beatBack(){
    return;
    BattleLayer *bl=(BattleLayer *)this->getParent();
    for (int i=0; i<bl->appearArr->count(); i++) {
        EnemyRole *enemy=(EnemyRole *)bl->appearArr->objectAtIndex(i);
        if (!isRunning&&enemy->target==this&&ccpDistance(enemy->getPosition(), this->getPosition())<=m_range&&beatBackFlag&&target!=NULL){
            beatBackFlag=false;
            this->target=enemy;
            judgePos();
            motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
        }else{
            callBackStand(NULL, 0);
        }
    }
}
void FriendlyRole::judgePos(){
    if (target->getPosition().x>this->getPosition().x) {
        aniDir=1;
    }else{
        aniDir=2;
    }
    return;
    if (this->getPosition().x-this->target->getPosition().x>0&&this->getPosition().x-this->target->getPosition().x<NEAR_MINATTACKRANGE) {
        isRunning=true;
        if (this->target->getPosition().y>this->getPosition().y) {
            CCLog("←  下");
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x+NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x+NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(FriendlyRole::callBackAttack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            this->runAction(seq);
        }else{
            CCLog("←  上");
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x+NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x+NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(FriendlyRole::callBackAttack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            this->runAction(seq);
        }
    }else if(this->getPosition().x-this->target->getPosition().x<=0&&this->getPosition().x-this->target->getPosition().x>-NEAR_MINATTACKRANGE){
        isRunning=true;
        if (this->target->getPosition().y>this->getPosition().y) {
            CCLog("→  下");
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x-NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x-NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(FriendlyRole::callBackAttack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            this->runAction(seq);
        }else{
            CCLog("→  上");
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA1);
            
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x-NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x-NEAR_MAXATTACKRANGE, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(FriendlyRole::callBackAttack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            this->runAction(seq);
        }
    }else{
        if (target->getPosition().x>this->getPosition().x) {
            aniDir=1;
        }else{
            aniDir=2;
        }
        motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA0);
    }
}
void FriendlyRole::onExit(){
    Person::onExit();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
   // CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
bool CureRole::init(){
    if (!Person::init()) {
        return false;
    }
    return true;
}
void CureRole::start(){
    skillbtns= SkillButton::create(skill->m_skillTimes);
    skillbtns->setPosition(ccp(-150, 60));
    skillbtns->setVisible(true);
    this->addChild(skillbtns,1);
    this->schedule(schedule_selector(CureRole::setZs));
    type=3;
    isCure=false;
    isTouch=false;
    aniDir=1;
}
void CureRole::judgePos(){
    if (target!=NULL&& target->getPosition().x>=this->getPosition().x) {
        aniDir=1;
    }else{
        aniDir=2;
    }
}
void CureRole::loadPersonTexture(){
//    LoadRes::shareLoadRes()->loadRolePersonTexture02();
}
void CureRole::moreCureTarget(CCArray *rolearr){
   // oldperson=this->target;
    for (int i=0; i<rolearr->count(); i++) {
        Person *person=(Person *)rolearr->objectAtIndex(i);
       // this->target=person;
        CCSprite *zlxg=CCSprite::createWithSpriteFrameName(M_ZL_TX_FPNG);
        zlxg->setTag(CUREBLOODACTION);//CUREBLOODACTION
        zlxg->runAction(AnimatePacker::getInstance()->getAnimate(M_ZL_TX_DATA));
        person->addChild(zlxg,1);
        person->changeBlood(getAttackResult()+6);

    }
  //  this->target=oldperson;
}
void CureRole::cureBlood(int changeBlood){
    if (target==NULL) {
        return;
    }
    CCSprite *blood=(CCSprite *)target->getChildByTag(BLOOD);
    target->getChildByTag(BLOODBG)->setVisible(true);
    target->getChildByTag(BLOOD)->setVisible(true);
    CCSprite *zlxg=CCSprite::createWithSpriteFrameName(M_ZL_TX_FPNG);
    zlxg->setTag(CUREBLOODACTION);//CUREBLOODACTION
    zlxg->setPosition(ccp(0, 100));
    zlxg->runAction(AnimatePacker::getInstance()->getAnimate(M_ZL_TX_DATA));
    target->addChild(zlxg,1);
    if (target->cur_hp+changeBlood>= target->m_hp) {
        CCScaleTo *scale= CCScaleTo::create((float)changeBlood/(float)target->m_hp, 0.5, 0.5);
        target->cur_hp=target->m_hp;
        blood->runAction(scale);
    }else{
        float per=0;
        if ((float)(target->cur_hp+changeBlood)/(float)target->m_hp>=1.0f) {
            per=1;
            target->cur_hp=target->m_hp;
        }else{
            per=(float)(target->cur_hp+changeBlood)/(float)target->m_hp;
            target->cur_hp=(int)(target->cur_hp+changeBlood);
        }
        target->cur_hp=target->cur_hp+changeBlood;
        CCScaleTo *scale=CCScaleTo::create((float)changeBlood/(float)target->m_hp, per*0.5, 0.5);
        blood->runAction(scale);
    }
}
void CureRole::cureTarget(){
    //   float changeBlood= getAttackResult();
    
}
void CureRole::cureCallBack(){
  //  judgePos();
    
    if (target!=NULL) {
        CCSprite *sp=(CCSprite *)target->getChildByTag(CUREBLOODACTION);
        if (sp) {
            sp->removeFromParentAndCleanup(true);
        }
        motionType(TREATED, aniDir, ARMATURE_ACTION_DATA2);
    }else {
        motionType(STAND, aniDir, ARMATURE_ACTION_DATA2);
    }
    
    
}
void CureRole::attackCallBack(){
    CCDelayTime *delay=CCDelayTime::create(1.5);
    CCDelayTime *delay1=CCDelayTime::create(0.45);
    isCure=true;
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(CureRole::callBackStand));
    CCSequence *seq1=CCSequence::create(delay1,fun1,NULL);
   // this->runAction(seq1);
    motionType(STAND,aniDir, ARMATURE_ACTION_DATA2);
    CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(CureRole::cureCallBack));
    CCSequence *seq=CCSequence::create(delay,func,NULL);
    cureBlood(getAttackResult());
    this->runAction(seq);
}
void CureRole::touchEvent(cocos2d::CCTouch *touch, bool touch_flag){
    this->stopAllActions();
    
    
    this->schedule(schedule_selector(CureRole::setZs));
    
    if (touch_flag) {//治疗
        m_role->stopAllActions();
        this->stopAllActions();
        if (isCure) {
            return;
        }
        if (touch->getLocation().x>this->getPosition().x) {
            aniDir=1;
            motionType(TREATED, aniDir, ARMATURE_ACTION_DATA2);
            
        }else{
            aniDir=2;
            motionType(TREATED, aniDir, ARMATURE_ACTION_DATA2);
            
        }
        
        
    }else{//跑步
        m_role->stopAllActions();
        this->stopAllActions();
        CCPoint touchPoint= touch->getLocation();
        
        int number=0;
        if (touch->getLocation().x>this->getPosition().x) {
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA3);
            number=1;
            
            //   enterNum=2;
        }else{
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA3);
            number=2;
            
            //   enterNum=1;
        }
        double distance=ccpDistance(this->getPosition(), touchPoint);
        CCMoveTo * move= CCMoveTo::create(distance/m_speed, touchPoint);
        CCCallFuncND* func= CCCallFuncND::create(this, callfuncND_selector(CureRole::cureCallBack), (void *)number);
        CCSequence * seq= CCSequence::create(move,func,NULL);
        seq->setTag(TOUCHMOVEACTION);
        this->runAction(seq);
    }
}

void CureRole::setZs(){
    float z=-this->getPositionY()/1;
    this->setZOrder(z);
    if (aniDir==1) {
        skillbtns->setPosition(ccp(-150, 60));
    }else if(aniDir==2){
        skillbtns->setPosition(ccp(150, 60));
    }
}
bool CureRole::isInSprite(CCTouch * touch){
    CCPoint touchPoint=touch->getLocation();
    CCPoint reallyPoint=this->getParent()->convertToNodeSpace(touchPoint);
    CCRect rect=CCRectMake(this->getPosition().x-100, this->getPosition().y-100, 200, 200);
    if(rect.containsPoint(reallyPoint)){
        return true;
    }
    return false;
}
void CureRole::callBackStand(cocos2d::CCObject *sender,void *num){
    isCure=false;
    
    motionType(STAND,aniDir, ARMATURE_ACTION_DATA2);
}
CureRole::~CureRole(){
//    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA2);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA3);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData(ARMATURE_ACTION_DATA2);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData(ARMATURE_ACTION_DATA3);
//    this->removeFromParentAndCleanup(true);
  //  CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}
bool EnemyRole::init(){
    if (!Person::init()) {
        return false;
    }
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EnemyRole::targetMove), "rolemove", NULL);
    aniDir=1;
    ismoveing=false;
    type=0;
    isDead=false;
    target=NULL;
    isRunning=false;
    beatBackFlag=true;
    g_bPinBallFlag=true;
    releaseTime=0;
    oldtouchnum=0;
    return true;
}
void EnemyRole::start(){
    select_red=CCSprite::create("select_red.png");
    select_red->setPosition(ccp(this->getContentSize().width/2,this->getContentSize().height/2));
    this->addChild(select_red,-1);
    select_red->setVisible(false);
    this->schedule(schedule_selector(EnemyRole::setZs));
    if (this->type==2) {
        this->schedule(schedule_selector(EnemyRole::releaseSkillControl), 1);
    }
}
void EnemyRole::enterAttackRegion(){
    if (ccpDistance(target->getPosition(), this->getPosition())<=m_range) {

        this->unschedule(schedule_selector(EnemyRole::changeDir));
        this->unschedule(schedule_selector(EnemyRole::enterAttackRegion));
        this->stopActionByTag(6000);
       // callBackStand(NULL, 0);
        
            judgeSelfPos();
        
    }
}
void EnemyRole::targetMove(CCObject *sender){
    FriendlyRole *role=(FriendlyRole *)sender;
    if (target==role&&!isAttacking) {
        traceTarge();
        CCLog("Here!!!!");
    }
}
void EnemyRole::loadPersonTexture(){
//    LoadRes::shareLoadRes()->loadBossTexture();
}
void EnemyRole::findTarget(){
    if (target!=NULL) {
        
    }else{
     //   CCLog("%d",GameData::shareGameData()->roleArr->count());
        int arc= arc4random()%(GameData::shareGameData()->roleArr->count());
        this->target=(Person *)GameData::shareGameData()->roleArr->objectAtIndex(arc);
        this->schedule(schedule_selector(EnemyRole::setZs));
        if (target) {
            if (this->getPosition().x>this->target->getPosition().x) {
                aniDir=2;
            }else{
                aniDir=1;
            }
        }
        
        MoveToTarget();
    }
}
void EnemyRole::MoveToTarget(){
    m_role->stopAllActions();
    motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
    CCTraceTo *trace=CCTraceTo::create(target, this,m_speed);
    trace->setTag(6000);
    this->runAction(trace);
    this->unschedule(schedule_selector(EnemyRole::setZs));
    this->schedule(schedule_selector(EnemyRole::setZs));
    this->schedule(schedule_selector(EnemyRole::enterAttackRegion));
    return;

    CCMoveTo *move;
    int motionNum=1;
    float distance;
    this->schedule(schedule_selector(EnemyRole::setZs));
    if (this->getPosition().x>this->target->getPosition().x) {
        distance=ccpDistance(this->getPosition(), ccp(target->getPosition().x+NEARATTACK_RANGE, target->getPosition().y));
        move=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x+NEARATTACK_RANGE, this->target->getPosition().y));
        motionNum=2;
        aniDir=2;
    }else{
        distance=ccpDistance(this->getPosition(), ccp(target->getPosition().x-NEARATTACK_RANGE, target->getPosition().y));
        move=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x-NEARATTACK_RANGE, this->target->getPosition().y));
        motionNum=1;
        aniDir=1;
    }
    isRunning=true;
    motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
    CCDelayTime *delay=CCDelayTime::create(1.5);
    CCCallFuncND *func=CCCallFuncND::create(this, callfuncND_selector(EnemyRole::callBackStand), (void *)motionNum);
    CCSequence *seq=CCSequence::create(delay,move,func,NULL);
    seq->setTag(200001);
    this->runAction(seq);
}
void EnemyRole::PinballHitEnemy(CCNode *pSender){
   // CCLog("hit me");
    g_pNodePinBall=pSender;
    g_bPinBallFlag=false;
    this->schedule(schedule_selector(EnemyRole::PinballDis));
}
void EnemyRole::PinballDis(){
    if (ccpDistance(g_pNodePinBall->getPosition(), this->getPosition())<100) {
        g_bPinBallFlag=false;
        return;
    }
    g_bPinBallFlag=true;
    this->unschedule(schedule_selector(EnemyRole::PinballDis));
    
}
void EnemyRole::callBackStand(cocos2d::CCObject *sender, void *num){
    isRunning=false;
    motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA4);
    CCSprite *blood=(CCSprite *)this->getChildByTag(BLOOD);
    CCSprite *bloodbg=(CCSprite *)this->getChildByTag(BLOODBG);
    bloodbg->setVisible(false);
    blood->setVisible(false);
}
void EnemyRole::checkTarget(){
    if (target->cur_hp>0) {
        
    }
}
bool EnemyRole::isInSprite(cocos2d::CCTouch *touch){
    CCPoint touchPoint=touch->getLocation();
    CCPoint reallyPoint=this->getParent()->convertToNodeSpace(touchPoint);
    CCRect rect=CCRectMake(this->getPosition().x-100, this->getPosition().y-100, 200, 200);
    if(rect.containsPoint(reallyPoint)){
        return true;
    }
    return false;
}
void EnemyRole::roleDead(){
    BattleLayer *dw= (BattleLayer *)this->getParent();
    dw->appearArr->removeObject(this);
    GameData::shareGameData()->enemyArr->removeObject(this);
    isDead=true;
    this->getChildByTag(BLOODBG)->setVisible(false);
    this->getChildByTag(BLOOD)->setVisible(false);
    target=NULL;
    this->stopAllActions();
//    this->m_role->getAnimation()->stop();//骨骼动画
  //  m_role->stopActionByTag(00000);
    this->m_role->stopAllActions();
    
    
    this->unscheduleAllSelectors();
    CCFadeOut *fadeout=CCFadeOut::create(0.15);
    CCFadeIn *fadein=CCFadeIn::create(0.15);
    CCSequence *seq=CCSequence::create(fadein,fadeout,NULL);
    CCRepeat *rep=CCRepeat::create(seq, 3);
    CCDelayTime *delay=CCDelayTime::create(1);
    CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyRole::callFadeInOutFunc));
    CCSequence *seq2=CCSequence::create(delay,func,NULL);
    m_role->runAction(rep);
    this->runAction(seq2);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("roledead", this);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "roleposmove");
    if (dw->appearArr->count()==0) {
        dw->monsterVisible();
    }
}
void EnemyRole::releaseSkillControl(){
    
    if (releaseTime==0) {
        //释放技能
        if(skillId==1){
//            m_role->getAnimation()->stop();
           // m_role->stopActionByTag(00000);
            EnemySkillInfo *skill=EnemySkillInfo::create(skillId);
            this->getParent()->addChild(skill);
            skill->releaseSkill(this->getPosition());
            releaseTime=10;
            return;
        }else if(skillId==2){
//            m_role->getAnimation()->stop();
           // m_role->stopActionByTag(00000);
         //   this->stopAllActions();
            this->unscheduleAllSelectors();
            
            motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA4);
            EnemySkillInfo *skill=EnemySkillInfo::create(skillId);
            this->getParent()->addChild(skill);
            skill->releaseSkill(this->getPosition());
            releaseTime=10;
            this->schedule(schedule_selector(EnemyRole::releaseSkillControl), 1);
            return;
        }
    }else if(releaseTime>0){
        releaseTime--;
        
    }else{
        CCLOG("出错了！");
        return;
    }
    if (this->type==2) {
        
    }else{
        return;
    }
}
void EnemyRole::callFadeInOutFunc(){
    this->removeFromParentAndCleanup(true);
}

void EnemyRole::changeDir(){
    if (aniDir!=oldtouchnum) {
        motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);

        oldtouchnum=aniDir;
    }

}
void EnemyRole::traceTarge(){
    this->unschedule(schedule_selector(EnemyRole::setZs));
    this->schedule(schedule_selector(EnemyRole::setZs));
    
    if (ccpDistance(target->getPosition(), this->getPosition())>=m_range-10) {
        this->unschedule(schedule_selector(EnemyRole::changeDir));
        m_role->stopAllActions();
        this->stopAllActions();
       // this->schedule(schedule_selector(EnemyRole::setZs));
        motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
        
        CCTraceTo *trace=CCTraceTo::create(target, this,m_speed);
        trace->setTag(6000);
        this->runAction(trace);
        this->schedule(schedule_selector(EnemyRole::changeDir));
        
        this->schedule(schedule_selector(EnemyRole::enterAttackRegion));
    }else{
        m_role->stopAllActions();
        this->stopAllActions();
        this->unschedule(schedule_selector(EnemyRole::changeDir));
        motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA4);
    }

    return;
    
    
    
    
    this->stopActionByTag(200001);
    this->stopAllActions();
    this->schedule(schedule_selector(EnemyRole::setZs));
    float distance= ccpDistance(this->getPosition(), target->getPosition());
    if (distance<=m_range) {
        aniDir=judgePos(this->getPosition(), target->getPosition());
        judgeSelfPos();
    }else{
        
        if (this->getPosition().x>this->target->getPosition().x) {
            CCMoveTo *runtotarget=CCMoveTo::create(distance/m_speed, ccp(target->getPosition().x+NEARATTACK_RANGE, target->getPosition().y));
            int a=2;
            aniDir=2;
            CCCallFuncND *fun=CCCallFuncND::create(this, callfuncND_selector(EnemyRole::runCallBack), &a);
            
            CCSequence *seq=CCSequence::create(runtotarget,fun,NULL);
            this->runAction(seq);
        }else{
            CCMoveTo *runtotarget=CCMoveTo::create(distance/m_speed, ccp(target->getPosition().x-NEARATTACK_RANGE, target->getPosition().y));
            int a=1;
            aniDir=1;
            CCCallFuncND *fun=CCCallFuncND::create(this, callfuncND_selector(EnemyRole::runCallBack), &a);
            CCSequence *seq=CCSequence::create(runtotarget,fun,NULL);
            this->runAction(seq);
        }
        isRunning=true;
        motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
        
        
    }
}
void EnemyRole::judgeSelfPos(){
    if (target==NULL) {
        return;
    }
    
    if ( this->getPosition().x-this->target->getPosition().x>0&&this->getPosition().x-this->target->getPosition().x<=m_range&&!ismoveing) {
        isRunning=true;
        this->stopActionByTag(1001);
        m_role->stopAllActions();
        this->stopAllActions();
        ismoveing=true;
        if (this->target->getPosition().y>this->getPosition().y) {
            CCLog("←  下");
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
            
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x+m_range, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x+m_range, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyRole::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }else{
            CCLog("←  上");
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
            
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x+m_range, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x+m_range, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyRole::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }
    }else if(this->getPosition().x-this->target->getPosition().x<=0&&this->getPosition().x-this->target->getPosition().x>=-(m_range)&&!ismoveing){
        isRunning=true;
        ismoveing=true;
        this->stopActionByTag(1001);
        m_role->stopAllActions();
        this->stopAllActions();
        
        if (this->target->getPosition().y>this->getPosition().y) {
            CCLog("→  下");
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
            
            
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x-m_range, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x-m_range, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyRole::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }else{
            CCLog("→  上");
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
            
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x-m_range, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x-m_range, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyRole::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }
    }else{
        
        if (target->getPosition().x>this->getPosition().x) {
            aniDir=1;
        }else{
            aniDir=2;
        }
        if (!isRunning) {
            isAttacking=true;
            motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA4);
        }
    }
}
void EnemyRole::runCallBack(cocos2d::CCObject *sender, void *num){
    isRunning=false;
    float distance= ccpDistance(this->getPosition(), target->getPosition());
    if (distance<=m_range) {
        judgeSelfPos();
    }else{
        traceTarge();
    }
}
void EnemyRole::beatBack(){
    
  //  getHostility();
    
   // getHostility();
    for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
        FriendlyRole *enemy=(FriendlyRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
        if (enemy->target==this&&ccpDistance(enemy->getPosition(), this->getPosition())<=m_range&&beatBackFlag)
        {
           // m_role->stopAllActions();
           // this->stopAllActions();
            beatBackFlag=false;
            this->target=enemy;
            judgeSelfPos();
        }
    }
}
void EnemyRole::getHostility(){
    for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
        Person *friends=(Person *)GameData::shareGameData()->roleArr->objectAtIndex(i);
        if(friends->hostility>this->target->hostility&&friends->target==this&&friends->isAttacking){
            if(friends==this->target){
                this->unschedule(schedule_selector(EnemyRole::getHostility));
                return;
            }else{
                if (ccpDistance(friends->getPosition(), this->getPosition())<=m_range+100) {
                    target=friends;
                   // m_role->stopAllActions();
                    traceTarge();
                    CCLog("EnemyRole::getHostility  if ");
                }else{
                    CCLog("EnemyRole::getHostility  else ");
                   // traceTarge();
                }
                
                
            }
        }
    }
}
int EnemyRole::judgePos(CCPoint m_pos,CCPoint target_pos){
    if (m_pos.x>target_pos.x) {
        return 2;
    }else{
        return 1;
    }
}
void EnemyRole::attackCallBack(){
    getHostility();
    if (target->m_hp>0) {
        CCDelayTime *dalay0= CCDelayTime::create(1);
        CCCallFunc *func= CCCallFunc::create(this, callfunc_selector(EnemyRole::traceTarge));
        CCSequence *seq= CCSequence::create(dalay0,func,NULL);
        seq->setTag(ATTACKCALLBACKFUNCACTION);
        
        this->runAction(seq);
         motionType(STAND, aniDir, ARMATURE_ACTION_DATA4);
        float percent=this->bloodResult(attackModel());
        target->changeBlood(-getAttackResult()*percent);
    }
}
void EnemyRole::setZs(){
    if (target) {
        if (this->getPosition().x>this->target->getPosition().x) {
            aniDir=2;
        }else{
            aniDir=1;
        }
    }
  
        float z=-this->getPositionY()/1;
        this->setZOrder(z);
   
    
}
void EnemyRole::onExit(){

}
EnemyRole::~EnemyRole(){
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "rolemove");

}
static int Regional2=200;
bool MasterRole::init(){
    if (!Person::init()) {
        return false;
    }
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MasterRole::enemyDead), "roledead", NULL);
    return true;
}
void MasterRole::start(){
    isAttacking=false;
    info0=new attackInfo();
    isMoving=false;
    isRunning=false;
    isTouch=false;
    m_strongTouch=false;
    p_endTouch=NULL;
    m_endTouchFlag=false;
    m_nSkillReleaseFlag=false;
    type=2;
    aniDir=0;
    touch_pos=NULL;
    skillbtns= SkillButton::create(11.0f);
    skillbtns->setPosition(ccp(-150, 60));
    skillbtns->setVisible(true);
    this->addChild(skillbtns,1);
    addSkill();
    this->schedule(schedule_selector(MasterRole::setZs));
    this->schedule(schedule_selector(MasterRole::enterAttackRegion));
}
void MasterRole::loadPersonTexture(){
//    LoadRes::shareLoadRes()->loadEnemyPersonTexture();
}
bool MasterRole::isInSprite(cocos2d::CCTouch *touch){
    CCPoint touchPoint=touch->getLocation();
    CCPoint reallyPoint=this->getParent()->convertToNodeSpace(touchPoint);
    CCRect rect=CCRectMake(this->getPosition().x-100, this->getPosition().y-100, 200, 200);
    
    
    if(rect.containsPoint(reallyPoint)){
        return true;
    }
    return false;
}
void MasterRole::setZs(){
    float z=-this->getPositionY()/1;
    this->setZOrder(z);
    if (aniDir==1) {
        skillbtns->setPosition(ccp(-150, 60));
    }else if(aniDir==2){
        skillbtns->setPosition(ccp(150, 60));
    }
}
void MasterRole::touchSchedul(){
    if (fireBall==NULL) {
        this->stopAllActions();
        this->unscheduleAllSelectors();
        m_strongTouch=false;
        isAttacking=false;
        this->schedule(schedule_selector(MasterRole::enterAttackRegion));
    }
}
void MasterRole::touchEvent(cocos2d::CCTouch *touch, bool touch_flag){
    this->stopActionByTag(TOUCHMOVEACTION);
    this->stopActionByTag(TOUCHATTACKACTION);
    this->stopActionByTag(302);
    m_strongTouch=true;
    if (fireBall) {
        p_endTouch=touch;
        m_endTouchFlag=touch_flag;
        this->unschedule(schedule_selector(MasterRole::touchSchedul));
        this->schedule(schedule_selector(MasterRole::touchSchedul));
        return;
    }
    this->stopAllActions();
    this->unscheduleAllSelectors();
    
    this->schedule(schedule_selector(MasterRole::setZs));
    if (touch_flag) {//远程
        m_strongTouch=false;
        isMoving=false;
        isRunning=false;
        isTouch=false;
        isAttacking=false;
        touch_pos=NULL;
        
        if (touch->getLocation().x>this->getPosition().x&&!isAttacking) {
            aniDir=1;
            motionType(ATTACK,aniDir, ARMATURE_ACTION_DATA6);
            moveFireball();
            isAttacking=true;
            
        }else{
            if(!isAttacking) {
                aniDir=2;
                motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA6);
                moveFireball();
                isAttacking=true;
                
            }
            
        }
    }else{//跑步
        m_strongTouch=false;
        isMoving=false;
        isRunning=true;
        isTouch=false;
        isAttacking=false;
        CCPoint touchPoint= touch->getLocation();
        touch_pos=NULL;
        m_role->stopAllActions();
        
        int number=0;
        if (touch->getLocation().x>this->getPosition().x) {
            aniDir=1;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA7);
            number=1;
            
        }else{
            aniDir=2;
            motionType(RUN, aniDir, ARMATURE_ACTION_DATA7);
            number=2;
            
        }
        double distance=ccpDistance(this->getPosition(), touchPoint);
        CCMoveTo * move= CCMoveTo::create(distance/m_speed, touchPoint);
        
        CCCallFuncND* func= CCCallFuncND::create(this, callfuncND_selector(MasterRole::callBackStand), (void *)number);
        CCSequence *seq= CCSequence::create(move,func,NULL);
        seq->setTag(TOUCHMOVEACTION);
        this->runAction(seq);
    }
}
void MasterRole::callBackAttack(cocos2d::CCNode *node, void *sender){
    isAttacking=false;
    if (target!=NULL) {
        this->unschedule(schedule_selector(MasterRole::enterAttackRegion));
        judgePos();
        moveFireball();
        isAttacking=true;
        motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA6);
    }else{
        this->unschedule(schedule_selector(MasterRole::enterAttackRegion));
        motionType(STAND, aniDir, ARMATURE_ACTION_DATA6);
    }
}
void MasterRole::releaseSkill(CCTouch *pTouch){
    CCPoint m_touchPos=pTouch->getLocation();
    BattleLayer *bl=(BattleLayer *)this->getParent();
    for (int i=0; i<bl->appearArr->count(); i++) {
        EnemyRole *enemy=(EnemyRole *)bl->appearArr->objectAtIndex(i);
        
        info0->charid=GameData::shareGameData()->chaId;
        info0->attack_id=this->m_id;
        time_t tt=time(NULL);
        char time[50]="";
        sprintf(time, "%ld",tt);
        
        info0->attack_time=time;
        info0->skillId=skillId;
        info0->countofAttack=0;
        
        
        
        
        if (ccpDistance(enemy->getPosition(), m_touchPos)<300) {
            attackedInfo *infos=new attackedInfo();
            infos->attacked_id=enemy->m_id;
            infos->attacked_curblood=enemy->cur_hp;
            infos->attacked_reduceblood=20;
            info0->attackeds->addObject(infos);
            enemy->changeBlood(-(20+arc4random()%10));
        }

        if (!this->getActionByTag(6666)) {
            CCDelayTime *delaytime=CCDelayTime::create(2);
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(MasterRole::sendAttackJson));
            CCSequence *seq=CCSequence::create(delaytime,func,NULL);
            seq->setTag(6666);
            this->runAction(seq);
        }else{
            return;
        }
        
        
        
    }
}

void MasterRole::sendAttackJson(){
    return;
    CSJson::FastWriter writer;
    CSJson::Value root;
    CSJson::Value attackeds;
    root["charid"]=GameData::shareGameData()->chaId;
    root["counts"]=1;
    root["strikerId"]=this->m_id;
    root["time"]=info0->attack_time;
    root["skillId"]=1;
    for (int i=0;i<info0->attackeds->count();i++) {
        attackedInfo *infos=(attackedInfo *)info0->attackeds->objectAtIndex(i);
        CCLog("%d",infos->attacked_id);
        attackeds[i]["attackedId"]=infos->attacked_id;
        attackeds[i]["curhp"]=infos->attacked_curblood;
        attackeds[i]["changeblood"]=infos->attacked_reduceblood;
    }
    root["enemyArray"]=attackeds;
    std::string json_str=writer.write(root);
    CCLog("%s",json_str.c_str());
    SocketClient *socket=GameData::shareGameData()->socket;
    Message *msg=(Message *)socket->constructMessage(json_str.c_str(), CMID_SENDBATTLESTRIKEINFO);
    socket->sendMessage_(msg, true);
    info0->attackeds->removeAllObjects();
    this->schedule(schedule_selector(MasterRole::receiveAttackJson));
}
void MasterRole::receiveAttackJson(){
    Message *msg=(Message *)GameData::shareGameData()->msg_dic->objectForKey(CMID_SENDBATTLESTRIKEINFO);
    if (msg) {
        this->unschedule(schedule_selector(MasterRole::receiveAttackJson));
        GameData::shareGameData()->msg_dic->removeObjectForKey(CMID_SENDBATTLESTRIKEINFO);
        CCLog("%s",msg->data);
    }
}
void MasterRole::enterAttackRegion(){
    BattleLayer *bl=(BattleLayer *)this->getParent();
    for (int i=0; i<bl->appearArr->count(); i++) {
        EnemyRole *enemy=(EnemyRole *)bl->appearArr->objectAtIndex(i);
        if (ccpDistance(this->getPosition(), enemy->getPosition())<m_range&&!m_strongTouch&&!isAttacking&&!enemy->isDead) {
            if (target!=NULL) {
                this->unschedule(schedule_selector(MasterRole::enterAttackRegion));
                judgePos();
                moveFireball();
                motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA6);
                isAttacking=true;
                break;
            }else{
                this->unschedule(schedule_selector(MasterRole::enterAttackRegion));
                target=enemy;
                this->getParent()->removeChildByTag(1023);
                fireBall=NULL;
                this->judgePos();
                this->moveFireball();
                isAttacking=true;
                this->motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA6);
                break;
            }
            
        }else{
            
        }
    }
}
void MasterRole::callBackStand(cocos2d::CCNode *node, void *sender){
    isRunning=false;
    isAttacking=false;
    motionType(STAND, aniDir, ARMATURE_ACTION_DATA6);
    this->unschedule(schedule_selector(MasterRole::enterAttackRegion));
    this->schedule(schedule_selector(MasterRole::enterAttackRegion));
}
void MasterRole::enemyDead(CCObject *sender){
    EnemyRole *enemy=(EnemyRole *)sender;
    if (enemy==target) {
        this->getParent()->removeChildByTag(1023,true);
        this->stopActionByTag(302);
        this->unscheduleAllSelectors();
        this->stopAllActions();
        if (fireBall) {
            fireBall->stopAllActions();
        }
        m_role->stopAllActions();
        this->target=NULL;
        this->fireBall=NULL;
        //m_role->stopActionByTag(00000);
        motionType(STAND, aniDir, ARMATURE_ACTION_DATA6);
        CCDelayTime *delay=CCDelayTime::create(1.5);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(MasterRole::callBackStand));
        CCSequence *seq=CCSequence::create(delay,func,NULL);
        this->runAction(seq);
//        this->m_role->getAnimation()->stop();
        
        
        //callBackStand(NULL, 0);
    }
    else
    {
        return;
    }
}
void MasterRole::judgePos(){
    if (target==NULL) {
//        m_role->getAnimation()->stop();
      //  m_role->stopActionByTag(00000);
        return;
    }else{
        if (target->getPosition().x>=this->getPosition().x) {
            aniDir=1;
        }else{
            aniDir=2;
        }

    }
}
void MasterRole::whetherExplodeOver(){
    
}
void MasterRole::targetExplode(){
    this->getParent()->removeChildByTag(1023);
    fireBall=NULL;
    isMoving=false;
    if(target!=NULL){
        
        float percent=this->bloodResult(attackModel());
        CCLog("targetExplode %f",percent);
        target->changeBlood(-getAttackResult()*percent);
        attackPtc=CCParticleSystemQuad::create(PARTICLE_EXPLODE_PLIST);
        attackPtc->setAutoRemoveOnFinish(true);
        attackPtc->setAutoRemoveOnFinish(true);
        attackPtc->setPosition(ccp(0, 0));
        target->addChild(attackPtc,2);
    }
}
void MasterRole::moveFireball(){
    if (!isRunning) {
        this->unschedule(schedule_selector(MasterRole::moveFireball));
        fireBall=CCParticleSystemQuad::create(PARTICLE_FIREBALL_PLIST);
        fireBall->setPosition(this->getPosition());
        fireBall->setAutoRemoveOnFinish(true);
        fireBall->setTag(1023);
        isMoving=true;
        this->getParent()->addChild(fireBall,2);
        float distance=ccpDistance(this->getPosition(), target->getPosition());
        CCMoveTo *fireMove=CCMoveTo::create(distance/2000, target->getPosition());
        CCCallFunc *moveEndFunc=CCCallFunc::create(this, callfunc_selector(MasterRole::targetExplode));
        CCSequence *seq=CCSequence::create(fireMove,moveEndFunc,NULL);
        seq->setTag(3333);
        fireBall->runAction(seq);
    }else{
        this->unschedule(schedule_selector(MasterRole::moveFireball));
        this->schedule(schedule_selector(MasterRole::moveFireball));
    }
}
void MasterRole::addSkill(){
    skill=SkillsInfo::create(REGIONALSKILL);
    //this->getParent()->addChild(skill);
}

void MasterRole::attackCallBack(){
    if (isAttacking&&target&&!m_strongTouch&&!isRunning&&!target->isDead) {
        this->unschedule(schedule_selector(MasterRole::enterAttackRegion));
        
//        CCDelayTime *delay1=CCDelayTime::create(0.45);
//        CCSequence *seq1=CCSequence::create(delay1,NULL);
//        seq1->setTag(TOUCHATTACKACTION);
//        this->runAction(seq1);
        CCDelayTime *delay=CCDelayTime::create(1.5);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(MasterRole::callBackAttack));
        CCSequence *seq=CCSequence::create(delay,func,NULL);
        seq->setTag(302);
        this->runAction(seq);
        motionType(STAND, aniDir, ARMATURE_ACTION_DATA6);
    }else{
        this->unschedule(schedule_selector(MasterRole::enterAttackRegion));
        CCDelayTime *delay=CCDelayTime::create(1.5);
        
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(MasterRole::callBackStand));
        CCSequence *seq=CCSequence::create(delay,func,NULL);
        
        this->runAction(seq);
        motionType(STAND, aniDir, ARMATURE_ACTION_DATA6);
    }
}
MasterRole::~MasterRole(){
//    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA6);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA7);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData (ARMATURE_ACTION_DATA6);
//    CCArmatureDataManager::sharedArmatureDataManager()->removeTextureData(ARMATURE_ACTION_DATA7);
//    this->removeFromParentAndCleanup(true);
   // CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

bool EnemyLongRangeBoss::init(){
    if (!EnemyRole::init()) {
        return false;
    }
    
    
    return true;
}
void EnemyLongRangeBoss::loadPersonTexture(){
   //  LoadRes::shareLoadRes()->loadBossTexture();
}
void EnemyLongRangeBoss::traceTarge(){
    this->unschedule(schedule_selector(EnemyLongRangeBoss::setZs));
    this->schedule(schedule_selector(EnemyLongRangeBoss::setZs));
    if (ccpDistance(target->getPosition(), this->getPosition())>500) {
        motionType(RUN, aniDir, ARMATURE_ACTION_DATA5);
        CCTraceTo *trace=CCTraceTo::create(target, this,m_speed);
        trace->setTag(6000);
        this->runAction(trace);
        
        this->schedule(schedule_selector(EnemyLongRangeBoss::enterAttackRegion));
    }else{
        
        motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA4);
    }
}
void EnemyLongRangeBoss::judgeSelfPos(){
    if (target==NULL) {
        return;
    }
    if ( this->getPosition().x-this->target->getPosition().x>0&&this->getPosition().x-this->target->getPosition().x<110) {
        isRunning=true;
        this->stopActionByTag(1001);
        if (this->target->getPosition().y>this->getPosition().y) {
            CCLog("←  下");
            motionType(RUN, 2, ARMATURE_ACTION_DATA5);
            aniDir=2;
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x+150, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x+150, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyLongRangeBoss::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }else{
            CCLog("←  上");
            motionType(RUN, 2, ARMATURE_ACTION_DATA5);
            aniDir=2;
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x+150, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x+150, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyLongRangeBoss::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }
    }else if(this->getPosition().x-this->target->getPosition().x<=0&&this->getPosition().x-this->target->getPosition().x>-110){
        isRunning=true;
        this->stopActionByTag(1001);
        if (this->target->getPosition().y>this->getPosition().y) {
            CCLog("→  下");
            motionType(RUN, 1, ARMATURE_ACTION_DATA5);
            aniDir=1;
            
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x-150, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x-150, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyLongRangeBoss::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }else{
            CCLog("→  上");
            motionType(RUN, 1, ARMATURE_ACTION_DATA5);
            aniDir=1;
            float distance=ccpDistance(this->getPosition(), ccp(this->target->getPosition().x-150, this->target->getPosition().y));
            CCMoveTo *moveToPos=CCMoveTo::create(distance/m_speed, ccp(this->target->getPosition().x-150, this->target->getPosition().y));
            CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemyLongRangeBoss::runCallBack));
            CCSequence *seq=CCSequence::create(moveToPos,func,NULL);
            seq->setTag(1001);
            this->runAction(seq);
        }
    }else{
        if (target->getPosition().x>this->getPosition().x) {
            aniDir=1;
        }else{
            aniDir=2;
        }
        if (!isRunning) {
            motionType(ATTACK, aniDir, ARMATURE_ACTION_DATA4);
        }
    }
}
void EnemyLongRangeBoss::runCallBack(cocos2d::CCObject *sender, void *num){
    isRunning=false;
    float distance= ccpDistance(this->getPosition(), target->getPosition());
    if (distance<=500) {
        //touchNum=judgePos(this->getPosition(), target->getPosition());
        judgeSelfPos();
    }else{
        traceTarge();
    }
}

void EnemyLongRangeBoss::enterAttackRegion(){
    if (ccpDistance(target->getPosition(), this->getPosition())<=500) {
        this->unschedule(schedule_selector(EnemyLongRangeBoss::enterAttackRegion));
        this->stopActionByTag(6000);
        
        callBackStand(NULL, 0);
        judgeSelfPos();
    }
}
