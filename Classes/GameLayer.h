#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "Sprite_Ice.h"
#define NDEBUG
enum usePropType{
    none,
    bomb,
    jelly,
    magicmirror,
    fire
};
class GameLayer : public cocos2d::Layer
{
public:
    friend class Sprite_Ice;
private:
    std::vector<std::string> inline getTtqPathVector()const{return m_ttqpath;};
    std::vector<std::string> inline getIcePathVector()const{return m_icepath;};
private:  //私有值
    static GameLayer* m_instance;
    
    cocos2d::Size m_visibleSize;
    cocos2d::Vec2 m_visibleOrigin;
   
    cocos2d::Vector<cocos2d::Sprite*>m_vector_moveSp;
    cocos2d::EventListenerTouchOneByOne* m_listen; //点击事件监听
    std::vector<std::string> m_ttqpath;
    std::vector<std::string> m_cltPath;
    std::vector<std::string> m_icepath;
    std::vector<std::string> m_cltMove;
    std::vector<std::string> m_uipath;
    std::map<int,cocos2d::Map<int,cocos2d::Sprite*>> m_mapSprite;
    cocos2d::Layer* m_layer_gameOver{nullptr};
    cocos2d::Label* m_labelClickNum{nullptr};
//    cocos2d::Rect m_ArrayRect[9][9];//9*9 九宫格
//    cocos2d::Vec2 m_ArrayVec2[9][9];
    cocos2d::Node* sweetMagicNode{nullptr};
    cocostudio::timeline::ActionTimeline* sweetMagicTimeLine{nullptr};
    cocos2d::Node* m_nodeFire{nullptr};
    cocostudio::timeline::ActionTimeline* m_fireTimeline{nullptr};
    cocos2d::MenuItemFont* btn_Bomb;
    cocos2d::MenuItemFont* btn_SweetMagic;
    cocos2d::MenuItemFont* btn_jelly;
    cocos2d::MenuItemFont* btn_magicmirror;
    cocos2d::MenuItemFont* btn_fire;
    const int maxCltNum{15};  //最大巧克力数
    int m_intEliminateNum{0}; //连消数量
    int m_intClickNum{0};  //点击次数
    int m_intlevel{1};    //关卡级数
    int  m_ArrayNum[7];
    usePropType m_usePropType{none};
    int m_intTtqNum{0};  //需要消除的 ttq ice
    int m_intCltNum{0};
private:  //私有方法
    //void alterIceStatus(cocos2d::Sprite* & ice,const int & clickNum = 0,const int & ttqNum = 1);
    bool isCanTouch();//获取是否可点状态
    void btnCallBack(Ref* ref,const std::string& name);
    cocos2d::Sprite* getChild(const int& index1,const int& index2);//移除sp
    void deleteChild(const int & index1,const int &  index2);//移除sp
    void deleteChild(cocos2d::Sprite* sp);
    void readJson(const std::string & path,const int& id);
    void getEmptyIndex(int* index,const std::string & ignoreName=""/*被忽略的sp名*/); //返回空位
    cocos2d::Sprite* addElement(const std::string & path,const int & tag,const unsigned short & index1,const unsigned short & index2,const std::string &name);
    void chocolateCollision(const int index1,const int index2);
    void initData();//初始化游戏数据
    void update(float dt);
    void alteredState(cocos2d::Sprite* & sp);
    void alteredClt(cocos2d::Sprite* & sp);
    void GameOver(const bool & victory);//游戏结束
    void GameRestart(Ref* ref);
    void initAnimation(const char* formatPath,const int & starIndex,const int& length,const float & perUnit,const std::string & animationName,const bool  &restoreOriginalFrame = false);
    cocos2d::Vec2 positionConverToIndex(cocos2d::Vec2 & vec);
    cocos2d::Vec2 indexConverToPosition(cocos2d::Vec2 & vec);
    bool onTouchBegan(cocos2d::Touch * touch,cocos2d::Event* event);
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    void onEnter();
    void onExit();
    void cleanup();
    void initUI();
public:
    static inline GameLayer* getInstance(){return m_instance;};
    CREATE_FUNC(GameLayer);

};

