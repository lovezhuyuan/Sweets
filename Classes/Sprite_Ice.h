//
//  Sprite_Ice.hpp
//  Sweets
//
//  Created by xdlc on 2017/10/25.
//
//

#ifndef Sprite_Ice_h
#define Sprite_Ice_h
#include "cocos2d.h"
#include "SpriteNew.h"
class Sprite_Ice :public SpriteNew{
private:
    //int m_intIceNum{0};
    int m_intIceLayerNum{1};
    int m_intTtqLayerNum{0};
    int m_intIceClickNum{5};
private:
    virtual bool init();
    void onEnterTransitionDidFinish();
public:
    static Sprite_Ice* createWithTexture(cocos2d::Texture2D *texture);
    CREATE_FUNC(Sprite_Ice);
    int inline getTtqLayerNum()const{return m_intTtqLayerNum;};
    int inline getIceLayerNum()const{return m_intIceLayerNum;};
    //int inline getIceLayerNum()const{return m_intIceLayerNum;};
   // SET_GET_VARLUE_INLINE(int,{0},m_intIceLayerNum,IceLayerNum);
   // SET_GET_VARLUE_INLINE(int,{0},m_intTtqLayerNum,TtqLayerNum);
   // SET_GET_VARLUE_INLINE(int,{5},m_intIceClickNum,IceClickNum);
    void alterTtqLayerNum(const int& num);
    void alterIceLayerNum(const int & num);
    void alterClickNum(const int & num);
};
#endif /* Sprite_Ice_hpp */
