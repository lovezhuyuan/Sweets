//
//  SpriteNew.cpp
//  Sweets
//
//  Created by xdlc on 2017/10/24.
//
//

#include "SpriteNew.h"
#include "AppDelegate.h"
USING_NS_CC;
bool SpriteNew::init(){
    if ( !Sprite::init() )
    {
        return false;
    }
    return true;
}
void SpriteNew::onEnterTransitionDidFinish(){
    Sprite::onEnterTransitionDidFinish();
    //数据初始化
    this->m_visibleSize = Director::getInstance()->getVisibleSize();
    this->m_visibleOrigin = Director::getInstance()->getVisibleOrigin();
}
SpriteNew* SpriteNew::createWithTexture(Texture2D *texture)
{
    SpriteNew *sprite = new (std::nothrow) SpriteNew();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
