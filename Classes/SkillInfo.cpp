//
//  SkillInfo.cpp
//  TestGame
//
//  Created by lh on 13-12-26.
//  www.9miao.com
//

#include "SkillInfo.h"
#include "AnimatePacker.h"
#include "GameData.h"
#include "ResourceName.h"
#include "PersonTemplate.h"
#include "AnimatePacker.h"
SkillsInfo::SkillsInfo()
:m_skillBuffid(0),
m_skillCounts(0),m_skillCri(0),
m_skillCrp(0),
m_skillName(""),
m_skillTypeId(0),
m_skillRange(0),
m_skillTimes(0),
m_skillScript(0),
moveeffect(0)
{
    
}
SkillsInfo::~SkillsInfo(){
    
}
SkillsInfo *SkillsInfo::create(int skillId){
    SkillsInfo* s_skill=new SkillsInfo();
    if (s_skill) {
        
        s_skill->initWithSkillId(skillId);
        return s_skill;
    }
    CC_SAFE_DELETE(s_skill);
    return NULL;
}
void SkillsInfo::regionalSKill(){
    AnimatePacker::getInstance()->loadAnimations("dihuo.xml");
    this->initWithSpriteFrameName("dihuo_00000.png");
    this->runAction(AnimatePacker::getInstance()->getAnimate("dihuoAni"));
}
void SkillsInfo::initWithSkillId(int skillId){
    CSJson::Reader reader;
    CSJson::Value root;
    CCString  * str= CCString::createWithContentsOfFile(SKILLJSONNAME);
    CCLOG("%s",str->getCString());
    if (reader.parse(str->getCString(), root)) {
        CSJson::Value data=root["data"];
        CSJson::Value row=data["row"];
        char skillname[20]="";
        sprintf(skillname, "%d",skillId);
        CSJson::Value skilldata=data[skillname];
        m_skillTypeId=skilldata[SKILLTYPE].asInt();
        m_skillCrp=skilldata[SKILLCRP].asInt();
        m_skillCri=skilldata[SKILLCRI].asInt();
        m_skillScript=skilldata[SKILLSCRIPT].asString().c_str();
        m_skillBuffid=skilldata[SKILLBUFFID].asInt();
        gold=skilldata[SKILLGOLD].asInt();
        nextid=skilldata[SKILLNEXTID].asInt();
        moveeffect=skilldata[SKILLMOVEEFFECT].asInt();
        m_skillTimes=skilldata[SKILLTIMES].asFloat();
        m_skillRange=skilldata[SKILLRANGES].asInt();
        CCLog("%d",m_skillRange);
        attackeffect=skilldata[SKILLATTACKEFFECT].asInt();
        passiveeffect=skilldata[SKILLPASSIVEEFFECT].asInt();
        m_skillCounts=skilldata[SKILLCOUNTS].asInt();
        m_skillName=skilldata[SKILLNAME].asString().c_str();
    }
}
EnemySkillInfo::EnemySkillInfo(){
    
}
EnemySkillInfo::~EnemySkillInfo(){
    
}
EnemySkillInfo *EnemySkillInfo::create(int skillId){
    EnemySkillInfo* s_skill=new EnemySkillInfo();
    if (s_skill) {
        s_skill->initWithSkillId(skillId);
        return s_skill;
    }
    CC_SAFE_DELETE(s_skill);
    return NULL;
}
void EnemySkillInfo::initWithSkillId(int skillId){
    m_skillId=skillId;
    switch (skillId) {
        case 1:
            skillType=THROW;
            skillRange=ALL;
            AnimatePacker::getInstance()->loadAnimations("bombAndfire.xml");
            break;
        case 2:
            skillType=THROW;
            skillRange=ALL;
            AnimatePacker::getInstance()->loadAnimations("ice.xml");
            AnimatePacker::getInstance()->loadAnimations("locked.xml");
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        default:
            break;
    }
}
void EnemySkillInfo::releaseSkill(CCPoint selfpoint){
    if (m_skillId==1) {
        if (skillRange==ALL) {
            for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
                CCSprite *bome=CCSprite::create("bombp.png");
                bome->setTag(111+i);
                Person *attackrole=(Person *)GameData::shareGameData()->roleArr->objectAtIndex(i);
                bome->setPosition(selfpoint);
                float z=-(attackrole->getPosition().y-100)/5;
                CCLog("%f",z);
                this->addChild(bome,z);
                CCJumpTo *jump=CCJumpTo::create(0.5, ccp(attackrole->getPosition().x, attackrole->getPosition().y-100), 150, 1);
                CCCallFuncND *func=CCCallFuncND::create(this, callfuncND_selector(EnemySkillInfo::callbackreleaseSkill),attackrole);
                CCSequence *seq=CCSequence::create(jump,func,NULL);
                bome->runAction(seq);
            }
        }
    }else if(m_skillId==2){
        if (skillRange==ALL) {
            for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
                Person *attackrole=(Person *)GameData::shareGameData()->roleArr->objectAtIndex(i);
                CCSprite *lock=CCSprite::createWithSpriteFrameName("touch_mouse_01.png");
                lock->setTag(113+i);
                lock->setPosition(ccp(attackrole->getPosition().x, attackrole->getPosition().y-100));
                CCRepeat *rep=CCRepeat::create(AnimatePacker::getInstance()->getAnimate("loackAni"), 3);
                CCCallFuncND *funND=CCCallFuncND::create(this, callfuncND_selector(EnemySkillInfo::callbackreleaseSkill), attackrole);
                CCSequence *seq=CCSequence::create(rep,funND,NULL);
                float z=-lock->getPositionY()/5;
                this->addChild(lock,z);
                lock->runAction(seq);
            }
        }
    }
}
void EnemySkillInfo::callbackreleaseSkill(CCObject *obj,void *sender){
    

    if (m_skillId==1) {
        float z=-(((Person *)sender)->getPositionY()-100)/5;
        this->getChildByTag(111)->setZOrder(z);
        CCSprite *fire=CCSprite::createWithSpriteFrameName("fire_00.png");
        fire->setPosition(ccp(115, 115));
        CCRepeat *rep=CCRepeat::create(AnimatePacker::getInstance()->getAnimate("fireAni"), 3);
        CCCallFuncND *funND=CCCallFuncND::create(this, callfuncND_selector(EnemySkillInfo::callbackfirebomb), sender);
        CCSequence *seq=CCSequence::create(rep,funND,NULL);
        fire->runAction(seq);
        this->getChildByTag(111)->addChild(fire);
    }else if(m_skillId==2){
        CCSprite *ice=CCSprite::createWithSpriteFrameName("ice_00.png");
        ice->setTag(112);
        CCCallFunc *func=CCCallFunc::create(this, callfunc_selector(EnemySkillInfo::callbackremovebomb));
        CCSequence *seq=CCSequence::create(AnimatePacker::getInstance()->getAnimate("iceAni"),func,NULL);
        ice->runAction(seq);
        ice->setPosition(ccp(this->getChildByTag(113)->getPosition().x, this->getChildByTag(113)->getPosition().y));
        float z=-ice->getPositionY()/5;
        this->addChild(ice,z);
        Person *person=(Person *)sender;
        if (ccpDistance(person->getPosition(), ice->getPosition())<200) {
            person->changeBlood(-2000);
        }
        this->removeChildByTag(113,true);
    }
}
void EnemySkillInfo::callbackfirebomb(CCObject *obj,void *sender){
    if (m_skillId==1) {
        CCSprite *sp=(CCSprite *)this->getChildByTag(111);
        CCCallFunc *removebome=CCCallFunc::create(this, callfunc_selector(EnemySkillInfo::callbackremovebomb));
        CCSequence *seq=CCSequence::create(AnimatePacker::getInstance()->getAnimate("bombAni"),removebome,NULL);
        sp->runAction(seq);
        Person *person=(Person *)sender;
        if (ccpDistance(person->getPosition(), sp->getPosition())<200) {
            person->changeBlood(-2000);
        }
    }else if(m_skillId==2){
        
    }
}
void EnemySkillInfo::callbackremovebomb(){
    if (m_skillId==1) {
        this->removeChildByTag(111,true);
    }else if(m_skillId==2){
        this->removeChildByTag(112,true);
    }
}