//
//  BattleLayer.cpp
//  TestGame
//
//  Created by lh on 13-11-4.
//  www.9miao.com
//

#include "BattleLayer.h"
#include "message.h"
#include "GeometricRecognizer.h"
#include "CheckPos.h"
#include "LoadRes.h"
#include <time.h>
#include "AnimatePacker.h"
#include "MainView.h"

//#include "MasterRole.h"
#define pi 3.1415926
static bool FriendRoleSkillTouch=false;

using namespace DollarRecognizer;
bool BattleLayer::init(){
    bool bRet = false;
    do {
        time_t t=time(NULL);
        gameover=NULL;
        selectedPerson=NULL;
        GameData::shareGameData()->oldtime=t;
        CCSize size= CCDirector::sharedDirector()->getWinSize();
        setTouchEnabled(true);
        CCSprite *attack_bg=CCSprite::create(M_BATTLE_BG);//背景图
        attack_bg->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(attack_bg,-1080);
        touch_arr= CCArray::create();
        touch_arr->retain();
        touch_dic= CCDictionary::create();
        touch_dic->retain();
        appearArr= CCArray::create();
        appearArr->retain();
        bRet = true ;
    }while (0);
    return bRet ;
}
void BattleLayer::onEnterTransitionDidFinish(){
    CCLayer::onEnter();
}
void BattleLayer::onEnter(){
    AnimatePacker::getInstance()->loadAnimations("wenzi.xml");
    AnimatePacker::getInstance()->loadAnimations(T_DH_TX_XML);
    AnimatePacker::getInstance()->loadAnimations(M_ZL_TX_XML);
    AnimatePacker::getInstance()->loadAnimations(T_DG_TX_XML);
    initParticle();
    setRole();
    monsterVisible();
}
void BattleLayer::monsterVisible(){
    appearArr->removeAllObjects();
    LoadRes::shareLoadRes()->loadBossTexture();
    if (GameData::shareGameData()->enemyArr->count()>=GameData::shareGameData()->onScreenCount) {
        for (int i=0;i<GameData::shareGameData()->onScreenCount;i++) {
            EnemyRole *enemy= (EnemyRole *)GameData::shareGameData()->enemyArr->objectAtIndex(i);
            enemy->start();
            enemy->skillId=2;
            setEnemyType(enemy);
            enemy->releaseTime=5;
        }
    }else if(GameData::shareGameData()->enemyArr->count()<GameData::shareGameData()->onScreenCount&&GameData::shareGameData()->enemyArr->count()>0){
        for (int i=0; i<GameData::shareGameData()->enemyArr->count(); i++) {
            EnemyRole *enemy= (EnemyRole *)GameData::shareGameData()->enemyArr->objectAtIndex(i);
            enemy->motionType(STAND, enemy->touchNum,ARMATURE_ACTION_DATA4);
            //enemy->setPosition(CheckPos::shareCheckPos()->CallBackPos(0));
            enemy->target=NULL;
            enemy->start();
            enemy->skillId=1;
            enemy->releaseTime=10;
            this->addChild(enemy,1);
            enemy->findTarget();
            appearArr->addObject(enemy);
        }
        
    }else{
        if (gameover==NULL) {
            this->stopAllActions();
            this->removeAllChildrenWithCleanup(true);
            this->unscheduleAllSelectors();
            setTouchEnabled(false);
            gameover=GameOver::create();
            this->addChild(gameover,99);
        }else{
            return;
        }
    }
}
void BattleLayer::setEnemyType(EnemyRole *enemy){
    enemy->motionType(STAND, enemy->touchNum,ARMATURE_ACTION_DATA4);
    CheckPos::shareCheckPos()->CallBackPos(0);
    enemy->setPosition(CheckPos::shareCheckPos()->CallBackPos(0));
    this->addChild(enemy,3);
    enemy->target=NULL;
    enemy->findTarget();
    appearArr->addObject(enemy);
}
void BattleLayer::setRole(){
    for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
        char num[20]="";
        Person *pPerson=(Person *)GameData::shareGameData()->roleArr->objectAtIndex(i);
        sprintf(num, "%d",i);
        if (pPerson->m_btype==2) {
            LoadRes::shareLoadRes()->loadEnemyPersonTexture();
            MasterRole* frsp=(MasterRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
            frsp->start();
            frsp->motionType(STAND, frsp->aniDir, ARMATURE_ACTION_DATA6);
            frsp->hostility=50;
            frsp->setPosition(ccp(300+300*i, 400));
            this->addChild(frsp,3);
        }else if(pPerson->m_btype==1){
            LoadRes::shareLoadRes()->loadRolePersonTexture02();
            FriendlyRole *frsp=(FriendlyRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
            frsp->start();
            frsp->motionType(STAND, frsp->aniDir, ARMATURE_ACTION_DATA0);
            frsp->hostility=100;
            frsp->setPosition(ccp(300+300*i, 400));
            this->addChild(frsp,-frsp->getPositionY()/5);
        }
        else if(pPerson->m_btype==3){
            LoadRes::shareLoadRes()->loadRolePersonTexture02();
            CureRole* frsp=(CureRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
            frsp->start();
            frsp->motionType(STAND, frsp->aniDir, ARMATURE_ACTION_DATA2);
            frsp->hostility=0;
            frsp->setPosition(ccp(300+300*i, 400));
            this->addChild(frsp,3);
        }
    }
}
void BattleLayer::initParticle(){
    geo=new GeometricRecognizer();
    geo->loadTemplates();
    CCGLProgram *glShaderProgram = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor);
    this->setShaderProgram(glShaderProgram);
    m_pParticle = CCParticleSystemQuad::create(BATTLE_PARTICLE);
    this->addChild(m_pParticle, 10) ;
    m_pParticle->setVisible(false) ;
    memset(m_pointPath, 0, sizeof(m_pointPath)) ;
    m_timer = 0 ;
    m_touchBeganPoint = m_touchEndPoint = ccp(0, 0) ;
    
}

void BattleLayer::line()
{
    if (m_Index<3 || m_Index > 1024) {
		return;
	}
	CCPoint *p = m_pointPath ;
	//CCPoint vertexMiddle[1024]={ccp(0.0, 0.0)};
    CCPoint*vertexMiddle = new CCPoint[m_Index] ;
	//memset(vertexMiddle, 0, sizeof(vertexMiddle)) ;
	memcpy(vertexMiddle, p, sizeof(CCPoint)*m_Index);
	
	CCPoint pt = ccpSub(p[m_Index-1], p[m_Index-2]);
	GLfloat angle = ccpToAngle(pt);
	vertexMiddle[m_Index-1].x += cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	vertexMiddle[m_Index-1].y += sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    
	CCPoint vertexTop[1024]={ccp(0.0, 0.0)};
	
	vertexTop[0].x = p[m_Index-1].x + cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	vertexTop[0].y = p[m_Index-1].y + sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	
	GLint count1 = 1;
	
	for (int i = (m_Index-2); i>0; --i) {
		float w = i*0.5f ;
		if (w < 3)w = 3 ;
		else if(w > 8)w = 8 ;
		triangle(&(vertexTop[count1]),p[i],p[i-1], w);
		count1++;
	}
	vertexTop[count1++] = p[0];
    
	CCPoint vertexBottom[1024]={ccp(0.0, 0.0)};
	
	vertexBottom[0].x = p[m_Index-1].x + cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	vertexBottom[0].y = p[m_Index-1].y + sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	
	GLint count2 = 1;
	
	for (int i = (m_Index-2); i>0; --i) {
		float w = i*0.5f ;
		if (w < 3)w = 3 ;
		else if(w > 8)w =8 ;
		triangle(&(vertexBottom[count2]),p[i],p[i-1], -w);
		count2++;
	}
	vertexBottom[count2++] = p[0];
	
	CCPoint vertexTriangle[512] ={ccp(0.0, 0.0)} ;
	CCPoint*vertexCpy = vertexTriangle;
	GLubyte lineColors[1024] = {0};
	GLubyte*lineCpy = lineColors ;
	//1
	*vertexCpy = vertexMiddle[m_Index-1] ;
	*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
	vertexCpy++ ;
	
	*vertexCpy = vertexMiddle[m_Index-2] ;
	*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
	vertexCpy++ ;
	
	*vertexCpy = vertexTop[1] ;
	*lineCpy = TOP_POINT_R; lineCpy++ ;
	*lineCpy = TOP_POINT_G; lineCpy++ ;
	*lineCpy = TOP_POINT_B; lineCpy++ ;
	*lineCpy = TOP_POINT_A; lineCpy++ ;
	vertexCpy++ ;
	
	*vertexCpy = vertexMiddle[m_Index-1] ;
	*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
	vertexCpy++ ;
	*vertexCpy = vertexBottom[1] ;
	*lineCpy = BOTTOM_POINT_R; lineCpy++ ;
	*lineCpy = BOTTOM_POINT_G; lineCpy++ ;
	*lineCpy = BOTTOM_POINT_B; lineCpy++ ;
	*lineCpy = BOTTOM_POINT_A; lineCpy++ ;
	vertexCpy++ ;
	*vertexCpy = vertexMiddle[m_Index-2] ;
	*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
	*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
	vertexCpy++ ;
	
	for (int i = 0; i < m_Index-2; i++) {
		//上半个四边形
		*vertexCpy = vertexMiddle[m_Index-(i+2)] ;
		*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+3)] ;
		*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexTop[i+1] ;
		*lineCpy = TOP_POINT_R; lineCpy++ ;
		*lineCpy = TOP_POINT_G; lineCpy++ ;
		*lineCpy = TOP_POINT_B; lineCpy++ ;
		*lineCpy = TOP_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		
		*vertexCpy = vertexTop[i+1] ;
		*lineCpy = TOP_POINT_R; lineCpy++ ;
		*lineCpy = TOP_POINT_G; lineCpy++ ;
		*lineCpy = TOP_POINT_B; lineCpy++ ;
		*lineCpy = TOP_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+3)] ;
		*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexTop[i+2] ;
		*lineCpy = TOP_POINT_R; lineCpy++ ;
		*lineCpy = TOP_POINT_G; lineCpy++ ;
		*lineCpy = TOP_POINT_B; lineCpy++ ;
		*lineCpy = TOP_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		
		//下半个四边形
		*vertexCpy = vertexBottom[i+1] ;
		*lineCpy = BOTTOM_POINT_R; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_G; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_B; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexBottom[i+2] ;
		*lineCpy = BOTTOM_POINT_R; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_G; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_B; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+2)] ;
		*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		
		*vertexCpy = vertexMiddle[m_Index-(i+2)] ;
		*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexBottom[i+2] ;
		*lineCpy = BOTTOM_POINT_R; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_G; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_B; lineCpy++ ;
		*lineCpy = BOTTOM_POINT_A; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+3)] ;
		*lineCpy = MIDDLE_POINT_R; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_G; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_B; lineCpy++ ;
		*lineCpy = MIDDLE_POINT_A; lineCpy++ ;
		vertexCpy++ ;
	}
    glDisable(GL_TEXTURE_2D);

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );
    this->getShaderProgram()->use();
    this->getShaderProgram()->setUniformsForBuiltins();
    
    ccVertex2F vetexes[512]={0};
    for (int i=0; i<512; i++) {
        vetexes[i].x=vertexTriangle[i].x;
        vetexes[i].y=vertexTriangle[i].y;
    }
    // Pass the verticies to draw to OpenGL
    glEnableVertexAttribArray(kCCVertexAttribFlag_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_TRUE, 0,vetexes);
    
    // Pass the colors of the vertices to draw to OpenGL
    glEnableVertexAttribArray(kCCVertexAttribFlag_Color);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, lineColors);
    
    GLint numberTo=(GLint)((vertexCpy-vertexTriangle-1));
    glDrawArrays(GL_TRIANGLES, 0,numberTo );
    
    glEnable(GL_TEXTURE_2D);
    delete [] vertexMiddle ;
}
void BattleLayer::draw(){

    
    
    
    if (FriendRoleSkillTouch) {
        long dt = time(NULL) ;
        
        if ( m_timer > 10 )
        {
            m_timer = 0 ;
            erasureNail() ;
        }
        else {
            m_timer += dt ;
        }
        line() ;
    }
    
}
void BattleLayer::erasureNail()
{
    if ( m_Index < 3 || m_Index > 1024)
        return ;
    CCPoint*p = m_pointPath ;
    m_Index-- ;
    memmove(p, &(p[1]), sizeof(CCPoint)*(m_Index)) ;
}

