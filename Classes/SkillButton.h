//
//  SkillButton.h
//  TestGame
//
//  Created by lh on 13-10-22.
//  www.9miao.com
//

#ifndef __TestGame__SkillButton__
#define __TestGame__SkillButton__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;
class SkillButton:public CCSprite{
public:
    

    SkillButton();
    ~SkillButton();
    CCSprite *spirte_block;
    bool TouchEnable;
    float _time;
    CCProgressTimer *progress_tiem_skill;
    CCProgressTo *action_progress_to;
    static SkillButton* create(float time);
    virtual void initWithDuration(float time);
    void SkillClickCallBack(CCObject *sender);
    void SKillCoolDownCallBack(CCNode *sender);
    bool touchSkillBtn(CCTouch *pTouch);
    
    
//    void skillHurt(float hurtPercent,int skillId,int standardAttack);
//    void screenEnemy();
//    void setType(int skillType);
//    void removeSelf(CCArmature * arm,MovementEventType type,const char * name);
//    void setSkillRegion(CCPoint centerPos);
  //  CREATE_FUNC(SkillButton);
};
#endif /* defined(__TestGame__SkillButton__) */
