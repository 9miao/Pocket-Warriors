//
//  GameData.h
//  TestGame
//
//  Created by lh on 13-9-12.
//  www.9miao.com
//

#ifndef __TestGame__GameData__
#define __TestGame__GameData__

#include <iostream>
#include "cocos2d.h"
//#include "allPetInfo1104.pb.h"
//#include "getPet1103.pb.h"
//#include "instanceInfo1201.pb.h"
#include "SocketClient.h"
#include <time.h>
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;
class GameData{
public:
    
    GameData();
    ~GameData();
    int onScreenCount;
    int pet_id;
    bool SkillBtnIsTouch;
    static GameData * shareGameData();
    int getRandom();
    CCPoint getTouchPoint();
    void setTouchPoint(CCPoint pos);
    bool fire_Flag;
    std::string welfare_Str;
    //宠物集合
//    ::google::protobuf::RepeatedPtrField< ::allPetInfo1101::PetFight > pfinfo_;
//    void setPetFightArr(::google::protobuf::RepeatedPtrField< ::allPetInfo1101::PetFight > pfinfo);
//    inline const ::google::protobuf::RepeatedPtrField< ::allPetInfo1101::PetFight >&
//    pfinfo() const;
//    inline const ::allPetInfo1101::PetFight& pfinfo(int index) const;
//    //副本集合
//    ::google::protobuf::RepeatedPtrField< ::instanceInfo1201::Group > grouplist_;
//    inline const ::google::protobuf::RepeatedPtrField< ::instanceInfo1201::Group >&
//    grouplist() const;
//    inline const ::instanceInfo1201::Group& grouplist(int index) const;
//    void setGroupList(::google::protobuf::RepeatedPtrField< ::instanceInfo1201::Group > grouplist);
//    //怪物集合
//    ::google::protobuf::RepeatedPtrField< ::MonsterInfo > mlist_;
//    inline const ::MonsterInfo& mlist(int index) const;
//    inline const ::google::protobuf::RepeatedPtrField< ::MonsterInfo >&
//    mlist() const;
//    void setMonsterGroup(::google::protobuf::RepeatedPtrField< ::MonsterInfo > mlist);
//    ::google::protobuf::RepeatedPtrField<::protofile::allPetInfo1101::PetShow > pfshow_;
//    
//    void setPetShowArr(::google::protobuf::RepeatedPtrField<::protofile::allPetInfo1101::PetShow > fshow);
    std::string str;
    time_t oldtime;
    time_t newtime;
    int gold;
    int coin;
    bool eneMoveFlag;
    SocketClient *socket;
    int chaId;
    CCDictionary *roleDic;
    CCDictionary *eneDic;
    CCArray *roleArr;
    CCArray *posArr;
    CCArray *enemyArr;
    CCDictionary * msg_dic;
    CCPoint point;
};
class attackedInfo:public CCObject{
public:
    attackedInfo();
    ~attackedInfo();
public:
    int attacked_id;        //被攻击者id
    int attacked_curblood;//被攻击者当前血量
    int attacked_reduceblood;//被攻击者掉血量

};
class attackInfo{
public:
    attackInfo();
    ~attackInfo();
    int charid;//角色id
    int attack_id;//第几波怪
    int countofAttack;//攻击者id
    int skillId;//攻击者技能ID；
    std::string attack_time;//发动攻击的时间
public:
    CCArray *attackeds;
    
};
#endif /* defined(__TestGame__GameData__) */
