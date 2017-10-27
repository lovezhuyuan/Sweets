#pragma once
#include "cocos2d.h"
#define SET_GET_VARLUE_INLINE(varType,DefaultValue, varName, funName)\
protected: varType varName;\
public: virtual inline varType get##funName(void) const { return varName; }\
public: virtual inline void set##funName(varType var){ varName = var; }
class SpriteNew :public cocos2d::Sprite{
public:
    cocos2d::Vec2 m_visibleOrigin;
    cocos2d::Size m_visibleSize;
    cocos2d::Vec2  m_index;  //所在下标
public:
    virtual bool init();
    void onEnterTransitionDidFinish();

    inline void setIndexX(const int & x){m_index.x = x;};
    inline void setIndexY(const int & y){m_index.y = y;};
    inline void setIndex(const int & x,int & y){m_index.x = x;m_index.y = y;};
    inline void setIndex(cocos2d::Vec2 & vec){m_index.x= vec.x;m_index.y= vec.y;};
    inline const cocos2d::Vec2 getIndex(){return m_index;};
    static SpriteNew* createWithTexture(cocos2d::Texture2D *texture);
    CREATE_FUNC(SpriteNew);
};
