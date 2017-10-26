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
    unsigned short m_index[2];  //所在下标
public:
    virtual bool init();
    void onEnterTransitionDidFinish();

    inline void setIndexX(const unsigned short x){m_index[0] = x;};
    inline void setIndexY(const unsigned short y){m_index[1] = y;};
    inline void setIndex(const unsigned short x,const unsigned short y){m_index[0] = x;m_index[1] = y;};
    inline const unsigned short* getIndex(){return m_index;};
    static SpriteNew* createWithTexture(cocos2d::Texture2D *texture);
    CREATE_FUNC(SpriteNew);
};
