//
//  PersonTemplate.h
//  TestGame
//
//  Created by lh on 13-11-26.
//  www.9miao.com
//

#ifndef __TestGame__PersonTemplate__
#define __TestGame__PersonTemplate__

#include <iostream>
#include "HeaderBase.h"
#include "SkillButton.h"
#include "SkillInfo.h"

typedef enum {
AVOID=1,
STRIKE,
PARRY,
NORMAL,
STRIKEANDPARRY
}Attackmodel;
typedef enum{
    RUN=1,
    STAND,
    ATTACK,
    ATTACKED,
    DEAD,
    TREATED
}PersonMotion;
typedef enum{
    BLOODBG=1000,
    BLOOD,
    DAOGUANG,
    ATTACKACTION,
    TOUCHMOVEACTION,
    TOUCHATTACKACTION,
    ATTACKCALLBACKFUNCACTION,
    CUREBLOODACTION
    
}PersonTag;


class Person:public CCSprite{
public:
    virtual bool init();
    CCSize size;
    int aniDir;
    bool lockFlag;
    SkillsInfo *skill;
   // CCArmature* m_role;//骨骼动画
    CCSprite *m_role;
    CCLabelTTF *reduceBlood;
    bool isDead;
    //初始化静态属性
    void setStaticProperties(int basehp,int baseattack,float defense_coff,float dodge_coff,
                             float tenacity_coff,float parry_coff,float deflect_coff,
                             float strike_coff,float strike_result_coff,float hitrate_coff);
    //初始化动态属性
    void setDynamicProperties(int mAttack,int mHp,int mDefense,int mHitRate,int mDodge,
                              int mParry,int mStrike,int mStrikeResult);
    //获得攻击结果
    float getAttackResult();
    void reduceBloodCallBack(CCObject *sender);
    virtual void loadPersonTexture();
    
    //角色死亡
    
    bool m_skillTouch;
  virtual  void roleDead();
    void attacked();
    void attackedCallBack(CCObject *sender);
    Attackmodel attackModel();
    void animateCallBack(CCObject *sender);
    float bloodResult(Attackmodel model);
    bool  isAttacking;
    void initBones();
    void setZ();
    virtual void motionType(PersonMotion per,int dir,const char* armname);
    int hostility;//仇恨
    //人物属性（动态属性）
    int type;
    int res_icon;
    int res_resource;
    int m_btype;
    int m_level;//等级
    int m_id;// id
    int m_petid;//模板id
    std::string m_name;//名字
    int skillId;
    bool isTouch;
    int m_attack;//攻击
    int m_hp;//血量
    int cur_hp;
    int m_defense;//防御
    //隐藏属性
    int m_hittarget;//命中
    int m_tenacity;//韧性
    int m_dodge;//闪避
    int m_parry;//格挡
    int m_strike;//暴击
    int m_strike_result;//暴击威力
    //其他属性
    int m_range;//攻击范围
    int m_speed;//移动速度
    int m_attack_speed;//攻击速度
    //固定属性
    int base_hp;//标准生命
    int base_attack;//标准攻击
    int base_strike;//标准暴击值
    float defense_cf;//防御系数
    float dodge_cf;//闪避系数
    float tenacity_cf;//韧性系数
    float parry_cf;//格挡系数
    float deflect_cf;//偏斜系数
    float strike_cf;//暴击率系数
    float strike_result_cf;//暴击威力系数
    float hitrate_cf;//命中系数
    
    float damage_rate;//伤害率
    float avoid_rate;//躲避率
    float parry_rate;//格挡率
    float strike_rate;//暴击率
    
    int m_skillTouchs;
    int m_maxTouchs;
    float DamageRate();
    float avoidRate();
    float parryRate();
    float strikeRate();
    void changeBloodCallBack();
    
    float DamageRateMultiple();
    float DamageResultMultiple();
    float HitRateMultiple();
    Person* target;
   // void getHurt();
   virtual void attackCallBack();
    void addBlood();
    void changeBlood(int changeBlood);
    void onExit();
    ~Person();
    
    CREATE_FUNC(Person);
};


class EnemyRole:public Person{
public:
    
