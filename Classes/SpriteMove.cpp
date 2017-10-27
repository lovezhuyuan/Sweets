#include "SpriteMove.h"
#include "AppDelegate.h"
USING_NS_CC;
bool SpriteMove::init(){
    if ( !Sprite::init() )
    {
        return false;
    }
    return true;
}
void SpriteMove::onEnter(){
    Sprite::onEnter();
    //数据初始化
    this->m_visibleSize = Director::getInstance()->getVisibleSize();
    this->m_visibleOrigin = Director::getInstance()->getVisibleOrigin();
}
void SpriteMove::moveAction(std::function<void()> func){
    float duration =0.0f;
    Vec2 postion;
    auto child = this->getChildByName("tail");
    if(this->getDirection()=='w'){  //上飞
        duration = 13.0f-this->getPositionY()/80.0f;
        postion = Vec2(this->getPositionX(),12.0f*80.0f+this->m_visibleOrigin.y-this->getBoundingBox().size.height*0.5);
        child->setPosition(Vec2(this->getBoundingBox().size.width*0.5,-this->getBoundingBox().size.height*0.5));
    }else if(this->getDirection()=='s'){
        child->setPosition(Vec2(this->getBoundingBox().size.width*0.5,1.5f*this->getBoundingBox().size.height));
        child->setRotation(180);
        duration = this->getPositionY()/80.0f-2.0f;
        postion = Vec2(this->getPositionX(),3.0f*80.0f+this->m_visibleOrigin.y+this->getBoundingBox().size.height*0.5);
    }else if(this->getDirection()=='a'){
        child->setPosition(Vec2(1.5f*this->getBoundingBox().size.width,this->getBoundingBox().size.height*0.5));
        child->setRotation(-90);
        duration = this->getPositionX()/80.0f+1.0f;
        postion = Vec2(0.0f+this->m_visibleOrigin.x+this->getBoundingBox().size.width*0.5,this->getPositionY());
    }else if(this->getDirection()=='d'){
        duration =10.0f -this->getPositionX()/80.0f;
        postion = Vec2(9.0f*80.0f+this->m_visibleOrigin.x-this->getBoundingBox().size.width*0.5,this->getPositionY());
        child->setPosition(Vec2(-this->getBoundingBox().size.width*0.5,this->getBoundingBox().size.height*0.5));
        child->setRotation(90);
    }
    this->runAction(Sequence::create(MoveTo::create(duration*0.15,postion),DelayTime::create(0.2f),CallFunc::create([=](){
        //撞碎了的动画
        auto node = Sprite::create();
        this->getParent()->addChild(node);
        node->setPosition(this->getPosition());
        node->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("ttqbroken")),RemoveSelf::create(),NULL));
        
        if(func){
            func();
        }
    }),RemoveSelf::create(),NULL));  //延时0.2 秒移除
}
SpriteMove* SpriteMove::createWithTexture(Texture2D *texture)
{
    SpriteMove *sprite = new (std::nothrow) SpriteMove();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}