void BattleLayer::triangle(CCPoint* vertex, CCPoint p1, CCPoint p2, GLfloat w)
{
    CCPoint pt = ccpSub(p1, p2);
	GLfloat angle = ccpToAngle(pt);
	
	GLfloat x = sinf(angle) * w;
	GLfloat y = cosf(angle) * w;
	vertex->x = p1.x+x;
	vertex->y = p1.y-y;
}
void BattleLayer::registerWithTouchDispatcher(){
     CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
void BattleLayer::ccTouchesBegan(cocos2d::CCSet *set, cocos2d::CCEvent *event){
    
    CCSetIterator  sel= set->begin();
    for (; sel!=set->end(); sel++) {
        CCTouch *pTouch=(CCTouch *)(* sel);
        CCPoint pos= pTouch->getLocation();
        path.clear();
        path.push_back(Point2D(pos.x,pos.y));
        //画线
        for (int i= 0; i<GameData::shareGameData()->roleArr->count(); i++) {

            Person *obj=(Person *)GameData::shareGameData()->roleArr->objectAtIndex(i);
            
            if (obj->m_btype==1&&!obj->m_skillTouch) {
                
                FriendlyRole *touch_Firend=(FriendlyRole *)obj;
                
                m_personBeginPoint=pTouch->getLocation();
                touch_Firend->m_skillTouch=touch_Firend->skillbtns->touchSkillBtn(pTouch);
                FriendRoleSkillTouch=touch_Firend->m_skillTouch;
                if (touch_Firend->isInSprite(pTouch)&&!touch_Firend->isTouch) {
                    touch_Firend->isTouch=true;
                    
                    if (selectedPerson!=touch_Firend) {
                        if (selectedPerson==NULL) {
                            selectedPerson=touch_Firend;
                        }
                        selectedPerson->removeChildByTag(9119);
                        selectedPerson=touch_Firend;
                        CCSprite *circle=CCSprite::create("select.png");
                        circle->setScale(0.8);
                        circle->setTag(9119);
                        circle->setPosition(ccp(touch_Firend->getContentSize().width/2, touch_Firend->getContentSize().height/2-30));
                        circle->runAction(CCScaleTo::create(0.1, 1));
                        touch_Firend->addChild(circle,-1);

                    }else{
                        
                    }
                    CCRect rect = CCRectMake(0,0,10,30);
                    CCRect rectInsets = CCRectMake(1,1,1,1);
                    CCSize winRect = CCSizeMake(10,30);
                    touch_line=CCScale9Sprite::create(LINE, rect, rectInsets);
                    touch_line->setTag(9200);
                    touch_line->setPosition(ccp(0, -20));
                    touch_line->setContentSize(winRect);
                    touch_line->setAnchorPoint(ccp(0, 0.5));

                    touch_line->setVisible(false);
                    touch_Firend ->addChild(touch_line,-1);
                    
                    CCSprite *touch_dir=CCSprite::create(DIRECTION);
                    touch_dir->setPosition(ccp(pos.x, pos.y));
                    touch_dir->setAnchorPoint(ccp(0.5, 0.5));
                    touch_dir->setTag(touch_Firend->m_id+3000);
                    touch_dir->setVisible(false);
                    touch_line->setVisible(false);
                    this->addChild(touch_dir,-1);
                    
                    touch_dic->setObject(touch_Firend, pTouch->getID());
                    touchNum.push_back(pTouch->getID());
                    return;
                    
                }else{
                    if (!m_bTouched&&touch_Firend->m_skillTouch) {
                        
                        CCLog("%d",touch_Firend->skillId);
                        skillLayerBegan(touch_Firend->skillId, touch_Firend, pTouch);
                        
                        
//                        m_bTouched = true ;
//                        m_Index = 0 ;
//                        m_pointPath[m_Index++] = ccpMult(pos, CC_CONTENT_SCALE_FACTOR());
//                        m_pParticle->resetSystem() ;
//                        m_pParticle->setPosition(pos) ;
//                        touch_Firend->m_skillTouchs=0;
//                        touch_Firend->isTouch=false;
//                        touchNum.push_back(pTouch->getID());
//                        touch_dic->setObject(touch_Firend, pTouch->getID());
//                        
                        return;
                    }
                }
            }else if(obj->m_btype==2&&!obj->m_skillTouch){
                MasterRole *master=(MasterRole *)obj;
                 m_personBeginPoint=pTouch->getLocation();
                if (master->isInSprite(pTouch)&&!master->isTouch) {
                    master->isTouch=true;
                    if (selectedPerson!=master) {
                        if (selectedPerson==NULL) {
                            selectedPerson=master;
                        }
                        selectedPerson->removeChildByTag(9119);
                        selectedPerson=master;
                        CCSprite *circle=CCSprite::create("select.png");
                        circle->setScale(0.8);
                        circle->setTag(9119);
                        circle->setPosition(ccp(master->getContentSize().width/2, master->getContentSize().height/2-30));
                        circle->runAction(CCScaleTo::create(0.1, 1));
                        master->addChild(circle,-1);
                        
                    }else{
                        
                    }
           
                    CCRect rect = CCRectMake(0,0,10,30);
                    CCRect rectInsets = CCRectMake(1,1,1,1);
                    CCSize winRect = CCSizeMake(10,30);
                    touch_line=CCScale9Sprite::create(LINE, rect, rectInsets);
                    touch_line->setTag(9200);
                    touch_line->setContentSize(winRect);
                    touch_line->setAnchorPoint(ccp(0, 0.5));
                    touch_line->setPosition(ccp(0, -20));
                    touch_line->setVisible(false);
                    master->addChild(touch_line,-1);
                    
                    CCSprite *touch_dir=CCSprite::create(DIRECTION);
                    touch_dir->setPosition(ccp(pos.x, pos.y));
                    touch_dir->setVisible(false);
                    touch_dir->setTag(master->m_id+3000);
                    
                    touch_line->setVisible(false);
                    this->addChild(touch_dir,-1);
                    
                    touch_dic->setObject(master, pTouch->getID());
                    touchNum.push_back(pTouch->getID());
                    return;
                }else{
                    master->m_skillTouch=master->skillbtns->touchSkillBtn(pTouch);
                    if (master->m_skillTouch) {
                        skillLayerBegan(master->skillId, master, pTouch);
//                        CCSprite *m_spOrg=CCSprite::create(CIRCLES);
//                        m_spOrg->setPosition(pTouch->getLocation());
//                        m_spOrg->setTag(1035);
//                        this->addChild(m_spOrg,99);
//                        touchNum.push_back(pTouch->getID());
//                        touch_dic->setObject(master, pTouch->getID());
                        return;
                    }
                }
            }else if(obj->m_btype==3&&!obj->m_skillTouch){
                
                CureRole *touch_Firend=(CureRole *)obj;
                 m_personBeginPoint=pTouch->getLocation();
                touch_Firend->m_skillTouch=touch_Firend->skillbtns->touchSkillBtn(pTouch);
                if (touch_Firend->isInSprite(pTouch)&&!touch_Firend->isTouch) {
                    if (selectedPerson!=touch_Firend) {
                        if (selectedPerson==NULL) {
                            selectedPerson=touch_Firend;
                        }
                        selectedPerson->removeChildByTag(9119);
                        selectedPerson=touch_Firend;
                        CCSprite *circle=CCSprite::create("select.png");
                        circle->setScale(0.8);
                        circle->setTag(9119);
                        circle->setPosition(ccp(touch_Firend->getContentSize().width/2, touch_Firend->getContentSize().height/2-30));
                        circle->runAction(CCScaleTo::create(0.1, 1));
                        touch_Firend->addChild(circle,-1);
                        
                    }else{
                        
                    }
                
                    touch_Firend->isTouch=true;
                    CCRect rect = CCRectMake(0,0,10,30);
                    CCRect rectInsets = CCRectMake(1,1,1,1);
                    CCSize winRect = CCSizeMake(10,30);
                    touch_line=CCScale9Sprite::create(LINE, rect, rectInsets);
                    touch_line->setTag(9200);
                    touch_line->setAnchorPoint(ccp(0, 0.5));
                    touch_line->setContentSize(winRect);
                    touch_line->setPosition(ccp(0, -20));
                    touch_line->setVisible(false);
                
                    touch_Firend ->addChild(touch_line,-1);
                    CCSprite *touch_dir=CCSprite::create(DIRECTION);
                    touch_dir->setPosition(ccp(pos.x, pos.y));
                    touch_dir->setVisible(false);
                    touch_dir->setTag(touch_Firend->m_id+3000);
                    touch_line->setVisible(false);
                    this->addChild(touch_dir,-1);
                    touch_dic->setObject(touch_Firend, pTouch->getID());
                    touchNum.push_back(pTouch->getID());
                    return;
                }else if(touch_Firend->m_skillTouch){
                    skillLayerBegan(touch_Firend->skillId, touch_Firend, pTouch);
                    
                    
//                    CCSprite *cureCirle=CCSprite::create("select.png");
//                    cureCirle->setPosition(pTouch->getLocation());
//                    cureCirle->setScale(5);
//                    cureCirle->setTag(1036);
//                    this->addChild(cureCirle,2);
//                    touch_dic->setObject(touch_Firend, pTouch->getID());
//                    touchNum.push_back(pTouch->getID());
                    return;
                }
            }

            
        }
        
    }
}
void BattleLayer::skillLayerBegan(int skillId,Person *person,CCTouch *pTouch){
    if(skillId==1){
        m_bTouched = true ;
        m_Index = 0 ;
        m_pointPath[m_Index++] = ccpMult(pTouch->getLocation(), CC_CONTENT_SCALE_FACTOR());
        m_pParticle->resetSystem() ;
        m_pParticle->setPosition(pTouch->getLocation()) ;
        person->m_skillTouchs=0;
        person->isTouch=false;
        touchNum.push_back(pTouch->getID());
        touch_dic->setObject(person, pTouch->getID());
        return;
    }else if(skillId==2){
        
    }else if (skillId==3){
        CCSprite *m_spOrg=CCSprite::create(CIRCLES);
        m_spOrg->setPosition(pTouch->getLocation());
        m_spOrg->setTag(1035);
        this->addChild(m_spOrg,99);
        touchNum.push_back(pTouch->getID());
        touch_dic->setObject(person, pTouch->getID());
        return;
    }else if(skillId==4){
        
    }else if(skillId==5){
        CCSprite *cureCirle=CCSprite::create("select.png");
        cureCirle->setPosition(pTouch->getLocation());
        cureCirle->setScale(5);
        cureCirle->setTag(1036);
        this->addChild(cureCirle,2);
        touch_dic->setObject(person, pTouch->getID());
        touchNum.push_back(pTouch->getID());
        return;
    }
//    switch (skillId) {
//        case 1:
//            m_bTouched = true ;
//            m_Index = 0 ;
//            m_pointPath[m_Index++] = ccpMult(pTouch->getLocation(), CC_CONTENT_SCALE_FACTOR());
//            m_pParticle->resetSystem() ;
//            m_pParticle->setPosition(pTouch->getLocation()) ;
//            person->m_skillTouchs=0;
//            person->isTouch=false;
//            touchNum.push_back(pTouch->getID());
//            touch_dic->setObject(person, pTouch->getID());
//            break;
//        case 2:
//            break;
//        case 3:
//            break;
//        case 4:
//            break;
//        case 5:
//            break;
//        default:
//            break;
//    }
}
void BattleLayer::skillLayerMoved(int skillId,Person *person,CCTouch *pTouch){
    if (skillId==1) {
        m_touchEndPoint = pTouch->getPreviousLocationInView() ;
        m_touchEndPoint = CCDirector::sharedDirector()->convertToGL(m_touchEndPoint) ;
        m_bTouched = true ;
        
        m_pParticle->setVisible(true) ;
        m_pParticle->setPosition(m_touchBeganPoint) ;
        CCPoint pointTmp = ccpMult(m_touchBeganPoint, CC_CONTENT_SCALE_FACTOR());
        Person *oldTarget=person->target;
        for (int i=0; i<appearArr->count(); i++) {
            EnemyRole *enemy=(EnemyRole *)appearArr->objectAtIndex(i);
            EnemyRole *ene=enemy;
            if (ene->isInSprite(pTouch)) {
                if (person->m_skillTouchs< person->m_maxTouchs) {
                    person->target=enemy;
                    float distance = ccpDistance(pTouch->getLocation(), m_pointPath[m_Index]) ;
                    if (distance>200) {
                        person->m_skillTouchs++;
                        ene->changeBlood(-(person->getAttackResult()+3));
                    }
                }else{
                    m_touchEndPoint = pTouch->getLocation() ;
                    m_pParticle->stopSystem() ;
                    touch_dic->removeObjectForKey(pTouch->getID());
                    person->m_skillTouch=false;
                    touchNum.erase(touchNum.begin()+pTouch->getID());
                    break;
                }
                
            }
            person->target=oldTarget;
        }
        if ( m_Index < POINT_NUM )
        {
            m_pointPath[m_Index++] = pointTmp ;
        }
        else {
            memmove(m_pointPath, &m_pointPath[1], sizeof(CCPoint)*(POINT_NUM-1)) ;
            m_pointPath[m_Index-1] = pointTmp ;
        }
    }else if(skillId==2){
        
    }else if(skillId==3){
        CCSprite *m_sp=(CCSprite *)this->getChildByTag(1035);
        if (m_sp) {
            m_sp->setPosition(pTouch->getLocation());
        }
    }else if(skillId==4){
        
    }else if(skillId==5){
        CCSprite *sp=(CCSprite *)this->getChildByTag(1036);
        sp->setPosition(pTouch->getLocation());
        return;
    }
}
void BattleLayer::skillLayerEnded(int skillId,Person *person,CCTouch *pTouch){
    if (skillId==1) {
        m_touchEndPoint = pTouch->getLocation() ;
        m_pParticle->stopSystem() ;
        person->m_skillTouch=false;
        FriendRoleSkillTouch=false;
        person->m_skillTouchs=0;
        person->isTouch=false;
        m_bTouched=false;
        touch_dic->removeObjectForKey(pTouch->getID());
    }else if(skillId==2){
        
    }else if(skillId==3){
        
        CCPoint m_touchPos=pTouch->getLocation();
//        BattleLayer *bl=(BattleLayer *)this->getParent();
//        CCLog("%d",appearArr->count());
        for (int i=0; i<appearArr->count(); i++) {
            EnemyRole *enemy=(EnemyRole *)appearArr->objectAtIndex(i);
            
//            info0->charid=GameData::shareGameData()->chaId;
//            info0->attack_id=this->m_id;
//            time_t tt=time(NULL);
//            char time[50]="";
//            sprintf(time, "%ld",tt);
            
//            info0->attack_time=time;
//            info0->skillId=skillId;
//            info0->countofAttack=0;
            
            
            
            
            if (ccpDistance(enemy->getPosition(), m_touchPos)<person->skill->m_skillRange) {
//                attackedInfo *infos=new attackedInfo();
//                infos->attacked_id=enemy->m_id;
//                infos->attacked_curblood=enemy->cur_hp;
//                infos->attacked_reduceblood=20;
//                info0->attackeds->addObject(infos);
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
        
        
        this->removeChildByTag(1035);
        person->m_skillTouch=false;
       // person->releaseSkill(pTouch);
        CCSprite *skillTexture=CCSprite::createWithSpriteFrameName(T_DH_TX_FPNG);
        skillTexture->setTag(1036);
        skillTexture->setPosition(pTouch->getLocation());
        CCCallFunc *func =CCCallFunc::create(this, callfunc_selector(BattleLayer::removeSkillTexture));
        CCSequence *seq=CCSequence::create(AnimatePacker::getInstance()->getAnimate(T_DH_TX_DATA),func,NULL);
        skillTexture->runAction(seq);
        
        this->addChild(skillTexture);
        float z=-skillTexture->getPositionY()/5;
        skillTexture->setZOrder(z);
    }else if(skillId==4){
        
    }else if(skillId==5){
        CCArray *arr=CCArray::create();
        this->removeChildByTag(1036);
        person->m_skillTouch=false;
        for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
            FriendlyRole *enemy=(FriendlyRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
            if (ccpDistance(enemy->getPosition(), pTouch->getLocation())<person->skill->m_skillRange) {
                
                arr->addObject(enemy);
            }
        }
        
      Person *oldperson=person->target;
        for (int i=0; i<arr->count(); i++) {
            Person *_person=(Person *)arr->objectAtIndex(i);
            person->target=_person;
            CCSprite *zlxg=CCSprite::createWithSpriteFrameName(M_ZL_TX_FPNG);
            zlxg->setTag(CUREBLOODACTION);//CUREBLOODACTION
            zlxg->runAction(AnimatePacker::getInstance()->getAnimate(M_ZL_TX_DATA));
            _person->addChild(zlxg,1);
            _person->changeBlood(person->getAttackResult()+6);
            
        }
        person->target=oldperson;
      //  curerole->moreCureTarget(arr);
        return;
    }
}

void BattleLayer::ccTouchesMoved(cocos2d::CCSet *set, cocos2d::CCEvent *event){
    CCSetIterator  sel= set->begin();
    for (; sel!=set->end(); sel++) {
        CCTouch *pTouch=(CCTouch *)(* sel);
        CCPoint pos= pTouch->getLocation();
        m_touchBeganPoint = pTouch->getLocationInView() ;
        m_personEndedPoint=pTouch->getLocation();
        m_touchBeganPoint = CCDirector::sharedDirector()->convertToGL(m_touchBeganPoint) ;
        path.push_back(Point2D(m_touchBeganPoint.x,m_touchBeganPoint.y));
        //画线
        for (vector<int>::const_iterator it=touchNum.begin(); it<touchNum.end(); it++) {
            if (pTouch->getID()==*it) {
                
                CCSprite *dir=NULL;
                Person *sp = (Person *)touch_dic->objectForKey(pTouch->getID());
                if (sp->isTouch) {
                    dir= (CCSprite *)this->getChildByTag(3000+sp->m_id);
                    if (dir) {
                        dir->setPosition(ccp(pos.x, pos.y));
                    }
                }
                
                if (ccpDistance(m_personBeginPoint, m_personEndedPoint)>10&&dir!=NULL){
                    dir->setVisible(true);
                }
                if (sp->m_btype==1) {
                    FriendlyRole *friendsRole=(FriendlyRole *)sp;
                    if (friendsRole->m_skillTouch&&!friendsRole->isTouch) {

                        skillLayerMoved(friendsRole->skillId,friendsRole,pTouch);
                       
                        break;
//                        m_touchEndPoint = pTouch->getPreviousLocationInView() ;
//                        m_touchEndPoint = CCDirector::sharedDirector()->convertToGL(m_touchEndPoint) ;
//                        m_bTouched = true ;
//                      
//                        m_pParticle->setVisible(true) ;
//                        m_pParticle->setPosition(m_touchBeganPoint) ;
//                        CCPoint pointTmp = ccpMult(m_touchBeganPoint, CC_CONTENT_SCALE_FACTOR());
//                        for (int i=0; i<appearArr->count(); i++) {
//                            EnemyRole *enemy=(EnemyRole *)appearArr->objectAtIndex(i);
//                            EnemyRole *ene=enemy;
//                            if (ene->isInSprite(pTouch)) {
//                                if (friendsRole->m_skillTouchs< friendsRole->m_maxTouchs) {
//                                    float distance = ccpDistance(pos, m_pointPath[m_Index]) ;
//                                    if (distance>200) {
//                                        friendsRole->m_skillTouchs++;
//                                        ene->changeBlood(friendsRole->getAttackResult()+3);
//                                    }
//                                }else{
//                                    m_touchEndPoint = pTouch->getLocation() ;
//                                    m_pParticle->stopSystem() ;
//                                    touch_dic->removeObjectForKey(pTouch->getID());
//                                    friendsRole->m_skillTouch=false;
//                                    touchNum.erase(touchNum.begin()+*it);
//                                    break;
//                                }
//                                    
//                            }
//                        }
//                        if ( m_Index < POINT_NUM )
//                        {
//                            m_pointPath[m_Index++] = pointTmp ;
//                        }
//                        else {
//                            memmove(m_pointPath, &m_pointPath[1], sizeof(CCPoint)*(POINT_NUM-1)) ;
//                            m_pointPath[m_Index-1] = pointTmp ;
//                        }
//                        break;
                    }else if(!friendsRole->m_skillTouch&&friendsRole->isTouch){
                        for (int i=0; i<appearArr->count(); i++) {
                            EnemyRole *ene=(EnemyRole *)appearArr->objectAtIndex(i);
                            CCScale9Sprite *touchline= (CCScale9Sprite *)sp->getChildByTag(9200);
                            if (ene->isInSprite(pTouch)) {
                                ene->select_red->setVisible(true);
                                touchline->initWithFile("redline.png");
                                dir->setPosition(ccp(ene->getPosition().x,ene->getPosition().y));
                                break;
                            }else{
                                touchline->initWithFile(LINE);
                                ene->select_red->setVisible(false);
                            }
                        }
                    }
                }
                else if (sp->m_btype==2){
                    if (sp->m_skillTouch) {
                        skillLayerMoved(sp->skillId,sp,pTouch);
//                        CCSprite *m_sp=(CCSprite *)this->getChildByTag(1035);
//                        if (m_sp) {
//                            m_sp->setPosition(pTouch->getLocation());
//                        }
                        
                    }else if(!sp->m_skillTouch&&sp->isTouch){
                        for (int i=0; i<appearArr->count(); i++) {
                            EnemyRole *enemy=(EnemyRole *)appearArr->objectAtIndex(i);
                            EnemyRole *ene=enemy;
                            if (ene->isInSprite(pTouch)) {
                                ene->select_red->setVisible(true);
                                dir->setPosition(ccp(ene->getPosition().x,ene->getPosition().y));
                            }else{
                                ene->select_red->setVisible(false);
                            }
                        }
                        if (ccpDistance(m_personBeginPoint, m_personEndedPoint)>10){
                            dir->setVisible(true);
                        }
                    }
                    
                }
                else if(sp->m_btype==3){
                    CureRole *cure=(CureRole *)sp;
                    if (cure->m_skillTouch) {
                        skillLayerMoved(sp->skillId,sp,pTouch);
                    }else if(!sp->m_skillTouch){
                        for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
                            FriendlyRole *ene=(FriendlyRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
                            if (ene->isInSprite(pTouch)&&sp!=ene) {
                                dir->setPosition(ccp(ene->getPosition().x,ene->getPosition().y));
                                ene->selectedCircle->setVisible(true);
                                
                            }else if(sp!=ene){
                                ene->selectedCircle->setVisible(false);
                                
                            
                            }else if (ene->isInSprite(pTouch)){
                                dir->setPosition(ccp(ene->getPosition().x,ene->getPosition().y));
                            }
                        }
                        if (ccpDistance(m_personBeginPoint, m_personEndedPoint)>10){
                            dir->setVisible(true);
                        }
                    }
                    
                }
                if (sp->isTouch&&!sp->m_skillTouch) {
                    CCSprite *touchline= (CCSprite *)sp->getChildByTag(9200);
                    CCPoint p1=sp->getPosition();
                    CCPoint p2=ccp(dir->getPosition().x, dir->getPosition().y);
                    //ccp(dir->getPosition().x, dir->getPosition().y);
                    double angle=(atan((p2.y-p1.y)/(p2.x-p1.x))*180/pi);
                    
                    if(p2.x>p1.x){
                        if(touchline){
                            
                            touchline->setRotation(-angle);
                        }
                    }else{
                        if(touchline){
                      
                            touchline->setRotation(180-angle);
                        }
                    }
                    float dis;
                    if(p2.x<190){
                        if(p2.y<810){
                            dis=ccpDistance(sp->getPosition(), ccp(190, p2.y));
                            dir->setVisible(true);
                            dir->setPosition(ccp(190,p2.y));
                            angle=(atan((p2.y-p1.y)/(190-p1.x))*180/pi);
                            if(p2.x>p1.x){
                                if(touchline){
                                    touchline->setRotation(-angle);
                                }
                            }else{
                                if(touchline){
                                    touchline->setRotation(180-angle);
                                }
                            }
                        }else{
                            dis=ccpDistance(sp->getPosition(), ccp(190, 810));
                            dir->setVisible(true);
                            dir->setPosition(ccp(190, 810));
                            angle=(atan((810-p1.y)/(190-p1.x))*180/pi);
                            if(190>p1.x){
                                if(touchline){
                                    touchline->setRotation(-angle);
                                }
                            }else{
                                if(touchline){
                                    touchline->setRotation(180-angle);
                                }
                            }
                        }
                        if(touchline){
                            touchline->setVisible(true);
                            touchline->setContentSize(CCSizeMake(dis, 30));
                            touchline->setAnchorPoint(ccp(0, 0.5));
                        }
                        
                    }else if(p2.x>1740){
                        if(p2.y<810){
                            dis=ccpDistance(sp->getPosition(), ccp(1740, p2.y));
                            dir->setVisible(true);
                            dir->setPosition(ccp(1740,p2.y));
                            angle=(atan((p2.y-p1.y)/(1740-p1.x))*180/pi);
                            if(p2.x>p1.x){
                                if(touchline){
                                    touchline->setRotation(-angle);
                                }
                            }else{
                                if(touchline){
                                    touchline->setRotation(180-angle);
                                }
                            }
                            
                        }else{
                            dis=ccpDistance(sp->getPosition(), ccp(1740, 810));
                            dir->setVisible(true);
                            dir->setPosition(ccp(1740,810));
                            angle=(atan((810-p1.y)/(1740-p1.x))*180/pi);
                            
                            if(1740>p1.x){
                                if(touchline){
                                    touchline->setRotation(-angle);
                                }
                            }else{
                                if(touchline){
                                    touchline->setRotation(180-angle);
                                }
                            }
                            
                        }
                        if(touchline){
                            touchline->setVisible(true);
                            touchline->setContentSize(CCSizeMake(dis, 30));
                            touchline->setAnchorPoint(ccp(0, 0.5));
                        }
                    }
                    else if(p2.y>810){
                        
                        if (p2.x>190&&p2.x<1740) {
                            dis=ccpDistance(sp->getPosition(), ccp(p2.x, 810));
                            dir->setVisible(true);
                            dir->setPosition(ccp(p2.x,810));
                            angle=(atan((810-p1.y)/(p2.x-p1.x))*180/pi);
                            if(p2.x>p1.x){
                                if(touchline){
                                    touchline->setRotation(-angle);
                                }
                            }else{
                                if(touchline){
                                    touchline->setRotation(180-angle);
                                }
                            }
                            if(touchline){
                                touchline->setVisible(true);
                                touchline->setContentSize(CCSizeMake(dis, 30));
                                touchline->setAnchorPoint(ccp(0, 0.5));
                            }
                        }
                    }
                    else{
//                        if (sp->m_btype!=3) {
//                            for (int i=0; i<appearArr->count(); i++) {
//                                EnemyRole *ene=(EnemyRole *)appearArr->objectAtIndex(i);
//                                CCScale9Sprite *touchline= (CCScale9Sprite *)sp->getChildByTag(9200);
//                                if (ene->isInSprite(pTouch)) {
//                                    ene->select_red->setVisible(true);
//                                    touchline->initWithFile("redline.png");
//                                    dir->setPosition(ccp(ene->getPosition().x,ene->getPosition().y));
//                                    break;
//                                }else{
//                                    touchline->initWithFile(LINE);
//                                    ene->select_red->setVisible(false);
//                                }
//                            }
//                        }
                        dis=ccpDistance(p1, p2);
                        if(touchline){
                            touchline->setVisible(true);
                            touchline->setContentSize(CCSizeMake(dis, 30));
                            touchline->setAnchorPoint(ccp(0, 0.5));
                        }
                        dir->setVisible(true);
                        
                    }
                }
            }  
        }
    }
}
void BattleLayer::ccTouchesEnded(cocos2d::CCSet *set, cocos2d::CCEvent *event){
    CCSetIterator  sel= set->begin();
    
    
    for (; sel!=set->end(); sel++) {
        CCTouch *pTouch=(CCTouch *)(* sel);
        checkGestures(path);
        m_bTouched = false ;
        
        
        
        for (vector<int>::const_iterator it=touchNum.begin(); it<touchNum.end(); it++) {
            
            if (pTouch->getID()==*it) {
                touchNum.erase(touchNum.begin()+*it);
//                pinballlayer=PinballLayer::create(ccp(pTouch->getLocation().x, pTouch->getLocation().y));
//                this->addChild(pinballlayer,3);
    
//                this->schedule(schedule_selector(BattleLayer::enemyHited));
                removeTouchDir(pTouch);
            }
        }
    }
}
void BattleLayer::removeSkillTexture(){
    this->removeChildByTag(1036);
}
void BattleLayer::removeTouchDir(CCTouch *touch){
    int tag= touch->getID();
    Person *obj = (Person *)touch_dic->objectForKey(tag);
    m_personEndedPoint=touch->getLocation();
    if (obj->m_btype==1) {
         FriendlyRole *sp=(FriendlyRole *)obj;
        
        
        if (sp->m_skillTouch&&!sp->isTouch){
            skillLayerEnded(sp->skillId, sp, touch);
//            m_touchEndPoint = touch->getLocation() ;
//            m_pParticle->stopSystem() ;
//            sp->m_skillTouch=false;
//            FriendRoleSkillTouch=false;
//            sp->m_skillTouchs=0;
//            sp->isTouch=false;
//            m_bTouched=false;
//            touch_dic->removeObjectForKey(tag);
            return;
        }
        
        CCSprite  *dir= (CCSprite *)this->getChildByTag(3000+obj->m_id);
        CCTouch *m_touch=new CCTouch();
        CCPoint pos=  CCDirector::sharedDirector()->convertToUI(ccp(dir->getPosition().x, dir->getPosition().y+40));
        m_touch-> setTouchInfo(100, pos.x, pos.y);
        sp->m_skillTouch=false;
        sp->isTouch=false;
        this->removeChildByTag(3000+sp->m_id);
        sp->removeChildByTag(9200);
        touch_dic->removeObjectForKey(tag);
        if(ccpDistance(m_personBeginPoint, m_personEndedPoint)<10){
            return;
            
        }
        for (int i=0; i<appearArr->count(); i++) {
        EnemyRole *enemy=(EnemyRole *)appearArr->objectAtIndex(i);
            enemy->select_red->setVisible(false);
            if (enemy->isInSprite(m_touch)) {
                
                if (sp->target==enemy) {
                        
                    return;
                }
                sp->target=enemy;
                    
                sp->touchEvent(m_touch,true);//touch
                return;
            }
            
        }
        sp->touchEvent(m_touch,false);//touch
    }
    else if (obj->m_btype==2){
        MasterRole *sp=(MasterRole *)obj;
        sp->isTouch=false;
        
        if (sp->m_skillTouch) {
            skillLayerEnded(sp->skillId, sp, touch);
            return;
//            this->removeChildByTag(1035);
//            sp->m_skillTouch=false;
//            sp->releaseSkill(touch);
//            CCSprite *skillTexture=CCSprite::createWithSpriteFrameName(T_DH_TX_FPNG);
//            skillTexture->setTag(1036);
//            skillTexture->setPosition(touch->getLocation());
//            CCCallFunc *func =CCCallFunc::create(this, callfunc_selector(BattleLayer::removeSkillTexture));
//            CCSequence *seq=CCSequence::create(AnimatePacker::getInstance()->getAnimate(T_DH_TX_DATA),func,NULL);
//            skillTexture->runAction(seq);
//            
//            this->addChild(skillTexture);
//            float z=-skillTexture->getPositionY()/5;
//            skillTexture->setZOrder(z);
        }else{
           
            CCSprite  *dir= (CCSprite *)this->getChildByTag(3000+obj->m_id);
            CCTouch *m_touch=new CCTouch();
            CCPoint pos=  CCDirector::sharedDirector()->convertToUI(ccp(dir->getPosition().x, dir->getPosition().y+40));
            m_touch-> setTouchInfo(100, pos.x, pos.y);
            this->removeChildByTag(3000+sp->m_id);
            sp->removeChildByTag(9200);
            touch_dic->removeObjectForKey(tag);
            if(ccpDistance(m_personBeginPoint, m_personEndedPoint)<10){
                return;
                
            }
            for (int i=0; i<appearArr->count(); i++) {
                EnemyRole *enemy=(EnemyRole *)appearArr->objectAtIndex(i);
//                if (enemy->type==1) {
//                    EnemyLongRangeBoss *ene=(EnemyLongRangeBoss *)enemy;
//                    if (ene->isInSprite(touch)) {
//                        sp->target=ene;
//                        sp->touchEvent(touch,true);
//                        return;
//                    }
//                }else{
                    if (enemy->isInSprite(m_touch)) {
                        enemy->select_red->setVisible(false);
                        sp->target=enemy;
                        sp->touchEvent(m_touch,true);
                        return;
                    }
//                }
                
            }
            
            sp->touchEvent(m_touch,false);
        }
        sp->m_skillTouch=false;
    }
    else if(obj->m_btype==3){
        
        CureRole *curerole=(CureRole *)obj;
        
//        CCArray *arr=CCArray::create();
        if (curerole->m_skillTouch) {
             skillLayerEnded(curerole->skillId, curerole, touch);
//            this->removeChildByTag(1036);
//            curerole->m_skillTouch=false;
//            for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
//                FriendlyRole *enemy=(FriendlyRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
//                CCLog("%d",curerole->skill->m_skillRange);
//                if (ccpDistance(enemy->getPosition(), touch->getLocation())<curerole->skill->m_skillRange) {
//                    
//                    arr->addObject(enemy);
//                }
//            }
//            curerole->moreCureTarget(arr);
            return;
        }
        curerole->isTouch=false;
        curerole->removeChildByTag(9200);
        touch_dic->removeObjectForKey(tag);
        CCSprite  *dir= (CCSprite *)this->getChildByTag(3000+obj->m_id);
        CCTouch *m_touch=new CCTouch();
        CCPoint pos=  CCDirector::sharedDirector()->convertToUI(ccp(dir->getPosition().x, dir->getPosition().y+40));
        m_touch-> setTouchInfo(100, pos.x, pos.y);
        this->removeChildByTag(3000+curerole->m_id);
        
        for (int i=0; i<GameData::shareGameData()->roleArr->count(); i++) {
            FriendlyRole *enemy=(FriendlyRole *)GameData::shareGameData()->roleArr->objectAtIndex(i);
            if (enemy->isInSprite(touch)&&obj!=enemy) {
                enemy->selectedCircle->setVisible(false);
                curerole->target=enemy;
                curerole->isCure=false;
                curerole->touchEvent(m_touch,true);
                return;
            }else if(enemy->isInSprite(touch)){
                curerole->target=enemy;
                curerole->isCure=false;
                curerole->touchEvent(m_touch,true);
                return;
            }
        }
        if(ccpDistance(m_personBeginPoint, m_personEndedPoint)<10){
            return;
            
        }
        curerole->touchEvent(m_touch,false);
        
    }
}

void BattleLayer::checkGestures(Path2D paths){
    RecognitionResult results=geo->recognize(paths);
    CCLog("%s",results.name.c_str());
//    if (!strcmp(results.name.c_str(), "Line")) {
//        gestureTag = 1;//1 直线
//    }else if(!strcmp(results.name.c_str(),"Circle")){
//        gestureTag = 2;
//        
//    }
}
void BattleLayer::enemyHited(){
    for (int i=0; i<GameData::shareGameData()->enemyArr->count(); i++) {
        EnemyRole *enemy=(EnemyRole *)GameData::shareGameData()->enemyArr->objectAtIndex(i);
        
        if (ccpDistance(enemy->getPosition(), pinballlayer->par->getPosition())<100&&enemy->g_bPinBallFlag) {
            enemy->PinballHitEnemy(pinballlayer->par);
            continue;
        }
    }
    
}
void BattleLayer::onExit(){
    CCLayer::onExit();
    
    
}
BattleLayer::~BattleLayer(){

    appearArr->removeAllObjects();
    touch_dic->removeAllObjects();
    //g_skillRoleArr->removeAllObjects();
    touch_arr->removeAllObjects();
    FriendRoleSkillTouch=false;
    appearArr->release();
    touch_dic->release();

    m_pParticle->stopSystem();
    CCArmatureDataManager::sharedArmatureDataManager()->removeAll();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeTextureForKey(M_BATTLE_BG);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(CIRCLES);
    CCTextureCache::sharedTextureCache()->removeTextureForKey("fuzinvrun0.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("fuzinvrun1.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("boss/daozhangrun0.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("boss/daozhangrun1.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("boss/daozhang1.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("boss/daozhang0.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("man0/shanzinan0.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("man0/shanzinan1.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("man0/shanzinanrun0.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("man0/shanzinanrun1.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("fuzinv0.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("fuzinv1.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("zhiliao.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("dihuo.png");
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA4);
    CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureData(ARMATURE_ACTION_DATA5);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCArmatureDataManager::purge();
    
    AnimatePacker::getInstance()->freeAnimations(M_HQ_TX_XML);
    AnimatePacker::getInstance()->freeAnimations(T_DH_TX_XML);
    AnimatePacker::getInstance()->freeAnimations(M_ZL_TX_XML);
    AnimatePacker::getInstance()->freeAnimations(T_DG_TX_XML);
    
    CCTextureCache::sharedTextureCache()->removeAllTextures();
}




PinballLayer::PinballLayer(){
    
}
PinballLayer::~PinballLayer(){
    
}
PinballLayer * PinballLayer::create(cocos2d::CCPoint enterpos){
    PinballLayer* pRet = new PinballLayer();
    if (pRet && pRet->initWithEnterPos(enterpos))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
bool PinballLayer::initWithEnterPos(cocos2d::CCPoint pos){
    if (PinballLayer::init())
    {
        CCSize winsize=CCDirector::sharedDirector()->getWinSize();
        b2Vec2 gravity=b2Vec2(0.0f, 0.0f);
        m_world=new b2World(gravity);
        
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0, 0);
        m_groundbody=m_world->CreateBody(&groundBodyDef);
        b2EdgeShape groundBox;
        b2FixtureDef groundBoxDef;
        groundBoxDef.shape=&groundBox;
        groundBox.Set(b2Vec2(0,0), b2Vec2(winsize.width/PTM_RATIO, 0));
        m_bottomfixture = m_groundbody->CreateFixture(&groundBoxDef);
        groundBox.Set(b2Vec2(0,0), b2Vec2(0, winsize.height/PTM_RATIO));
        m_groundbody->CreateFixture(&groundBoxDef);
        groundBox.Set(b2Vec2(0, winsize.height/PTM_RATIO), b2Vec2(winsize.width/PTM_RATIO,
                                                                  winsize.height/PTM_RATIO));
        m_groundbody->CreateFixture(&groundBoxDef);
        groundBox.Set(b2Vec2(winsize.width/PTM_RATIO, winsize.height/PTM_RATIO),
                      b2Vec2(winsize.width/PTM_RATIO, 0));
        m_groundbody->CreateFixture(&groundBoxDef);
        
        par=CCParticleSystemQuad::create("fireBalls.plist");
    
        par->setPosition(ccp(100, 100));
        par->setTag(1);
        this->addChild(par);
        
        b2BodyDef ballBodyDef;
        ballBodyDef.type = b2_dynamicBody;
        ballBodyDef.position.Set(100/PTM_RATIO, 100/PTM_RATIO);
        ballBodyDef.userData = par;
        b2Body * ballBody = m_world->CreateBody(&ballBodyDef);
        
        // Create circle shape
        b2CircleShape circle;
        
        circle.m_radius = 26.0/PTM_RATIO;
        
        // Create shape definition and add to body
        b2FixtureDef ballShapeDef;
        ballShapeDef.shape = &circle;
        ballShapeDef.density = 1.0f;
        ballShapeDef.friction = 0.f;
        ballShapeDef.restitution = 1.0f;
        m_ballfixture = ballBody->CreateFixture(&ballShapeDef);
        
        b2Vec2 force = b2Vec2(pos.x/100, pos.y/100);
        ballBody->ApplyLinearImpulse(force, ballBodyDef.position);
        
        start();
        return true;
    }
    return false;
}
void PinballLayer::times(float dt){
    m_world->Step(0.01, 10, 10);
    for(b2Body *b = m_world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            CCSprite *sprite = (CCSprite *)b->GetUserData();
            sprite->setPosition(ccp(b->GetPosition().x * PTM_RATIO,
                                    b->GetPosition().y * PTM_RATIO));
            
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            
        }
    }
}
void PinballLayer::start(){
    this->schedule(schedule_selector(PinballLayer::times),0.01);
}
void PinballLayer::stop(){
    par->setVisible(false);
    this->unschedule(schedule_selector(PinballLayer::times));
}