    virtual bool init();
    ~EnemyRole();
    int oldtouchnum;
    bool ismoveing;
    virtual void enterAttackRegion();
    virtual void roleDead();
    virtual void loadPersonTexture();
    virtual void traceTarge();
    virtual void judgeSelfPos();
    virtual void runCallBack(CCObject *sender,void *num);
    void callFadeInOutFunc();
    void changeDir();
    int touchNum;
    CCSprite *select_red;
    bool isRunning;
    bool beatBackFlag;
    Person *attackMeTarget;
    CCNode *g_pNodePinBall;
    int judgePos(CCPoint m_pos,CCPoint target_pos);
    void findTarget();
    void PinballHitEnemy(CCNode *pSender);
    void PinballDis();
    bool g_bPinBallFlag;
    void MoveToTarget();
    void checkTarget();
    void onExit();
    void start();
    void setZs();
    void targetMove(CCObject *sender);
    void releaseSkillControl();
    void beatBack();
    void getHostility();
    int releaseTime;
    
    void callBackStand(CCObject * sender,void * num);
    bool isInSprite(CCTouch *touch);
    virtual void attackCallBack();
    CREATE_FUNC(EnemyRole);
};


class FriendlyRole:public Person{
    
public:
    virtual bool init();
    void start();
    CCLabelTTF *label;
    SkillButton *skillbtns;
    void moveToSelectTarget();
   // bool touchSkillBtn(CCTouch *pTouch);
    int enterNum;
    bool beatBackFlag;
    CCSprite *selectedCircle;
    virtual void loadPersonTexture();
    void enterAttackOrgin();
    void enterAttackRegion();
    void findTarget();
    void TargetDead(CCObject *sender);
    void attackStand();
    bool isRunning;
    virtual void attackCallBack();
    void traceTarge();
    void beatBack();
    void onExit();
    void judgePos();
    void setZs();
    ~FriendlyRole();
    void touchEvent(CCTouch *touch,bool touch_flag);
    bool isInSprite(CCTouch *touch);
    void callBackStand(CCNode *node,void * sender);
    void callBackAttack(CCNode *node,void * sender);
    CREATE_FUNC(FriendlyRole);
};

class MasterRole:public Person{
public:
    virtual bool init();
    virtual void attackCallBack();
    ~MasterRole();
    attackInfo *info0;
    void start();
    void sendAttackJson();
    void receiveAttackJson();
    void touchSchedul();
    void addSkill();
    bool m_endTouchFlag;
    CCTouch *p_endTouch;
    bool m_strongTouch;
    CCTouch *touch_pos;
    bool m_touch_flag;
    SkillButton *skillbtns;
    bool m_nSkillReleaseFlag;
    int dir;
    
    bool isRunning;
    bool isMoving;
    CCParticleSystemQuad *attackPtc;
    CCParticleSystemQuad *fireBall;
    virtual void loadPersonTexture();
    void setZs();
    void enterAttackRegion();
    void judgePos();

    void enemyDead(CCObject *sender);
    void moveFireball();
   // bool touchSkillBtn(CCTouch *pTouch);
    void targetExplode();
    void whetherExplodeOver();
    void releaseSkill(CCTouch *pTouch);
    
    void touchEvent(CCTouch *touch,bool touch_flag);
    bool isInSprite(CCTouch *touch);
    void callBackStand(CCNode *node,void * sender);
    void callBackAttack(CCNode *node,void * sender);
    CREATE_FUNC(MasterRole);
};



class CureRole:public Person{
public:
    ~CureRole();
    SkillButton *skillbtns;
    virtual bool init();
    void start();
    int dir;
    Person *oldperson;
    bool isCure;
    void cureTarget();
    bool skilltouchFlag;
    void moreCureTarget(CCArray *rolearr);
    void setZs();
    void cureBlood(int changeBlood);
    void cureCallBack();
    void judgePos();
    bool isInSprite(CCTouch *touch);
    void callBackStand(CCObject *sender,void *num);
    void touchEvent(CCTouch *touch,bool touch_flag);
    virtual void loadPersonTexture();
    virtual void attackCallBack();
    CREATE_FUNC(CureRole);
};
class EnemyLongRangeBoss:public EnemyRole{
public:
    virtual bool init();
    virtual void loadPersonTexture();
    virtual void enterAttackRegion();
    virtual void traceTarge();
    virtual void judgeSelfPos();
    virtual void runCallBack(CCObject *sender,void *num);
    
    CREATE_FUNC(EnemyLongRangeBoss);
};
#endif /* defined(__TestGame__PersonTemplate__) */
