#include "GameLayer.h"
#include "AppDelegate.h"
#include "external/json/document.h"
#include "SpriteMove.h"
#include "SpriteNew.h"
#include "Sprite_Ice.h"
USING_NS_CC;
GameLayer* GameLayer::m_instance = nullptr;
bool GameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}
void GameLayer::onEnter(){
    Layer::onEnter();
    //数据初始化
    this->m_visibleSize = Director::getInstance()->getVisibleSize();
    this->m_visibleOrigin = Director::getInstance()->getVisibleOrigin();
    this->m_vector_moveSp.reserve(324);//初始化 vector大小   这个必须 （避免vector 内存重置）
}
void GameLayer::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    m_instance = this;
    //    //创建裁剪节点
    //    auto clipNode = ClippingNode::create();
    //    clipNode->setInverted(true);
    //    clipNode->setAlphaThreshold(1);//0  会根据 Alpha 取图形状    1 长方形
    //    this->addChild(clipNode,1000);
    //
    
    //    auto dibu = Sprite::create("Map/DDLFT_ditu.png");
    //    //dibu->setTextureRect(Rect(0,0,720,720));
    //    dibu->setPosition(this->m_visibleOrigin.x+this->m_visibleSize.width*0.5,this->m_visibleOrigin.y+360.0f+80.0f*3.0f);
    //    clipNode->addChild(dibu);
    //
    //    auto stencil = Sprite::create("Map/DDLFT_ditu.png");//上面的图形
    //  //  stencil->setTextureRect(Rect(0,0,720,720));
    //    stencil->setPosition(this->m_visibleOrigin.x+this->m_visibleSize.width*0.5,this->m_visibleOrigin.y+360.0f+80.0f*3.0f);
    //    clipNode->setStencil(stencil);
    // AnimationCache::getInstance()->addAnimation(<#cocos2d::Animation *animation#>, <#const std::string &name#>)
    
    //点击精灵 事件监听
    m_listen = EventListenerTouchOneByOne::create();
    m_listen->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->m_listen,2);//比 ui图层 优先级低(0 是UI图层的)
    m_ttqpath.push_back("ttq/ttq_1.png");
    m_ttqpath.push_back("ttq/ttq_2.png");
    m_ttqpath.push_back("ttq/ttq_3.png");
    m_ttqpath.push_back("ttq/ttq_4.png");
    
    m_icepath.push_back("ttq/BINGTANG1.png");
    m_icepath.push_back("ttq/BINGTANG2.png");
    m_icepath.push_back("ttq/BINGTANG3.png");
    
    m_cltMove.push_back("ttq/chocolate cream shang.png");
    m_cltMove.push_back("ttq/chocolate cream xia.png");
    m_cltMove.push_back("ttq/chocolate cream zuo.png");
    m_cltMove.push_back("ttq/chocolate cream you.png");
    
    m_cltPath.push_back("ttq/chocolate1.png");
    m_cltPath.push_back("ttq/chocolate2.png");
    m_cltPath.push_back("ttq/chocolate3.png");
    m_cltPath.push_back("ttq/chocolate4.png");
    m_cltPath.push_back("ttq/chocolate5.png");
    
    m_uipath.push_back("Map/DDLFT_ditu.png");
    
    m_uipath.push_back("Animation/SKT-BINGKUAI/SKT-BING-bingtangtexiao.png");
    m_uipath.push_back("Animation/SKT-BINGKUAI/SKT-BING-texiao2.png");
    
    //加载图片缓存
    std::vector<std::vector<std::string>> path;
    path.push_back(m_ttqpath);
    path.push_back(m_icepath);
    path.push_back(m_cltMove);
    path.push_back(m_cltPath);
    path.push_back(m_uipath);
    int * index = new int(0);
    auto itr = path.begin();
    for(;itr!=path.end();itr++){
        *index=*index+(int)(*itr).size();
    }
    itr = path.begin();
    for(;itr!=path.end();itr++){
        for(int j =0;j<(*itr).size();j++){
            Director::getInstance()->getTextureCache()->addImageAsync((*itr).at(j),[=](Texture2D* texture)mutable{
                --(*index);
                if((*index)==0){
                    delete index;
                    index = nullptr;
                    this->initUI();
                }
            });
        }
    }
