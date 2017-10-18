#pragma once
#include "cocos2d.h"
class GameLayer : public cocos2d::Layer
{

private:
    cocos2d::Size m_visibleSize;
    cocos2d::Vec2 m_visibleOrigin;
   
    cocos2d::Vector<cocos2d::Sprite*>m_vector_moveSp;
    cocos2d::EventListenerTouchOneByOne* m_listen;
    cocos2d::Vector<cocos2d::Texture2D*> m_texture_dikuai;
    std::map<int,cocos2d::Map<int,cocos2d::Sprite*>> m_mapSprite;
    cocos2d::Layer* m_layer_gameOver{nullptr};
   
    int m_intlevel{0};
    int m_intOneNum{0};
    int m_intTwoNum{0};
    int m_intThreeNum{0};
    int m_intFourNum{0};
    int m_intIceNum{0};
    int m_intDevilNum{0};
    int m_intChocolatesNum{0};
private:
    void readJson(std::string path,int id);
    int* getEmptyIndex(int *index); //返回空位
    cocos2d::Sprite* addObject(int textureIndex,int tag,int* positonIndex,std::string name);
    void initData();//初始化游戏数据
    void slowUpdate(float dt);
    void alteredState(cocos2d::Sprite* sp);
    void GameOver(bool vector);//游戏结束
    void GameRestart(Ref* ref);
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    void onEnter();
    void onExit();
    void cleanup();
    void initUI();
public:
    CREATE_FUNC(GameLayer);

};

