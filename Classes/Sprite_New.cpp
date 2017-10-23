#include "Sprite_New.h"
#include "AppDelegate.h"
USING_NS_CC;
bool Sprite_New::init(){
    if ( !Sprite::init() )
    {
        return false;
    }
    return true;
}
void Sprite_New::onEnter(){
    Sprite::onEnter();
    //数据初始化
    this->m_visibleSize = Director::getInstance()->getVisibleSize();
    this->m_visibleOrigin = Director::getInstance()->getVisibleOrigin();
}
void Sprite_New::moveAction(std::function<void()> func){
    float duration =0.0f;
    Vec2 postion;
    if(this->getDirection()=='w'){
        duration = 13.0f-this->getPositionY()/80.0f;
        postion = Vec2(this->getPositionX(),13.0f*80.0f+this->m_visibleOrigin.y);
    }else if(this->getDirection()=='s'){
        duration = this->getPositionY()/80.0f-2.0f;
        postion = Vec2(this->getPositionX(),2.0f*80.0f+this->m_visibleOrigin.y);
    }else if(this->getDirection()=='a'){
        duration = this->getPositionX()/80.0f+1.0f;
        postion = Vec2(-80.0f+this->m_visibleOrigin.x,this->getPositionY());
    }else if(this->getDirection()=='d'){
        duration =10.0f -this->getPositionX()/80.0f;
        postion = Vec2(10.0f*80.0f+this->m_visibleOrigin.x,this->getPositionY());
    }
    this->runAction(Sequence::create(MoveTo::create(duration*0.3,postion),DelayTime::create(0.2f),CallFunc::create([=](){
        if(func){
            func();
        }
    }),RemoveSelf::create(),NULL));  //延时0.2 秒移除
}
Sprite_New* Sprite_New::createWithTexture(Texture2D *texture)
{
    Sprite_New *sprite = new (std::nothrow) Sprite_New();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}









