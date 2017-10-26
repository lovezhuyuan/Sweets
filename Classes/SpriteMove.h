#pragma once
#include "cocos2d.h"
class SpriteMove :public cocos2d::Sprite{
private:
    char m_charDirection{NULL};// wsad 上下左右
    cocos2d::Vec2 m_visibleOrigin;
    cocos2d::Size m_visibleSize;
private:
    virtual bool init();
    void onEnter();
public:
    inline void setDirection(const char a){m_charDirection=a;};
    inline char getDirection()const{return m_charDirection;};
    void moveAction(std::function<void()> func);
    static SpriteMove* createWithTexture(cocos2d::Texture2D *texture);
    CREATE_FUNC(SpriteMove);
};
