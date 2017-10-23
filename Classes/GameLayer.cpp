#include "GameLayer.h"
#include "AppDelegate.h"
#include "json/document.h"
#include "Sprite_New.h"
USING_NS_CC;

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
    m_listen->onTouchBegan = [=](Touch * touch,Event* event)mutable{
        if(this->m_vector_moveSp.size()>0 ||(m_layer_gameOver&& m_layer_gameOver->isVisible())){
            return false;
        }
        bool needBreak = false;
        Vec2 location = touch->getLocation();
        if(m_usePropType!=none){ //  使用道具
            for(int i = 0;i<9;i++){
                for(int j = 0;j<9;j++){
                    if(this->m_ArrayRect[i][j].containsPoint(location)){
                        auto sp = this->getChild(i, j);
                        if(m_usePropType==fire){ //使用火把
                            auto node = CSLoader::createNode("Animation/SKT-XHB-dianran/Layer.csb");
                            node->setIgnoreAnchorPointForPosition(false);
                            node->setAnchorPoint(Vec2(0.5,0.5));
                            this->addChild(node);
                            node->setPosition(Vec2(m_ArrayRect[i][j].origin.x+40,m_ArrayRect[i][j].origin.y+40));
                            auto timeline = CSLoader::createTimeline("Animation/SKT-XHB-dianran/Layer.csb");
                            node->runAction(timeline);
                            timeline->play("animation1",false);
                            timeline->setAnimationEndCallFunc("animation1",[=](){
                                node->removeFromParent();
                            });
                            if(sp==nullptr){
                                return false;
                            }
                            if(!sp->getName().compare("ttq")){
                                this->deleteChild(sp);
                            }else if(!sp->getName().compare("ice")){
                                auto tag = sp->getTag();
                                tag-= 100;
                                if(tag<100)//少于一层了
                                {
                                   this->deleteChild(sp);
                                }else{
                                   sp->setTag(tag);
                                    sp->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_icepath[tag/100 - 1]));
                                }
                            }else if(!sp->getName().compare("clt")){//点到巧克力
                                this->deleteChild(sp);
                            }
                        }
                        if(sp==nullptr){//点在空格上
                            this->m_intEliminateNum=0;
                            if(m_usePropType==bomb){
                                std::function<void(Sprite* sptemp)> func =[=](Sprite* sptemp){
                                    //爆炸特效
                                    auto bomb = Sprite::create();
                                    this->addChild(bomb);
                                    bomb->setPosition(Vec2(m_ArrayRect[i][j].origin.x+m_visibleOrigin.x+40,m_ArrayRect[i][j].origin.y+m_visibleOrigin.y+40));
                                    bomb->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("bomb")),RemoveSelf::create(),nullptr));
                                    
                                    if(sptemp!=nullptr){
                                        if(!sptemp->getName().compare("ttq")){
                                            sptemp->setTag(100);
                                            alteredState(sptemp);
                                        }else{
                                            this->deleteChild(sptemp);
                                        }
                                    }
                                };
                                auto spTemp = this->getChild(i,j+1);//上部
                                func(spTemp);
                                spTemp = this->getChild(i,j-1);//上部
                                func(spTemp);
                                spTemp = this->getChild(i+1,j);//上部
                                func(spTemp);
                                spTemp = this->getChild(i-1,j);//上部
                                func(spTemp);
                                //4个角落
                                spTemp = this->getChild(i-1,j+1);//
                                func(spTemp);
                                spTemp = this->getChild(i-1,j-1);//
                                func(spTemp);
                                spTemp = this->getChild(i+1,j+1);//
                                func(spTemp);
                                spTemp = this->getChild(i+1,j-1);//
                                func(spTemp);
                            }
                            if(m_usePropType==jelly){
                                int * positionIndex=new int[2];
                                positionIndex[0]=i;
                                positionIndex[1]=j;
                                Director::getInstance()->getTextureCache()->addImage("ttq/GD.png");
                                this->addElement("ttq/GD.png", 0, positionIndex, "gd");
                                delete [] positionIndex;
                            }
                            if(m_usePropType==magicmirror){
                                int * positionIndex=new int[2];
                                positionIndex[0]=i;
                                positionIndex[1]=j;
                                Director::getInstance()->getTextureCache()->addImage("Animation/Magicmirror/hahajing0.png");
                                this->addElement("Animation/Magicmirror/hahajing0.png", 0, positionIndex, "magicmirror");
                                delete [] positionIndex;
                            }if(m_usePropType==fire){
                                
                            }
                        }
                        
                        return false;
                    }
                }
            }
        }
        for(auto temp : this->m_mapSprite){
            for(auto mapSecond :temp.second){
                auto sp = mapSecond.second;
                if(sp->getBoundingBox().containsPoint(location)){
                    if(!sp->getName().compare("ttq")||!sp->getName().compare("ice")){
                        if(m_intClickNum<=0){
                            return false;
                        }
                        this->m_intEliminateNum=0;
                        if(!sp->getName().compare("ice")){
                            auto tag = sp->getTag();
                            tag-=10;
                            sp->setTag(tag);
                            if(tag<=100){
                                this->deleteChild(sp);
                            }
                            if(tag%100 -tag%10== 0&&tag>100){
                                tag-=50;
                                sp->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_icepath[tag/100 -1]));
                                sp->setTag(tag);
                            }
                            
                        }else{
                            this->alteredState(sp);
                        }
                        m_intClickNum--;
                        m_labelClickNum->setString(StringUtils::format("%d",m_intClickNum));
                    }
                    needBreak = true;
                    break;
                }
            }
            if(needBreak==true){
                break;
            }
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->m_listen,2);//比 ui图层 优先级低
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
    //初始化的格子
    for(int i =0;i<9;i++){
        for(int j = 0;j<9;j++){
            m_ArrayRect[i][j]=Rect(this->m_visibleOrigin.x+i*80,this->m_visibleOrigin.y+j*80+240,80,80);
        }
    }
}
void GameLayer::initUI(){
    auto bg = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("Map/DDLFT_ditu.png"));
    this->addChild(bg);
    bg->setPosition(Vec2(this->m_visibleOrigin.x+this->m_visibleSize.width*0.5,this->m_visibleOrigin.y+360.0f+80.0f*3.0f));
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
    auto animation =Animation::create();
    for(int i =1; i<27;i++){
        Rect rect = Rect::ZERO;
        auto texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("Animation/bomb/SKT-boomTEXIAO%d.png",i));
        rect.size = texture->getContentSize();
        SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
        animation->addSpriteFrame(frame);
    }
    AnimationCache::getInstance()->addAnimation(animation,"bomb");
    animation->setDelayPerUnit(1.0f/26.0f);
    
    //哈哈镜动画初始化
    auto magicmirror =Animation::create();
    for(int i =0; i<3;i++){
        Rect rect = Rect::ZERO;
        auto texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("Animation/Magicmirror/hahajing%d.png",i));
        rect.size = texture->getContentSize();
        SpriteFrame *frame = SpriteFrame::createWithTexture(texture, rect);
        magicmirror->addSpriteFrame(frame);
    }
    AnimationCache::getInstance()->addAnimation(magicmirror,"magicmirror");
    magicmirror->setDelayPerUnit(0.3f/3.0f);
    magicmirror->setRestoreOriginalFrame(true);
    
    //火把 初始化
    m_nodeFire = CSLoader::createNode("Animation/SKT-XHB-dianran/Layer.csb");
    m_nodeFire->setIgnoreAnchorPointForPosition(false);
    m_nodeFire->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(m_nodeFire);
    m_fireTimeline = CSLoader::createTimeline("Animation/SKT-XHB-dianran/Layer.csb");
    m_nodeFire->runAction(m_fireTimeline);
    m_nodeFire->setVisible(false);
    
