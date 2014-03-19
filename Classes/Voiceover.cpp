//
//  Voiceover.cpp
//  TestGame
//
//  Created by lh on 13-9-22.
//  www.9miao.com
//

#include "Voiceover.h"
static int num=1;
bool Voiceover::init(){
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 255))) {
        return false;
    }
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    std::string str1="欢迎来到宗派！每个伙伴都有自己独特的元神技能！";
    std::string str2="你可以通过招募更多的伙伴来扩大团队！";
    description=  CCLabelTTF::create("", "AmericanTypewriter-Bold", 60, CCSizeMake(0,100), kCCTextAlignmentLeft);
    description->setAnchorPoint(ccp(0, 0));
    description->setPosition(ccp(300, size.height/2));
    description->setVisible(false);
    this->addChild(description);
    description2=CCLabelTTF::create("", "AmericanTypewriter-Bold", 60, CCSizeMake(0,100), kCCTextAlignmentLeft);
    description2->setAnchorPoint(ccp(0, 0));
    description2->setVisible(false);
    description2->setPosition(ccp(500, size.height/2-150));
    this->addChild(description2);
    description->initWithString(str1.c_str(), "AmericanTypewriter-Bold", 60, CCSizeMake(0,100), kCCTextAlignmentLeft);
    description->setAnchorPoint(ccp(0, 0));
    description2->initWithString(str2.c_str(), "AmericanTypewriter-Bold", 40, CCSizeMake(0,100), kCCTextAlignmentLeft);
    description2->setAnchorPoint(ccp(0, 0));
    this->schedule(schedule_selector(Voiceover::playDesc),0.1);
    return true;
}
void Voiceover::playDesc(){
    if(!description->isVisible()){
        description->setVisible(true);
    }
    
    if (description->getDimensions().width==1800) {
        this->unschedule(schedule_selector(Voiceover::playDesc));
        num=1;
        fadeout();
       // this->schedule(schedule_selector(Voiceover::playDesc2),0.1);
        
    }else{
        
        description->setDimensions(CCSizeMake(180*num, 100));
        
        num++;
    }
    
    
}
void Voiceover::playDesc2(){
    if (!description2->isVisible()) {
        description2->setVisible(true);
    }
    if (description2->getDimensions().width==1800) {
        this->unschedule(schedule_selector(Voiceover::playDesc2));
        fadeout();
        num=1;
    }else{
        description2->setDimensions(CCSizeMake(180*num, 100));
       
        num++;
    }
}
void Voiceover::fadeout(){
    description2->removeFromParent();
    description->removeFromParent();
    CCFadeOut * fadeout=CCFadeOut::create(1);
    CCCallFunc * func= CCCallFunc::create(this, callfunc_selector(Voiceover::removeSelf));
    CCSequence * seq= CCSequence::create(fadeout,func,NULL);
    
    this->runAction(seq);
}
void Voiceover::removeSelf(){
    CCNotificationCenter::sharedNotificationCenter()->postNotification("runBattle");
    this->removeFromParentAndCleanup(true);
}