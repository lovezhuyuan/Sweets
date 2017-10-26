//
//  Sprite_Ice.cpp
//  Sweets
//
//  Created by xdlc on 2017/10/25.
//
//

#include "Sprite_Ice.h"
#include "AppDelegate.h"
#include "GameLayer.h"
USING_NS_CC;
bool Sprite_Ice::init(){
    if ( !SpriteNew::init() )
    {
        return false;
    }
    return true;
}
void Sprite_Ice::onEnterTransitionDidFinish(){
    SpriteNew::onEnterTransitionDidFinish();
    //数据初始化
    this->m_visibleSize = Director::getInstance()->getVisibleSize();
    this->m_visibleOrigin = Director::getInstance()->getVisibleOrigin();
    //添加子 甜甜圈
    Sprite * child = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(GameLayer::getInstance()->getTtqPathVector()[0]));
    child->setTag(0);
    child->setPosition(Vec2(40,40));
    child->setLocalZOrder(-1);
    child->setVisible(false);
    child->setName("ttq");
    this->addChild(child);
    
}
Sprite_Ice* Sprite_Ice::createWithTexture(Texture2D *texture)
{
    Sprite_Ice *sprite = new (std::nothrow) Sprite_Ice();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
void Sprite_Ice::alterTtqLayerNum(const int& num){
    auto ttqNum = this->getTtqLayerNum()+num;
    auto addIce = ttqNum/5;  //添加的冰数
    if(addIce+this->getIceLayerNum()>3){
        ttqNum=4;
    }else{
        ttqNum=ttqNum%5;
    }
    if(addIce>0){
        alterIceLayerNum(1);
    }
    auto child = static_cast<Sprite*>(this->getChildByName("ttq"));
    child->setVisible(false);
    if(ttqNum>0){
        child->setVisible(true);
        child->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(GameLayer::getInstance()->getTtqPathVector()[ttqNum-1]));
    }
    this->m_intTtqLayerNum = ttqNum;
}
void Sprite_Ice::alterIceLayerNum(const int & num){
    if(num>0&&this->getIceLayerNum()>=3){
        return;
    }
    this->m_intIceLayerNum = MIN(this->getIceLayerNum()+num,3);//冰层不大于3
    
    if(this->getIceLayerNum()<1){
        //移除自身
        GameLayer::getInstance()->deleteChild(this);
        return;
    }
    //触发 特效------
    
    
    //改变纹理
    this->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(GameLayer::getInstance()->getIcePathVector()[this->getIceLayerNum()-1]));
    
}
void Sprite_Ice::alterClickNum(const int & num){
    if(this->m_intIceClickNum>num){
        this->m_intIceClickNum-=num;
    }else{
        this->alterIceLayerNum(-1); //减一层冰
        this->m_intIceClickNum+=5; //增加5次点击
        this->alterClickNum(num);
    }
}
