//    auto node = Sprite::create();
//    this->addChild(node,100);
//    node->runAction(RepeatForever::create(Animate::create(magicmirror)));
//    node->setPosition(m_visibleOrigin.x+m_visibleSize.width*0.5,m_visibleSize.height*0.5+m_visibleOrigin.y);
   
   // m_fireTimeline->play("animation0", true);
    
    this->initData();
    this->scheduleUpdate();
    //this->schedule(schedule_selector(GameLayer::slowUpdate), 0.05, CC_REPEAT_FOREVER, 0); // 开启update
}
void GameLayer::btnCallBack(Ref* ref,std::string name){
    if(!name.compare("bomb")){
        if(m_usePropType==bomb){
            m_usePropType=none;
        }else{
             m_usePropType = bomb;
        }
    }else if(!name.compare("sweetMagic")){
        if((m_layer_gameOver&&m_layer_gameOver->isVisible())||m_vector_moveSp.size()>0){ //游戏结束   m_vector_moveSp里面有sp 时 不可以触发事件
            return;
        }
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
        this->m_intEliminateNum=0;
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
    }else if(!name.compare("magicmirror")){
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
        }
    }
    btn_Bomb->setScale(1.0f);
    btn_jelly->setScale(1.0f);
    btn_magicmirror->setScale(1.0f);
    btn_fire->setScale(1.0f);
    m_nodeFire->setVisible(false);
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
        m_nodeFire->setVisible(true);
        m_nodeFire->setPosition(btn_fire->getPosition());
        m_fireTimeline->play("animation0", true);
    }
}
void GameLayer::readJson(std::string path,int id){
    rapidjson::Document readdoc;
    std::string load_str = FileUtils::getInstance()->getStringFromFile(path);
    readdoc.Parse<0>(load_str.c_str());
    rapidjson::Value object = readdoc[StringUtils::format("%d",id).c_str()].GetObject();
    //数据先置为0
    m_intOneNum = 0;
    m_intTwoNum = 0;
    m_intThreeNum = 0;
    m_intFourNum = 0;
    //赋值
    if(object.HasMember("0")){
        m_intClickNum = object["0"].GetInt();
        m_labelClickNum->setString(StringUtils::format("%d",m_intClickNum));
    }
    if(object.HasMember("1")){
         m_intOneNum = object["1"].GetInt();
    }
    if(object.HasMember("2")){
         m_intTwoNum = object["2"].GetInt();
    }
    if(object.HasMember("3")){
        m_intThreeNum = object["3"].GetInt();
    }
    if(object.HasMember("4")){
        m_intFourNum = object["4"].GetInt();
    }
    if(object.HasMember("5")){ //冰块
        m_intIceNum = object["5"].GetInt();
    }
    if(object.HasMember("6")){ //小恶魔
        m_intDevilNum = object["6"].GetInt();
    }
    if(object.HasMember("7")){ //巧克力
        m_intChocolatesNum = object["7"].GetInt();
    }
}
int* GameLayer::getEmptyIndex(int* index){
    int index1 = CCRANDOM_0_1()*9;
    int index2 = CCRANDOM_0_1()*9;
    if(this->m_mapSprite.find(index1)!=this->m_mapSprite.end()&&(*this->m_mapSprite.find(index1)).second.find(index2)!=(*this->m_mapSprite.find(index1)).second.end()){//随机出的位置有东西
        int count = 0;
        while(true){
            //向上
            int tempIndex1=index1;
            int tempIndex2=index2+count;
            tempIndex2 = count % 9;
            tempIndex1+=(count/9);
            if(tempIndex1<9&&(this->m_mapSprite.find(tempIndex1)==this->m_mapSprite.end()||(*this->m_mapSprite.find(tempIndex1)).second.find(tempIndex2)==(*this->m_mapSprite.find(tempIndex1)).second.end())){
                index1 = tempIndex1;
                index2 = tempIndex2;
                break;
            }
            //向下
            tempIndex1=index1;
            tempIndex2=index2-count;
            tempIndex2 = count % 9;
            tempIndex1 = (count/9);
            if(tempIndex1>=0&&(this->m_mapSprite.find(tempIndex1)==this->m_mapSprite.end()||(*this->m_mapSprite.find(tempIndex1)).second.find(tempIndex2)==(*this->m_mapSprite.find(tempIndex1)).second.end())){
                index1 = tempIndex1;
                index2 = tempIndex2;
                break;
            }
            count++;
        }
    }
    index[0] = index1;
    index[1] = index2;
    return index;
}
cocos2d::Sprite* GameLayer::addElement(const std::string path,const int tag,const int* positonIndex,const std::string name){
    int index1 = positonIndex[0];
    int index2 = positonIndex[1];
    if(index1>8||index2>8||index1<0||index2<0){
        return nullptr;
    }
    
    auto sp = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(path));
    this->addChild(sp);
    sp->setPosition(Vec2(this->m_visibleOrigin.x+index1*80+sp->getBoundingBox().size.width*0.5,this->m_visibleOrigin.y+index2*80+240+sp->getBoundingBox().size.height*0.5));
    int*  userData = new int[2];
    userData[0]=index1;
    userData[1]=index2;
    sp->setUserData(userData);//注意这个 内存的释放
    sp->setTag(tag);
    sp->setName(name);
    if(m_mapSprite.find(index1)==m_mapSprite.end()){//map中不存在这个对象
        cocos2d::Map<int,Sprite*> tempSprite;
        tempSprite.insert(index2,sp);
        m_mapSprite.insert(std::map<int,cocos2d::Map<int,Sprite*>>::value_type(index1,tempSprite));
    }else{   //map中存在这个对象
        (*m_mapSprite.find(index1)).second.insert(index2,sp);
    }
    return sp;
}
void GameLayer::initData(){
    m_labelClickNum->setString(StringUtils::format("%d",m_intClickNum));
 //   this->readJson("configure/configure.json",m_intlevel);
   
    //初始化地图 精灵
    for(int i= 0; i<m_intOneNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addElement(m_ttqpath[0],0, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i = 0; i<m_intTwoNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addElement(m_ttqpath[1],1, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i =0; i<m_intThreeNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addElement(m_ttqpath[2],2, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i =0; i<m_intFourNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addElement(m_ttqpath[3],3, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i =0; i<m_intIceNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addElement(m_icepath[0],150, positionIndex,"ice");//tag 百位代表冰块层数   十位 代表可点数  个位 代表吃的糖果数
        delete [] positionIndex;
    }
    for(int i =0; i<m_intDevilNum;i++){   //
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addElement(m_ttqpath[0],9, positionIndex,"devil");
        delete [] positionIndex;
    }
    for(int i =0;i<m_intChocolatesNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addElement(m_cltPath[0],0, positionIndex,"clt");
        delete [] positionIndex;
    }

}
cocos2d::Sprite* GameLayer::getChild(const int index1,const int index2){
    auto itr1 =this->m_mapSprite.find(index1);
    if(itr1!=this->m_mapSprite.end()){
        auto itr2 = (*itr1).second.find(index2);
        if(itr2 != (*itr1).second.end()){
            return (*itr2).second;
        }
    }
    return nullptr;
}
void GameLayer::deleteChild(const int index1,const int index2){
    auto sp = this->getChild(index1, index2);
    if(sp!=nullptr){
        this->deleteChild(sp);
    }
}
void GameLayer::deleteChild(cocos2d::Sprite* sp){
    if(sp==nullptr){
        return;
    }
    (*(this->m_mapSprite.find(((int*)sp->getUserData())[0]))).second.erase(((int*)sp->getUserData())[1]);//删除sp
    delete[] (int*)sp->getUserData();
    sp->removeFromParent();
}
void GameLayer::chocolateCollision(const int index1,const int index2){
    auto sp = this->getChild(index1,index2);
    if(sp!=nullptr){
        if(!sp->getName().compare("ttq")){//巧克力遇到甜甜圈
            auto tag =sp->getTag();
            int  data[2];
            data[0] = ((int*)sp->getUserData())[0];
            data[1] = ((int*)sp->getUserData())[1];
            deleteChild(sp);
            this->addElement(m_cltPath[0],tag,data,"clt");
            
        }else if(!sp->getName().compare("devil"))//遇到小恶魔
        {
            
        }else if(!sp->getName().compare("ice")){//遇到冰块
            
        }
    }else{
        int temp [2];
        temp[0] = index1;
        temp[1]= index2;
        this->addElement(m_cltPath[0], 0,temp,"clt");
        int num =0;
        std::map<int,cocos2d::Map<int,cocos2d::Sprite*>>::iterator itr = this->m_mapSprite.begin();  //后期取消 这个遍历
        for(;itr!=this->m_mapSprite.end();itr++){
            auto itr2 =(*itr).second;
            for(auto child : itr2){
                if(!child.second->getName().compare("clt")){
                    num++;
                }
            }
        }
        if(num>=15){
            this->GameOver(false);
            return;
        }
    }
}
void GameLayer::update(float dt){
    if(this->m_vector_moveSp.size()<=0||(m_layer_gameOver&&m_layer_gameOver->isVisible())){
        if(m_intClickNum<=0){
            this->GameOver(false);
        }
        return;
    }
    Vector<Sprite*>::iterator itrMoveSp = this->m_vector_moveSp.begin();
    bool needBreak = false;
    for(;itrMoveSp<this->m_vector_moveSp.end();itrMoveSp++){
        if(!(*itrMoveSp)->getName().compare("cltMove")){
            continue;
        }
        for(auto child :this->m_mapSprite){
            for(auto mapSecond :child.second){
               auto sp = mapSecond.second;
                if((*itrMoveSp)->getBoundingBox().intersectsRect(sp->getBoundingBox())){
                    if(!sp->getName().compare("ice")){ //糖块遇到冰块
                        auto tag = sp->getTag();
                        if(tag<300){  //糖块小于300可以吃
                            tag+=1;
                            if(tag%10==4){
                                //播放特效
                                tag+=96;
                                sp->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_icepath[tag/100 - 1]));
                            }
                            sp->setTag(tag);
                        }
                    }
                    else if(!sp->getName().compare("devil")){  //小糖块遇到 小恶魔
                        sp->setTag(sp->getTag()+1);
                        if(sp->getTag()>=10){
                            this->deleteChild(sp);
                        }
                    }else if(!sp->getName().compare("clt")){ // 遇到的是巧克力
                        this->alteredClt(sp);
                    }else if(!sp->getName().compare("gd")){
                        auto spMove =static_cast<Sprite_New*>(*itrMoveSp);
                        spMove->stopAllActions();
                        Rect  moveRect = spMove->getBoundingBox();
                        Rect  spRect = sp->getBoundingBox();
                        if(spMove->getDirection()=='w'){
                            spMove->setPositionY(spMove->getPositionY()-(moveRect.origin.y+moveRect.size.height-spRect.origin.y)-1);//下移
                            spMove->setDirection('s');
                        }else if(spMove->getDirection()=='s'){
                            spMove->setPositionY(spMove->getPositionY()+spRect.origin.y+spRect.size.height-moveRect.origin.y+1);//上移
                            spMove->setDirection('w');
                        }else if(spMove->getDirection()=='a'){
                            spMove->setPositionX(spMove->getPositionX()+spRect.origin.x+spRect.size.width-moveRect.origin.x+1);
                            spMove->setDirection('d');
                        }else if(spMove->getDirection()=='d'){
                            spMove->setPositionX(spMove->getPositionX()-(moveRect.origin.x+moveRect.size.width-spRect.origin.x)-1);
                            spMove->setDirection('a');
                        }
                        if((*itrMoveSp)->getBoundingBox().intersectsRect(sp->getBoundingBox())){
                            throw ("ERROR");//计算有误 需要优化
                        }
                        static_cast<Sprite_New*>(spMove)->moveAction([=](){m_vector_moveSp.eraseObject(spMove);});
                        needBreak = true;
                        break;
                    }else if(!sp->getName().compare("magicmirror")){//遇到哈哈镜
                        auto spMove =static_cast<Sprite_New*>(*itrMoveSp);
                        spMove->stopAllActions();
                        Rect  moveRect = spMove->getBoundingBox();
                        Rect  spRect = sp->getBoundingBox();
                        int i= ((int*)sp->getUserData())[0];
                        int j= ((int*)sp->getUserData())[1];
                        if(spMove->getDirection()=='w'){
                            spMove->setDirection('d');
                            spMove->setPosition(Vec2(m_ArrayRect[i][j-1].origin.x+40,m_ArrayRect[i][j-1].origin.y+40));
                            if(moveRect.origin.y+moveRect.size.height>spRect.origin.y){
                                spMove->setPositionY(spMove->getPositionY()-(moveRect.origin.y+moveRect.size.height-spRect.origin.y)-1);//保证 不在触碰
                            }
                        }else if(spMove->getDirection()=='d'){
                            spMove->setDirection('s');
                            spMove->setPosition(Vec2(m_ArrayRect[i-1][j].origin.x+40,m_ArrayRect[i-1][j].origin.y+40));
                            if(moveRect.origin.x+moveRect.size.width>spRect.origin.x){
                                spMove->setPositionX(spMove->getPositionX()-(moveRect.origin.x+moveRect.size.width-spRect.origin.x)-1);//保证 不在触碰
                            }
                        }else if(spMove->getDirection()=='s'){
                            spMove->setDirection('a');
                            spMove->setPosition(Vec2(m_ArrayRect[i][j+1].origin.x+40,m_ArrayRect[i][j+1].origin.y+40));
                            if(moveRect.origin.y<spRect.origin.y+spRect.size.height){
                                spMove->setPositionY(spMove->getPositionY()+(spRect.origin.y+spRect.size.height - moveRect.origin.y)+1);//保证 不在触碰
                            }
                        }else if(spMove->getDirection()=='a'){
                            spMove->setDirection('w');
                            spMove->setPosition(Vec2(m_ArrayRect[i+1][j].origin.x+40,m_ArrayRect[i+1][j].origin.y+40));
                            if(spRect.origin.x+spRect.size.width>moveRect.origin.x){
                                spMove->setPositionX(spMove->getPositionX()+(spRect.origin.x+spRect.size.width-moveRect.origin.x)+1);
                            }
                        }
                        sp->setTag(sp->getTag()+1);
                        if((*itrMoveSp)->getBoundingBox().intersectsRect(sp->getBoundingBox())){
                            throw ("ERROR");//计算有误 需要优化
                        }
                        if(sp->getTag()>=5){
                            sp->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("magicmirror")),CallFunc::create([=](){
                                this->deleteChild(sp);
                            }),nullptr));
                        }else{
                            sp->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("magicmirror")));
                        }
                        static_cast<Sprite_New*>(spMove)->moveAction([=](){m_vector_moveSp.eraseObject(spMove);});
                        needBreak = true;
                        break;
                    }
                    else{
                         this->alteredState(sp);
                    }
                    (*itrMoveSp)->removeFromParent();
                    this->m_vector_moveSp.eraseObject(*itrMoveSp);
                    --itrMoveSp;
                    needBreak = true;
                    break;
                }
            }
            if(needBreak==true){
                needBreak=false;
                break;
            }
        }
    }
}
void GameLayer::GameRestart(Ref* ref){
    if(this->m_layer_gameOver){
        static_cast<MenuItemFont*>(this->m_layer_gameOver->getChildByName("Menu")->getChildByName("overBtn"))->setEnabled(false);
        this->m_layer_gameOver->setVisible(false);
    }
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
    this->initData();
}
void GameLayer::GameOver(bool vector){
    if(m_layer_gameOver!=nullptr&&m_layer_gameOver->isVisible()==true){
        return;
    }
    std::string vectorStr= "";
    if(vector==true){
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
void GameLayer::alteredClt(cocos2d::Sprite* sp){
    int tag = sp->getTag();
    tag++;
    if(tag>=sizeof(m_cltPath)/sizeof(m_cltPath[0])){
        (*(this->m_mapSprite.find(((int*)sp->getUserData())[0]))).second.erase(((int*)sp->getUserData())[1]);//删除sp
        for(int i = 0; i<4;i++){  //向4个反向 发射 4个sp
            int index1 = (((int*)sp->getUserData())[0]);
            int index2 = (((int*)sp->getUserData())[1]);
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
void GameLayer::alteredState(Sprite* sp){
        int tag = sp->getTag();
        tag++;
        if(tag>=m_ttqpath.size()){ //tag 值大于 3
            this->m_intEliminateNum++;
            if(this->m_intEliminateNum>10){
                
            }else if(this->m_intEliminateNum>20){
                
            }else if(this->m_intEliminateNum>20){
                
            }
            for(int i = 0; i<4;i++){  //向4个反向 发射 4个sp
                auto spMove = Sprite_New::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(m_ttqpath[0]));
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
            bool needBreak = false;
            std::map<int,cocos2d::Map<int,cocos2d::Sprite*>>::iterator itr = this->m_mapSprite.begin();
            for(;;itr++){
                if(itr==this->m_mapSprite.end()){
                    this->GameOver(true);
                    break;
                }
                auto itr2 =(*itr).second;
                for(auto child : itr2){
                    if(!child.second->getName().compare("ttq")){
                        needBreak = true;
                        break;
                    }
                }
                if(needBreak==true){
                    break;
                }
            }
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
    for(auto temp : this->m_mapSprite){
        for(auto mapSecond :temp.second){
            auto sp = mapSecond.second;
            delete [] (int*)sp->getUserData();
        }
    }

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
