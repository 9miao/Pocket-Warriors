//
//  SkillInfo.h
//  TestGame
//
//  Created by lh on 13-12-26.
//  www.9miao.com
//

#ifndef __TestGame__SkillInfo__
#define __TestGame__SkillInfo__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
typedef enum{
ERROR=0,
REGIONALSKILL,
CUTSKILL,
CURESKILL,
REGIONALTREATED
}SkillName;
class SkillsInfo:public CCSprite{
public:
    SkillsInfo();
    ~SkillsInfo();
    static SkillsInfo *create(int skillId);
    void initWithSkillId(int skillId);
    int m_skillRange;//每一次触发技能后 碰触墙壁次数 半径 伤害次数
    void regionalSKill();

    const char *m_skillName;//技能名称
    int m_skillTypeId;//技能类型 1，射击 2，弹弹球 3，圆形范围 4，切水果
    const char *m_skillScript;//伤害计算公式
    int m_skillCounts;//每次允许发射次数，x表示x次技能之后技能冷却
    
    float m_skillTimes;// 技能冷却时间
    int m_skillBuffid;//出发的buffid  0为没有buff
    int m_skillCri;//暴击概率 50表示出发概率50%
    double m_skillCrp;//暴击威力
    int nextid;//本技能升级后的技能id
    int gold;//技能升级所需要的金币
    int attackeffect;//技能释放特效
    int moveeffect;// 技能移动特效
    int passiveeffect;//受击特效
    int _skillId;
private:
    
};
typedef enum {
THROW=1,//投掷
EARTHBULGE,//地面突起
CHARGE,//冲锋
IMPRISONMENT,//禁锢
RANGE,//范围
RUSH,//冲击波
SUMMON,
CONTINUOUSATTACK
}ENEMYSKILLTYPE;
typedef enum
{
SRANDONE=1,
ALL,
CIRCLESRANGE,
SECTORRANGE,
RELEASECOUNTS
}RANGES;
class EnemySkillInfo:public CCLayer{
public:
    EnemySkillInfo();
    ~EnemySkillInfo();
    ENEMYSKILLTYPE skillType;
    RANGES skillRange;
    int m_skillId;
    void initWithSkillId(int skillId);
    void releaseSkill(CCPoint selfpoint);
    void callbackreleaseSkill(CCObject *obj,void * sender);
    void callbackfirebomb(CCObject *obj,void *sender);
    void callbackremovebomb();
    static EnemySkillInfo *create(int skillId);
};
#endif /* defined(__TestGame__SkillInfo__) */
