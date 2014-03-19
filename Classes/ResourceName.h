//
//  ResourceName.h
//  TestGame
//
//  Created by lh on 13-12-30.
//  www.9miao.com
//

#ifndef TestGame_ResourceName_h
#define TestGame_ResourceName_h
/* Resource Name*/
//ip port
#define ip "172.16.18.25"
#define ports  11009
//command Id
#define CMID_LOGIN 1001                  //玩家登陆
#define CMID_REGISTER 1002               //玩家注册
#define CMID_TEAMINFO 1103               //获取团队界面信息
#define CMID_GETPETINFO 1104             //获取出战宠物详细信息
#define CMID_GETPERMITENTERBATTLE 1202   //角色请求进入副本
#define CMID_EXITBATTLE 1204             //关闭、退出副本
#define CMID_SENDBATTLESTRIKEINFO 1203   //角色在副本中战斗数值合法验证
#define CMID_GETAVAILABLEFB 1201         //获取主线可征战副本id
#define CMID_GETSTRENGTHENINFO 1401      //获取强化界面信息
#define CMID_STRENGTHPET 1402            //强化宠物
#define CMID_WELFARE 1501                //福利界面
#define CMID_GETWELFARE 1520             //领取福利
//enemy range
#define ENEMYATTACK_RANGE 200
#define TRACETARGET_RANGE 170
#define NEARATTACK_RANGE 170
#define ENEMY_SPEED 200.0f
//near Attack (friendlyRole)
#define NEAR_ROLEATTACKRANGE 200
#define NEAR_ROLESPEED 200.0f
#define NEAR_MINATTACKRANGE 120
#define NEAR_MAXATTACKRANGE 150

//fontcolor
#define fontColor ccc3(255, 203, 141)
//loadinglayer

#define M_LOADING_BG "loadingBg.png"
#define M_PROGRESS_BG "loadingProgress_Bg.png"
#define M_PROGRESS "loadingProgress.png"
//registerlayer
#define M_REGISTER_BG "register_bg.png"
#define M_REGISTER_BTN_UP "reg_cj_up.png"
#define M_REGISTER_BTN_DOWN "reg_cj_down.png"
#define M_REGISTER_BOUNDER "register_bounder.png"
//loginview
#define M_LOGIN_BG "login_bg.png"
#define M_LOGIN_BTN_UP "denglu_up.png"
#define M_LOGIN_BTN_DOWN "denglu_down.png"
#define M_CHUANGJIAN_UP "chuangjian_up.png"
#define M_CHUANGJIAN_DOWN "chuangjian_down.png"
//mainview
#define M_MAINVIEW_BG "background.png"
#define M_MVDRAG_BG "drag_bg.png"
#define M_KZBTN_UP "kz_btn_up.png"
#define M_KZBTN_DOWN "kz_btn_down.png"
#define M_TDBTN_UP "td_btn_up.png"
#define M_TDBTN_DOWN "td_btn_down.png"
#define M_QHBTN_UP "qh_btn_up.png"
#define M_QHBTN_DOWN "qh_btn_down.png"
#define M_FLBTN_UP "fl_btn_up.png"
#define M_FLBTN_DOWN "fl_btn_down.png"
//selectFB
#define M_SELECTFB_MONSTERINFO "monster.json"
#define M_SELECTFB_MONSTER "instance.json"
#define M_SELECTFB_BGJSON "FBLayer/selectFB_1.json"
//persontemplate
#define M_PERSONBLOOD_BG "person_blood_bg.png"
#define M_PERSON_BLOOD "person_blood.png"
#define M_HQ_TX_XML "huoqiu1.xml"
#define M_HQ_TX_FPNG "hqg_00.png"
#define M_HQ_TX_DATA "huoqiu"
//friendRole
#define ARMATURE_ACTION_DATA0 "fsla"//fuzinv
#define ARMATURE_ACTION_DATA1 "fsla"//fuzinvrun
//curerole
#define ARMATURE_ACTION_DATA2 "fsla"//fuzinv
#define ARMATURE_ACTION_DATA3 "fsla"//fuzinvrun
#define M_ZL_TX_XML "zhiliao.xml"
#define M_ZL_TX_FPNG "zhiliao_l_00001.png"
#define M_ZL_TX_DATA "zhiliaoani"
//enemy
#define ARMATURE_ACTION_DATA4 "ctb"//daozhang
#define ARMATURE_ACTION_DATA5 "ctb"//daozhangrun
//master
#define ARMATURE_ACTION_DATA6 "fcfa"//shanzinan
#define ARMATURE_ACTION_DATA7 "fcfa"//shanzinanrun
#define PARTICLE_EXPLODE_PLIST "exp2.plist"
#define PARTICLE_DRAWPARTICLE_PLIST "Particle.plist"
#define PARTICLE_FIREBALL_PLIST "fireball01.plist"
//battlelyaer
#define M_BATTLE_BG "background01.png"
#define LINE "line.png"
#define DIRECTION "dir.png"
#define CIRCLES "fz_cirle.png"
#define T_DH_TX_XML "dihuo.xml"
#define T_DH_TX_FPNG "dihuo_00000.png"
#define T_DH_TX_DATA "dihuoAni"
#define T_DG_TX_XML "daoguang.xml"
#define BATTLE_PARTICLE "Particle.plist"
//ttf
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#define SELF_TTF_ONE "--unknown-1--.TTF"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SELF_TTF_ONE "fonts/font1.ttf"
#endif

#define SELF_TTF_TWO "黑体"


//skill list
#define SKILLJSONNAME "skill.json"
#define SKILLNAME "kname"
#define SKILLTYPE "typeid"
#define SKILLSCRIPT "script"
#define SKILLCOUNTS "counts"
#define SKILLRANGES "ranges"
#define SKILLTIMES "times"
#define SKILLBUFFID "buffid"
#define SKILLCRI "cri"
#define SKILLCRP "crp"
#define SKILLNEXTID "nextid"
#define SKILLGOLD "gold"
#define SKILLATTACKEFFECT "attackeffect"
#define SKILLMOVEEFFECT "moveeffect"
#define SKILLPASSIVEEFFECT "passiveeffect"
//messagebox
#define MESSAGEBOXBG "messagebox.png"
#define SURE_UP "sure_up.png"
#define SURE_DOWN "sure_down.png"
//welfarelayer
#define WELFLAYER_BG "welfare_Bg.png"
#define RECEIVE_BTN_UP "receive_up.png"
#define RECEIVE_BTN_DOWN "receive_down.png"
#define RECEIVE_BTN_DIS "receive_dis.png"
#define CELL_DIR "cell_dir.png"
#define WELFCELL_BG "welfarecell_Bg.png"

#endif