//    //初始化九宫格RECT
//    for(int i =0;i<9;i++){
//        for(int j = 0;j<9;j++){
//            m_ArrayRect[i][j]=Rect(this->m_visibleOrigin.x+i*(80.0f),j*80+240,80,80);
//        }
//    }
//    //初始化九宫格位置
//    for(int i =0;i<9;i++){
//        for(int j = 0;j<9;j++){
//            m_ArrayVec2[i][j]=Vec2(this->m_visibleOrigin.x+(i+0.5)*80,(j+0.5)*80+240);
//        }
//    }
}
//点触摸 监听回调
bool GameLayer::onTouchBegan(Touch * touch,Event* event){//mutable
    if(!this->isCanTouch()){ //是否可点
        return false;
    }
    Vec2 location = touch->getLocation();
    this->m_intEliminateNum=0; //重置 连消计数
    if(m_usePropType!=none){ //  使用道具
        Vec2 vec = location;
        vec = this->positionConverToIndex(vec);
        auto sp = this->getChild(vec.x, vec.y);
        if(m_usePropType==fire){ //使用火把
            auto node = CSLoader::createNode("Animation/SKT-XHB-dianran/Layer.csb");
            node->setIgnoreAnchorPointForPosition(false);
            node->setAnchorPoint(Vec2(0.5,0.5));
            this->addChild(node);
            node->setPosition(Vec2(this->indexConverToPosition(vec).x,this->indexConverToPosition(vec).y));
            auto timeline = CSLoader::createTimeline("Animation/SKT-XHB-dianran/Layer.csb");
            node->runAction(timeline);
            timeline->play("animation1",false);
            timeline->setAnimationEndCallFunc("animation1",[=](){
                node->removeFromParent();
            });
            if(sp==nullptr){//火把什么也没点到
                return false;
            }
            if(!sp->getName().compare("ttq")){
                this->deleteChild(sp);
            }else if(!sp->getName().compare("ice")){ //减一层冰块
                static_cast<Sprite_Ice*>(sp)->alterIceLayerNum(-1);//减一层冰
            }
        }
        else if(sp==nullptr){//点在空格上 并且 不是使用火把    （只能点在空格上的道具）
            if(m_usePropType==bomb){
                //爆炸特效
                auto bomb = Sprite::create();
                this->addChild(bomb);
                bomb->setPosition(Vec2(this->indexConverToPosition(vec).x,this->indexConverToPosition(vec).y));
                bomb->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("bomb")),RemoveSelf::create(),nullptr));
                std::function<void(Sprite* sptemp)> func =[=](Sprite* sptemp){
                    if(sptemp!=nullptr){
                        if(!sptemp->getName().compare("ttq")){
                            sptemp->setTag(100);
                            alteredState(sptemp);
                        }else{
                            this->deleteChild(sptemp);
                        }
                    }
                };
                auto spTemp = this->getChild(vec.x,vec.y+1);//上部
                func(spTemp);
                spTemp = this->getChild(vec.x,vec.y-1);//上部
                func(spTemp);
                spTemp = this->getChild(vec.x+1,vec.y);//上部
                func(spTemp);
                spTemp = this->getChild(vec.x-1,vec.y);//上部
                func(spTemp);
                //4个角落
                spTemp = this->getChild(vec.x-1,vec.y+1);//
                func(spTemp);
                spTemp = this->getChild(vec.x-1,vec.y-1);//
                func(spTemp);
                spTemp = this->getChild(vec.x+1,vec.y+1);//
                func(spTemp);
                spTemp = this->getChild(vec.x+1,vec.y-1);//
                func(spTemp);
            }
            if(m_usePropType==jelly){
                Director::getInstance()->getTextureCache()->addImage("ttq/GD.png");
                this->addElement("ttq/GD.png", 0, vec.x,vec.y, "gd");
            }
            if(m_usePropType==magicmirror){
                Director::getInstance()->getTextureCache()->addImage("Animation/Magicmirror/hahajing0.png");
                this->addElement("Animation/Magicmirror/hahajing0.png", 0, vec.x,vec.y, "magicmirror");
            }
        }
        return false;
    }
    //不在使用道具状态
    Vec2 index = positionConverToIndex(location);
    auto sp = getChild(index.x,index.y);
    if(nullptr==sp ||m_intClickNum<=0 ){  // 点在空格上或者  没有点击数了
        return false;
    }
    if(!sp->getName().compare("ttq")||!sp->getName().compare("ice")){
        if(!sp->getName().compare("ice")){
            static_cast<Sprite_Ice*>(sp)->alterClickNum(1); // 可点数减一
        }else{
            this->alteredState(sp);
        }
        m_intClickNum--;    //点击数减一
        m_labelClickNum->setString(StringUtils::format("%d",m_intClickNum));
        return false;
    }
    
    return false;
}
void GameLayer::initUI(){ //初始化UI界面
    auto bgg = Sprite::create("Map/bg.png");
    bgg->setPosition(Vec2(m_visibleOrigin.x+m_visibleSize.width*0.5,m_visibleOrigin.y+m_visibleSize.height*0.5));
    this->addChild(bgg);
    auto bg = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("Map/DDLFT_ditu.png"));
    this->addChild(bg);
    bg->setPosition(Vec2(this->m_visibleOrigin.x+this->m_visibleSize.width*0.5,this->m_visibleOrigin.y+m_visibleSize.height*0.5-12));
    m_labelClickNum = Label::createWithTTF(StringUtils::format("%d",m_intClickNum), "fonts/Marker Felt.ttf", 48);
    this->addChild(m_labelClickNum);
    m_labelClickNum->setPosition(Vec2(m_visibleSize.width*0.5+m_visibleOrigin.x,m_visibleSize.height*0.9+m_visibleOrigin.y));
    
    btn_Bomb = MenuItemFont::create("bomb", CC_CALLBACK_1(GameLayer::btnCallBack,this,"bomb"));
    btn_Bomb->setPosition(Vec2(m_visibleOrigin.x+m_visibleSize.width*0.1,m_visibleSize.height*0.1+m_visibleOrigin.y));
    
    btn_SweetMagic = MenuItemFont::create("SweetMagic", CC_CALLBACK_1(GameLayer::btnCallBack,this,"sweetMagic"));
    btn_SweetMagic->setPosition(Vec2(m_visibleOrigin.x+m_visibleSize.width*0.3,m_visibleSize.height*0.1+m_visibleOrigin.y));
    
    btn_jelly = MenuItemFont::create("Jelly", CC_CALLBACK_1(GameLayer::btnCallBack,this,"Jelly"));
    btn_jelly->setPosition(Vec2(m_visibleOrigin.x+m_visibleSize.width*0.5,m_visibleSize.height*0.1+m_visibleOrigin.y));
    
    btn_magicmirror = MenuItemFont::create("magicmirror", CC_CALLBACK_1(GameLayer::btnCallBack,this,"magicmirror"));
    btn_magicmirror->setPosition(Vec2(m_visibleOrigin.x+m_visibleSize.width*0.7,m_visibleSize.height*0.1+m_visibleOrigin.y));
    
    btn_fire = MenuItemFont::create("fire", CC_CALLBACK_1(GameLayer::btnCallBack,this,"fire"));
    btn_fire->setPosition(Vec2(m_visibleOrigin.x+m_visibleSize.width*0.9,m_visibleSize.height*0.1+m_visibleOrigin.y));
    
    auto menu = Menu::create(btn_Bomb,btn_SweetMagic,btn_jelly,btn_magicmirror,btn_fire,nullptr);
    this->addChild(menu);
    menu->setPosition(Vec2(m_visibleOrigin.x,m_visibleOrigin.y));
    
    //爆炸动画初始化
    //    auto animation =Animation::create();
    //    for(int i =1; i<27;i++){
    //        Rect rect = Rect::ZERO;
    //        auto texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("Animation/bomb/SKT-boomTEXIAO%d.png",i));
    //        rect.size = texture->getContentSize();
    //        SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
    //        animation->addSpriteFrame(frame);
    //    }
    //    AnimationCache::getInstance()->addAnimation(animation,"bomb");
    //    animation->setDelayPerUnit(1.0f/26.0f);
    
    //哈哈镜动画初始化
    //    auto magicmirror =Animation::create();
    //    for(int i =0; i<3;i++){
    //        Rect rect = Rect::ZERO;
    //        auto texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("Animation/Magicmirror/hahajing%d.png",i));
    //        rect.size = texture->getContentSize();
    //        SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
    //        magicmirror->addSpriteFrame(frame);
    //    }
    //    AnimationCache::getInstance()->addAnimation(magicmirror,"magicmirror");
    //    magicmirror->setDelayPerUnit(0.3f/3.0f);
    //    magicmirror->setRestoreOriginalFrame(true);
    //
    //火把 初始化
    //    m_nodeFire = CSLoader::createNode("Animation/SKT-XHB-dianran/Layer.csb");
    //    m_nodeFire->setIgnoreAnchorPointForPosition(false);
    //    m_nodeFire->setAnchorPoint(Vec2(0.5,0.5));
    //    this->addChild(m_nodeFire);
    //    m_fireTimeline = CSLoader::createTimeline("Animation/SKT-XHB-dianran/Layer.csb");
    //    m_nodeFire->runAction(m_fireTimeline);
    //    m_nodeFire->setVisible(false);
    
    //    auto node = Sprite::create();
    //    this->addChild(node,100);
    //    node->runAction(RepeatForever::create(Animate::create(magicmirror)));
    //    node->setPosition(m_visibleOrigin.x+m_visibleSize.width*0.5,m_visibleSize.height*0.5+m_visibleOrigin.y);
    
    // m_fireTimeline->play("animation0", true);
    
    this->initData();
    this->scheduleUpdate();
    //this->schedule(schedule_selector(GameLayer::slowUpdate), 0.05, CC_REPEAT_FOREVER, 0); // 开启update
}
void GameLayer::initAnimation(const char*  formatPath,const int & starIndex,const int & length,const float & perUnit,const std::string & animationName,const bool & restoreOriginalFrame){
    auto animation =Animation::create();
    for(int i =starIndex; i<length;i++){
        Rect rect = Rect::ZERO;
        auto texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format(formatPath,i));
        rect.size = texture->getContentSize();
        SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
        animation->addSpriteFrame(frame);
    }
    AnimationCache::getInstance()->addAnimation(animation,animationName);
    animation->setDelayPerUnit(perUnit);
    animation->setRestoreOriginalFrame(restoreOriginalFrame);
}
bool GameLayer::isCanTouch(){
    if(this->m_vector_moveSp.size()>0 ||(m_layer_gameOver&& m_layer_gameOver->isVisible())||this->m_intClickNum<=0){
        return false;
    }
    return true;
}
void GameLayer::btnCallBack(Ref* ref,const std::string& name){
    if(!this->isCanTouch()){ //是否可点
        return;
    }
    if(!name.compare("bomb")){  //炸弹
        //检测动画 初始化
        if(nullptr==AnimationCache::getInstance()->getAnimation("bomb")){
            this->initAnimation("Animation/bomb/SKT-boomTEXIAO%d.png",1,27,1.0f/27.0f,"bomb",false);
        }
        if(m_usePropType==bomb){
            m_usePropType=none;
        }else{
            m_usePropType = bomb;
        }
    }else if(!name.compare("sweetMagic")){
        m_usePropType = none;
        if(sweetMagicNode==nullptr){
            sweetMagicNode= CSLoader::createNode("Animation/SKT-TMF/Layer.csb");
            this->addChild(sweetMagicNode);
            sweetMagicTimeLine = CSLoader::createTimeline("Animation/SKT-TMF/Layer.csb");
            sweetMagicNode->runAction(sweetMagicTimeLine);
            sweetMagicTimeLine->play("animation0", false);
            sweetMagicTimeLine->setAnimationEndCallFunc("animation0", [=](){
                sweetMagicNode->setVisible(false);
            });
        }else{
            sweetMagicNode->setVisible(true);
            sweetMagicTimeLine->play("animation0", false);
        }
        this->m_intEliminateNum=0;  //重置 连消计数
        for(auto temp : this->m_mapSprite){
            for(auto mapSecond :temp.second){
                if(!mapSecond.second->getName().compare("ttq")){
                    this->alteredState(mapSecond.second);
                }
            }
        }
    }else if(!name.compare("Jelly")){
        if(m_usePropType==jelly){
            m_usePropType=none;
        }else{
            m_usePropType = jelly;
        }
    }else if(!name.compare("magicmirror")){//哈哈镜
        //检测动画 初始化
        if(nullptr == AnimationCache::getInstance()->getAnimation("magicmirror")){
            this->initAnimation("Animation/Magicmirror/hahajing%d.png",0,3,0.3f/3.0f,"magicmirror",true);
        }
        if(m_usePropType==magicmirror){
            m_usePropType=none;
        }else{
            m_usePropType = magicmirror;
        }
    }else if(!name.compare("fire")){
        if(m_usePropType==fire){
            m_usePropType=none;
        }else{
            m_usePropType = fire;
            if(m_nodeFire==nullptr){//按钮处火把初始化
                m_nodeFire = CSLoader::createNode("Animation/SKT-XHB-dianran/Layer.csb");
                m_nodeFire->setIgnoreAnchorPointForPosition(false);
                m_nodeFire->setAnchorPoint(Vec2(0.5,0.5));
                this->addChild(m_nodeFire);
                m_fireTimeline = CSLoader::createTimeline("Animation/SKT-XHB-dianran/Layer.csb");
                m_nodeFire->runAction(m_fireTimeline);
            }
            m_nodeFire->setVisible(true);
            m_nodeFire->setPosition(btn_fire->getPosition());
            m_fireTimeline->play("animation0", true);
        }
    }
    btn_Bomb->setScale(1.0f);
    btn_jelly->setScale(1.0f);
    btn_magicmirror->setScale(1.0f);
    btn_fire->setScale(1.0f);
    if(nullptr!=m_nodeFire){
        m_nodeFire->setVisible(false);
    }
    if(m_fireTimeline!=nullptr&&m_fireTimeline->isPlaying()){
        m_fireTimeline->stop();
    }
    if(m_usePropType==bomb){
        btn_Bomb->setScale(1.5f);
    }else if(m_usePropType==jelly){
        btn_jelly->setScale(1.5f);
    }else if(m_usePropType==magicmirror){
        btn_magicmirror->setScale(1.5f);
    }else if(m_usePropType==fire){
        btn_fire->setScale(1.5f);
    }
}
void GameLayer::readJson(const std::string & path,const int & id){
    rapidjson::Document readdoc;
    std::string load_str = FileUtils::getInstance()->getStringFromFile(path);
    readdoc.Parse<0>(load_str.c_str());
    //auto object = readdoc[StringUtils::format("%d",id).c_str()][0];
    //数据先置为0
    m_intClickNum =0;
    for(int i =0; i<sizeof(m_ArrayNum)/sizeof(m_ArrayNum[0]);i++){
        m_ArrayNum[i] = 0;
    }
    //赋值
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("0")){
        m_intClickNum = readdoc[StringUtils::format("%d",id).c_str()]["0"].GetInt();
        m_labelClickNum->setString(StringUtils::format("%d",m_intClickNum));
    }
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("1")){
        m_ArrayNum[0] = readdoc[StringUtils::format("%d",id).c_str()]["1"].GetInt();
    }
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("2")){
        m_ArrayNum[1] = readdoc[StringUtils::format("%d",id).c_str()]["2"].GetInt();
    }
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("3")){
        m_ArrayNum[2] = readdoc[StringUtils::format("%d",id).c_str()]["3"].GetInt();
    }
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("4")){
        m_ArrayNum[3] = readdoc[StringUtils::format("%d",id).c_str()]["4"].GetInt();
    }
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("5")){ //冰块
        m_ArrayNum[4] = readdoc[StringUtils::format("%d",id).c_str()]["5"].GetInt();
    }
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("6")){ //小恶魔
        m_ArrayNum[5] = readdoc[StringUtils::format("%d",id).c_str()]["6"].GetInt();
    }
    if(readdoc[StringUtils::format("%d",id).c_str()].HasMember("7")){ //巧克力
        m_ArrayNum[6] = readdoc[StringUtils::format("%d",id).c_str()]["7"].GetInt();
    }
}
void GameLayer::getEmptyIndex(int* index,const std::string & ignoreName){
    index[0] = CCRANDOM_0_1()*9;
    index[1] = CCRANDOM_0_1()*9;
    //格子上有  物品  并且 名字不是  忽略名
    if(this->getChild(index[0], index[1])!=nullptr&&this->getChild(index[0], index[1])->getName().compare(ignoreName)){//随机出的位置有东西
        int count = 0;
        while(true){
            //向上
            int tempIndex1= index[0];
            int tempIndex2= index[1]+count;
            tempIndex2 = count % 9;
            tempIndex1+=(count/9);
            if(tempIndex1>=0&&tempIndex1<=8&&tempIndex2>=0&&tempIndex2<=8&&(this->getChild(tempIndex1, tempIndex2)==nullptr||!this->getChild(tempIndex1, tempIndex2)->getName().compare(ignoreName))){
                index[0] = tempIndex1;
                index[1] = tempIndex2;
                break;
            }
            //向下
            tempIndex1= index[0];
            tempIndex2= index[1]-count;
            tempIndex2 = count % 9;
            tempIndex1 = (count/9);
            if(tempIndex1>=0&&tempIndex1<=8&&tempIndex2>=0&&tempIndex2<=8&&(this->getChild(tempIndex1, tempIndex2)==nullptr||!this->getChild(tempIndex1, tempIndex2)->getName().compare(ignoreName))){
                index[0] = tempIndex1;
                index[1] = tempIndex2;
                break;
            }
            count++;
        }
    }
}
cocos2d::Sprite* GameLayer::addElement(const std::string & path,const int & tag,const unsigned short& index1,const unsigned short &index2,const std::string & name){
    if(index1>8||index2>8||index1<0||index2<0){
        return nullptr;
    }
    SpriteNew* sp = nullptr;
    if(!name.compare("ice")){
        sp = Sprite_Ice::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(path));
    }else{
        sp = SpriteNew::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(path));
    }
    this->addChild(sp);
    Vec2 position = Vec2(index1,index2);
    sp->setPosition(this->indexConverToPosition(position));
    sp->setIndex(index1, index2);
    sp->setTag(tag);
    sp->setName(name);
    if(m_mapSprite.find(index1)==m_mapSprite.end()){//map中不存在这个对象
        cocos2d::Map<int,Sprite*> tempSprite;
        tempSprite.insert(index2,sp);
        m_mapSprite.insert(std::map<int,cocos2d::Map<int,Sprite*>>::value_type(index1,tempSprite));
    }else{   //map中存在这个对象
        (*m_mapSprite.find(index1)).second.insert(index2,sp);
    }
    if(!name.compare("ttq")||!name.compare("ice")){
        m_intTtqNum++;
    }else if(!name.compare("clt")){ //添加的是巧克力
        m_intCltNum++;
        if(m_intCltNum>=maxCltNum){
            this->GameOver(false); //游戏结束
        }
    }
    return sp;
}
Vec2 GameLayer::indexConverToPosition(Vec2 & vec){ //返回格子中点
   return vec = Vec2(this->m_visibleOrigin.x+(vec.x+0.5)*80,(vec.y+0.5)*80+240);
}
Vec2 GameLayer::positionConverToIndex( Vec2 & vec){
    vec.x = int(vec.x-this->m_visibleOrigin.x)/80;
    vec.y = int(vec.y-240-this->m_visibleOrigin.y)/80;
    return vec;
}
void GameLayer::initData(){
     //   m_intFourNum=3;
    //    m_intOneNum=3;
    //    m_intTwoNum = 3;
    //    m_intThreeNum =3;
    //    m_intChocolatesNum=0;
     m_ArrayNum[3] =5;
     m_intClickNum =100;
    //    m_labelClickNum->setString(StringUtils::format("%d",m_intClickNum));
   // this->readJson("configure/configure.json",m_intlevel);
    //CCLOG("%d",m_intOneNum+m_intTwoNum+m_intThreeNum+m_intFourNum+ m_intIceNum+m_intDevilNum+m_intChocolatesNum);
    //初始化地图 精灵
    int positionIndex[2] = {0,0};
    for(int i= 0; i<m_ArrayNum[0];i++){
        getEmptyIndex(&(positionIndex[0]));
        this->addElement(m_ttqpath[0],0, positionIndex[0],positionIndex[1],"ttq");
    }
    for(int i = 0; i<m_ArrayNum[1];i++){
        getEmptyIndex(&(positionIndex[0]));
        this->addElement(m_ttqpath[1],1,positionIndex[0],positionIndex[1],"ttq");
    }
    for(int i =0; i<m_ArrayNum[2];i++){
        getEmptyIndex(&(positionIndex[0]));
        this->addElement(m_ttqpath[2],2, positionIndex[0],positionIndex[1],"ttq");
    }
    for(int i =0; i<m_ArrayNum[3];i++){
        getEmptyIndex(&(positionIndex[0]));
        this->addElement(m_ttqpath[3],3, positionIndex[0],positionIndex[1],"ttq");
    }
    for(int i =0; i<m_ArrayNum[4];i++){  //冰糖里面有ttq
        //可以有ttp
        getEmptyIndex(&(positionIndex[0]),"ttq");
        auto ttq = getChild(positionIndex[0], positionIndex[1]);
        auto ttqLayerNum = 0;
        if(nullptr!=ttq){
            ttqLayerNum = ttq->getTag()+1;
            this->deleteChild(ttq);
        }
        auto ice = this->addElement(m_icepath[0],0, positionIndex[0],positionIndex[1],"ice");//不采用tag
       // static_cast<Sprite_Ice*>(ice)->setIceLayerNum(1); //冰层数
        static_cast<Sprite_Ice*>(ice)->alterTtqLayerNum(ttqLayerNum); //甜甜圈 数量
    }
    for(int i =0; i<m_ArrayNum[5];i++){   //
        getEmptyIndex(&(positionIndex[0]));
        // this->addElement(m_ttqpath[0],9, positionIndex[0],positionIndex[1],"devil");
    }
    for(int i =0;i<m_ArrayNum[6];i++){
        getEmptyIndex(&(positionIndex[0]));
        this->addElement(m_cltPath[0],0, positionIndex[0],positionIndex[1],"clt");
    }
}
cocos2d::Sprite* GameLayer::getChild(const int & index1,const int& index2){
    auto itr1 =this->m_mapSprite.find(index1);
    if(itr1!=this->m_mapSprite.end()){
        auto itr2 = (*itr1).second.find(index2);
        if(itr2 != (*itr1).second.end()){
            return (*itr2).second;
        }
    }
    return nullptr;
}
void GameLayer::deleteChild(const int & index1,const int & index2){
    auto sp = this->getChild(index1, index2);
    if(sp!=nullptr){
        this->deleteChild(sp);
    }
}
void GameLayer::deleteChild(cocos2d::Sprite*  sp){
    if(sp==nullptr){
        return;
    }
    (*(this->m_mapSprite.find((static_cast<SpriteNew*>(sp)->getIndex())[0]))).second.erase((static_cast<SpriteNew*>(sp)->getIndex())[1]);//删除sp
    auto name = sp->getName();
    if(!name.compare("ice")&&static_cast<Sprite_Ice*>(sp)->getTtqLayerNum()>0){
        this->addElement(m_ttqpath[static_cast<Sprite_Ice*>(sp)->getTtqLayerNum()-1],static_cast<Sprite_Ice*>(sp)->getTtqLayerNum()-1, (static_cast<SpriteNew*>(sp)->getIndex())[0],(static_cast<SpriteNew*>(sp)->getIndex())[1],"ttq");
    }
    sp->removeFromParent();
    sp=nullptr;
    if(!name.compare("clt")){
        CC_ASSERT(m_intCltNum>0);
        m_intCltNum--;
    }
    //检测游戏是否结束
    if(!name.compare("ttq")||!name.compare("ice")){
        m_intTtqNum--;
        if(m_intTtqNum<=0){
            this->GameOver(true);
        }
    }
}
void GameLayer::chocolateCollision(const int index1,const int index2){
    if(index1<0||index1>8||index2<0||index2>8){
        return;
    }
    auto sp = this->getChild(index1,index2);
    if(sp!=nullptr){
        if(!sp->getName().compare("ttq")){//巧克力遇到甜甜圈
            auto data =  static_cast<SpriteNew*>(sp)->getIndex();
            deleteChild(sp);
            this->addElement(m_cltPath[0],0,data[0],data[1],"clt");
            
        }else if(!sp->getName().compare("devil"))//遇到小恶魔
        {
            
        }else if(!sp->getName().compare("ice")){//遇到冰块
            
        }
    }else{
        this->addElement(m_cltPath[0], 0,index1,index2,"clt");
    }
}
//void GameLayer::alterIceStatus(cocos2d::Sprite* & ice,const int & clickNum,const int & ttqNum){
//    int totalttqNum = static_cast<Sprite_Ice*>(ice)->getTtqLayerNum()+ttqNum;
//    auto addIceNum = totalttqNum/5; //5个甜甜圈 加一层冰
//    if(addIceNum+static_cast<Sprite_Ice*>(ice)->getIceLayerNum()>3){  // 冰层少于等于三层
//        addIceNum = 3 - static_cast<Sprite_Ice*>(ice)->getIceLayerNum();
//        static_cast<Sprite_Ice*>(ice)->setTtqLayerNum(4);
//    }else{
//        static_cast<Sprite_Ice*>(ice)->setTtqLayerNum(totalttqNum%5);
//    }
//    //新的点击数
//    auto newClickNum = (static_cast<Sprite_Ice*>(ice)->getIceLayerNum()+addIceNum)*5 +static_cast<Sprite_Ice*>(ice)->getIceClickNum()-clickNum;
//    //新的冰层数
//    auto newIceNum =  newClickNum/5;
//    newClickNum = newClickNum%5;
//    if(newIceNum<1){   //冰层数  不少于1
//        deleteChild(ice);
//        return;
//    }
//    auto child =static_cast<Sprite*>(ice->getChildByName("ttq"));
//    child->setVisible(false);
//    auto newTtqNum = static_cast<Sprite_Ice*>(ice)->getTtqLayerNum();
//    if(newTtqNum>0){
//        child->setVisible(true);
//        child->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_ttqpath[newTtqNum-1]));
//    }
//    ice->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_icepath[newIceNum-1]));
//    static_cast<Sprite_Ice*>(ice)->setIceLayerNum(newIceNum);
//}
void GameLayer::update(float dt){
    if(this->m_vector_moveSp.size()<=0||(m_layer_gameOver&&m_layer_gameOver->isVisible())){//检测游戏状态
        if(m_intClickNum<=0){
            this->GameOver(false);
        }
        return;
    }
    //检测移动的 碰撞
    Vector<Sprite*>::iterator itrMoveSp = this->m_vector_moveSp.begin();
    bool needBreak = false;
    for(;itrMoveSp<this->m_vector_moveSp.end();itrMoveSp++){
        if(!(*itrMoveSp)->getName().compare("cltMove")){  //移动的是 巧克力 保留
            continue;
        }
        Vec2 vec = (*itrMoveSp)->getPosition();
        vec = this->positionConverToIndex(vec);
        auto sp = this->getChild(vec.x, vec.y);
        if(sp==nullptr){    //没有碰到物品
            continue;
        }
        if(!sp->getName().compare("ice")){ //移动 的 糖块 遇到冰块
            if(static_cast<Sprite_Ice*>(sp)->getIceLayerNum()>=3 && static_cast<Sprite_Ice*>(sp)->getTtqLayerNum()>=4){
                continue;
            }
            static_cast<Sprite_Ice*>(sp)->alterTtqLayerNum(1);
        }
        else if(!sp->getName().compare("devil")){  //小糖块遇到 小恶魔
            sp->setTag(sp->getTag()+1);
            if(sp->getTag()>=10){
                this->deleteChild(sp);
            }
        }else if(!sp->getName().compare("clt")){ // 遇到的是巧克力
            this->alteredClt(sp);
        }else if(!sp->getName().compare("gd")){
            auto spMove =static_cast<SpriteMove*>(*itrMoveSp);
            spMove->stopAllActions();
            Rect  moveRect = spMove->getBoundingBox();
            Rect  spRect = sp->getBoundingBox();
            if(spMove->getDirection()=='w'){
                vec.y-=1;
                indexConverToPosition(vec);
                vec.y+=39;
                //vec = this->indexConverToPosition(this->indexConverToPosition(spMove));
                spMove->setPosition(vec);//spMove->getPositionY()-(moveRect.origin.y+moveRect.size.height-spRect.origin.y)-1);//下移
                spMove->setDirection('s');
            }else if(spMove->getDirection()=='s'){
                vec.y+=1;
                indexConverToPosition(vec);
                vec.y-=39;
                spMove->setPosition(vec); ///Y(spMove->getPositionY()+spRect.origin.y+spRect.size.height-moveRect.origin.y+1);//上移
                spMove->setDirection('w');
            }else if(spMove->getDirection()=='a'){
                vec.x+=1;
                indexConverToPosition(vec);
                vec.x-=39;
                spMove->setPosition(vec);
                //spMove->setPositionX(spMove->getPositionX()+spRect.origin.x+spRect.size.width-moveRect.origin.x+1);
                spMove->setDirection('d');
            }else if(spMove->getDirection()=='d'){
                vec.x-=1;
                indexConverToPosition(vec);
                vec.x+=39;
                spMove->setPosition(vec);
               // spMove->setPositionX(spMove->getPositionX()-(moveRect.origin.x+moveRect.size.width-spRect.origin.x)-1);
                spMove->setDirection('a');
            }
            CCASSERT((*itrMoveSp)->getBoundingBox().intersectsRect(sp->getBoundingBox()), "error Calculation");
            static_cast<SpriteMove*>(spMove)->moveAction([=](){m_vector_moveSp.eraseObject(spMove);});
            needBreak = true;
            break;
        }else if(!sp->getName().compare("magicmirror")){//遇到哈哈镜
            auto spMove =static_cast<SpriteMove*>(*itrMoveSp);
            spMove->stopAllActions();
            Rect  moveRect = spMove->getBoundingBox();
            Rect  spRect = sp->getBoundingBox();
            int i= (static_cast<SpriteNew*>(sp)->getIndex())[0];
            int j= (static_cast<SpriteNew*>(sp)->getIndex())[1];
            Vec2 position;
            if(spMove->getDirection()=='w'){
                spMove->setDirection('d');
                position = Vec2(i,j-1);
                position = this->indexConverToPosition(position);
                spMove->setPosition(position);//Vec2(m_ArrayRect[i][j-1].origin.x+40,m_ArrayRect[i][j-1].origin.y+40));
            }else if(spMove->getDirection()=='d'){
                spMove->setDirection('s');
                position = Vec2(i-1,j);
                position = this->indexConverToPosition(position);
                spMove->setPosition(position);
               
            }else if(spMove->getDirection()=='s'){
                spMove->setDirection('a');
                position = Vec2(i,j+1);
                position = this->indexConverToPosition(position);
                spMove->setPosition(position);
               
            }else if(spMove->getDirection()=='a'){
                spMove->setDirection('w');
                position = Vec2(i+1,j);
                position = this->indexConverToPosition(position);
                spMove->setPosition(position);
               
            }
            sp->setTag(sp->getTag()+1);
          //  CCASSERT((*itrMoveSp)->getBoundingBox().intersectsRect(sp->getBoundingBox()), "error Calculation");
            if(sp->getTag()>=5){
                sp->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("magicmirror")),CallFunc::create([=]()mutable{
                //    this->deleteChild(sp);
                }),nullptr));
            }else{
                sp->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("magicmirror")));
            }
            static_cast<SpriteMove*>(spMove)->moveAction([=](){m_vector_moveSp.eraseObject(spMove);});
            needBreak = true;
            break;
        }
        else{
            this->alteredState(sp);
        }
        (*itrMoveSp)->removeFromParent();
        this->m_vector_moveSp.eraseObject(*itrMoveSp);
        --itrMoveSp;
    }
}
void GameLayer::GameRestart(Ref* ref){
    Vector<Sprite*>::iterator itr=this->m_vector_moveSp.begin();
    for(;itr<this->m_vector_moveSp.end();itr++){
        (*itr)->removeFromParent();
        this->m_vector_moveSp.eraseObject(*itr);
        itr--;
    }
    for(auto temp : this->m_mapSprite){
        for(auto mapSecond :temp.second){
            this->deleteChild(mapSecond.second);
        }
    }
    if(this->m_layer_gameOver){
        static_cast<MenuItemFont*>(this->m_layer_gameOver->getChildByName("Menu")->getChildByName("overBtn"))->setEnabled(false);
        this->m_layer_gameOver->setVisible(false);
    }
    this->initData();
}
void GameLayer::GameOver(const bool & victory){
    if(m_layer_gameOver!=nullptr&&m_layer_gameOver->isVisible()==true){
        return;
    }
    std::string vectorStr= "";
    if(victory==true){
        vectorStr="Successful";
        m_intlevel+=1;
    }else{
        vectorStr="GameOver";
    }
    if(this->m_layer_gameOver){
        this->m_layer_gameOver->setVisible(true);
        static_cast<MenuItemFont*>(this->m_layer_gameOver->getChildByName("Menu")->getChildByName("overBtn"))->setEnabled(true);
        static_cast<MenuItemFont*>(this->m_layer_gameOver->getChildByName("Menu")->getChildByName("overBtn"))->setString(vectorStr);
    }else{
        this->m_layer_gameOver= Layer::create();
        auto gameOverBtn = MenuItemFont::create(vectorStr,CC_CALLBACK_1(GameLayer::GameRestart,this));
        gameOverBtn->setName("overBtn");
        gameOverBtn->setColor(Color3B(255,0,0));
        gameOverBtn->setScale(2.5f);
        gameOverBtn->setPosition(m_visibleOrigin.x+m_visibleSize.width*0.5,+m_visibleOrigin.y+m_visibleSize.height*0.5);
        auto menu = Menu::create(gameOverBtn,NULL);
        menu->setName("Menu");
        menu->setPosition(Vec2(m_visibleOrigin.x,m_visibleOrigin.y));
        this->m_layer_gameOver->addChild(menu);
        this->addChild(this->m_layer_gameOver,1);
    }
    
}
//检测巧克力状态
void GameLayer::alteredClt(cocos2d::Sprite* &sp){
    int tag = sp->getTag();
    tag++;
    if(tag>=m_cltPath.size()){
        (*(this->m_mapSprite.find((static_cast<SpriteNew*>(sp)->getIndex())[0]))).second.erase((static_cast<SpriteNew*>(sp)->getIndex())[1]);//删除sp
        for(int i = 0; i<4;i++){  //向4个反向 发射 4个sp
            int index1 = ((static_cast<SpriteNew*>(sp)->getIndex())[0]);
            int index2 = ((static_cast<SpriteNew*>(sp)->getIndex())[1]);
            Vec2 postion;
            std::string path;
            if(i==0){//向上飞
                path = m_cltMove[0];
                postion = Vec2(sp->getPositionX(),1.0f*80.0f+this->m_visibleOrigin.y+sp->getPositionY());
                index2+=1;
            }else if(i==1){  //向下飞
                path = m_cltMove[1];
                postion = Vec2(sp->getPositionX(),-1.0f*80.0f+this->m_visibleOrigin.y+sp->getPositionY());
                index2-=1;
            }else if(i==2){  //向左飞
                path = m_cltMove[2];
                postion = Vec2(-80.0f+sp->getPositionX()+this->m_visibleOrigin.x,sp->getPositionY());
                index1-=1;
            }else{//向右飞
                path = m_cltMove[3];
                postion = Vec2(1.0f*80.0f+sp->getPositionX()+this->m_visibleOrigin.x,sp->getPositionY());
                index1+=1;
            }
            auto spMove = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(path));
            this->addChild(spMove);
            spMove->setPosition(sp->getPosition());
            this->m_vector_moveSp.pushBack(spMove);
            spMove->setName("cltMove");
            spMove->runAction(Sequence::create(MoveTo::create(0.3,postion),DelayTime::create(0.2f),CallFunc::create([=](){
                this->m_vector_moveSp.eraseObject(spMove);
                chocolateCollision(index1,index2);
            }),RemoveSelf::create(),NULL));  //延时0.2 秒移除
        }
        this->deleteChild(sp);
    }
    else{
        sp->setTag(tag);
        sp->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_cltPath[tag]));
    }
}
void GameLayer::alteredState(Sprite* &sp){
    int tag = sp->getTag();
    tag++;
    if(tag>=m_ttqpath.size()){ //tag 值大于 3
        this->m_intEliminateNum++;
        if(this->m_intEliminateNum>10){
            
        }else if(this->m_intEliminateNum>20){
            
        }else if(this->m_intEliminateNum>20){
            
        }
        for(int i = 0; i<4;i++){  //向4个反向 发射 4个sp
            auto spMove = SpriteMove::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_ttqpath[0]));
            spMove->setTag(1);
            //spMove->set
            this->addChild(spMove);
            spMove->setPosition(sp->getPosition());
            spMove->setScale(1/4.0f);
            Vec2 postion;
            if(i==0){//向上飞
                spMove->setDirection('w');
            }else if(i==1){  //向下飞
                spMove->setDirection('s');
                
            }else if(i==2){  //向左飞
                spMove->setDirection('a');
                
            }else{//向右飞
                spMove->setDirection('d');
            }
            m_vector_moveSp.pushBack(spMove);
            spMove->moveAction([=](){m_vector_moveSp.eraseObject(spMove);});
        }
        this->deleteChild(sp);
    }else{  //tag 值小于 4  tag + 1
        sp->setTag(tag);
        sp->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_ttqpath[tag]));
    }
}
void GameLayer::onExitTransitionDidStart(){
    Layer::onExitTransitionDidStart();
}
void GameLayer::onExit(){
    Layer::onExit();
    Director::getInstance()->getEventDispatcher()->removeEventListener(this->m_listen);//销毁 监听事件
}
void GameLayer::cleanup(){
    Layer::cleanup();
}

void GameLayer::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
