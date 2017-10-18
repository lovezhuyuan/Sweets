#include "GameLayer.h"
#include "AppDelegate.h"
#include "json/document.h"
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
    
    this->schedule(schedule_selector(GameLayer::slowUpdate), 0.2, CC_REPEAT_FOREVER, 0);
    //点击精灵 事件监听
    m_listen = EventListenerTouchOneByOne::create();
    m_listen->onTouchBegan = [=](Touch * touch,Event* event)mutable{
        Vec2 location = touch->getLocation();
        for(auto temp : this->m_mapSprite){
            for(auto mapSecond :temp.second){
                auto sp = mapSecond.second;
                if(sp->getBoundingBox().containsPoint(location)){
                    this->alteredState(sp);
                    break;
                }
            }
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->m_listen,1);//ui图层 优先级低
    std::string path[]={
        "ttq/ttq_1.png",
        "ttq/ttq_2.png",
        "ttq/ttq_3.png",
        "ttq/ttq_4.png",
        "Map/DDLFT_ditu.png"
    };
    int * index = new int(sizeof(path)/sizeof(path[0]));
    for(int i =0;i<sizeof(path)/sizeof(path[0]);i++){
        Director::getInstance()->getTextureCache()->addImageAsync(path[i],[=](Texture2D* texture)mutable{
            --(*index);
            if(texture==nullptr){
                CCLOG("111");
                return;
            }
            if((*index)==0){
                delete index;
                index = nullptr;
                this->initUI();
            }
        });
    }
}
void GameLayer::initUI(){
    this->m_texture_dikuai.pushBack(Director::getInstance()->getTextureCache()->getTextureForKey("ttq/ttq_1.png"));
    this->m_texture_dikuai.pushBack(Director::getInstance()->getTextureCache()->getTextureForKey("ttq/ttq_2.png"));
    this->m_texture_dikuai.pushBack(Director::getInstance()->getTextureCache()->getTextureForKey("ttq/ttq_3.png"));
    this->m_texture_dikuai.pushBack(Director::getInstance()->getTextureCache()->getTextureForKey("ttq/ttq_4.png"));
    auto bg = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("Map/DDLFT_ditu.png"));
    this->addChild(bg);
    bg->setPosition(Vec2(this->m_visibleOrigin.x+this->m_visibleSize.width*0.5,this->m_visibleOrigin.y+360.0f+80.0f*3.0f));
    this->readJson("configure/configure.json",1);
    this->initData();
}
void GameLayer::readJson(std::string path,int id){
    rapidjson::Document readdoc;
    std::string load_str = FileUtils::getInstance()->getStringFromFile(path);
    readdoc.Parse<0>(load_str.c_str());
    //assert(readdoc.HasMember(StringUtils::format("%d",id).c_str()));
    rapidjson::Value object = readdoc[StringUtils::format("%d",id).c_str()].GetObject();
    //数据先置为0
    m_intOneNum = 0;
    m_intTwoNum = 0;
    m_intThreeNum = 0;
    m_intFourNum = 0;
    //赋值
    if(object.HasMember("1")){
         m_intOneNum = object["1"].GetInt();
    }
    if(object.HasMember("2")){
         m_intTwoNum = object["3"].GetInt();
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
        m_intIceNum = object["6"].GetInt();
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
cocos2d::Sprite* GameLayer::addObject(int textureIndex,int* positonIndex,std::string name){
    int index1 = positonIndex[0];
    int index2 = positonIndex[1];
    auto sp = Sprite::createWithTexture(this->m_texture_dikuai.at(textureIndex));
    this->addChild(sp);
    sp->setPosition(Vec2(this->m_visibleOrigin.x+index2*80+sp->getBoundingBox().size.width*0.5,this->m_visibleOrigin.y+index1*80+240+sp->getBoundingBox().size.height*0.5));
    auto label = Label::createWithTTF(StringUtils::format("%d",textureIndex),"fonts/Marker Felt.ttf",24);
    sp->addChild(label);
    label->setColor(Color3B(255, 0, 0));
    label->setPosition(this->m_visibleOrigin.x+sp->getBoundingBox().size.width*0.5,this->m_visibleOrigin.y+sp->getBoundingBox().size.height*0.5);
    label->setName("label");
    int*  userData = new int[2];
    userData[0]=index1;
    userData[1]=index2;
    sp->setUserData(userData);//注意这个 内存的释放
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
    //初始化地图 精灵
    for(int i= 0; i<m_intOneNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addObject(0, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i = 0; i<m_intTwoNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addObject(1, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i =0; i<m_intThreeNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addObject(2, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i =0; i<m_intFourNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addObject(3, positionIndex,"ttq");
        delete [] positionIndex;
    }
    for(int i =0; i<m_intIceNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addObject(-1, positionIndex,"ice");
        delete [] positionIndex;
    }
    for(int i =0; i<m_intIceNum;i++){
        int* positionIndex = new int[2];
        positionIndex = getEmptyIndex(positionIndex);
        this->addObject(-1, positionIndex,"devil");
        delete [] positionIndex;
    }
    
    
}
void GameLayer::slowUpdate(float dt){
    Vector<Sprite*>::iterator itrMoveSp = this->m_vector_moveSp.begin();
    bool needBreak = false;
    for(;itrMoveSp<this->m_vector_moveSp.end();itrMoveSp++){
        for(auto child :this->m_mapSprite){
            for(auto mapSecond :child.second){
               auto sp = mapSecond.second;
                auto aa =sp->getBoundingBox();
                auto bb =(*itrMoveSp)->getBoundingBox();
                if((*itrMoveSp)->getBoundingBox().intersectsRect(sp->getBoundingBox())){
                    (*itrMoveSp)->removeFromParent();
                    this->m_vector_moveSp.eraseObject(*itrMoveSp);
                    this->alteredState(sp);
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
        this->m_layer_gameOver->setVisible(false);
    }
    Vector<Sprite*>::iterator itr=this->m_vector_moveSp.begin();
    for(;itr<this->m_vector_moveSp.end();itr++){
        (*itr)->removeFromParent();
        this->m_vector_moveSp.eraseObject(*itr);
        itr--;
    }
    this->initData();
}
void GameLayer::GameOver(bool vector){
    std::string vectorStr= "";
    if(vector==true){
        vectorStr="Successful";
    }else{
        vectorStr="GameOver";
    }
    if(this->m_layer_gameOver){
        this->m_layer_gameOver->setVisible(true);
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
        this->addChild(this->m_layer_gameOver);
    }
   
}
void GameLayer::alteredState(Sprite* sp){
        int tag = sp->getTag();
        tag++;
        if(tag>3){ //tag 值大于 3
            (*(this->m_mapSprite.find(((int*)sp->getUserData())[0]))).second.erase(((int*)sp->getUserData())[1]);//删除sp
            std::map<int,cocos2d::Map<int,cocos2d::Sprite*>>::iterator itr = this->m_mapSprite.begin();
            for(;;itr++){
                if(itr==this->m_mapSprite.end()){
                    this->GameOver(true);
                    break;
                }
                if((*itr).second.empty()){
                    continue;
                }else{
                    break;;
                }
            }
            for(int i = 0; i<4;i++){  //向4个反向 发射 4个sp
                auto spMove = Sprite::createWithTexture(this->m_texture_dikuai.at(0));
                spMove->setTag(1);
                this->addChild(spMove);
                spMove->setPosition(sp->getPosition());
                spMove->setScale(1/4.0f);
                float duration =0.0f;
                Vec2 postion;
                if(i==0){//向上飞
                    duration = 13.0f-spMove->getPositionY()/80.0f;
                    postion = Vec2(spMove->getPositionX(),13.0f*80.0f+this->m_visibleOrigin.y);
                }else if(i==1){  //向下飞
                    duration = spMove->getPositionY()/80.0f-2.0f;
                    postion = Vec2(spMove->getPositionX(),2.0f*80.0f+this->m_visibleOrigin.y);
                }else if(i==2){  //向左飞
                    duration = spMove->getPositionX()/80.0f+1.0f;
                    postion = Vec2(-80.0f+this->m_visibleOrigin.x,spMove->getPositionY());
                }else{//向右飞
                    duration =10.0f - spMove->getPositionX()/80.0f;
                    postion = Vec2(10.0f*80.0f+this->m_visibleOrigin.x,spMove->getPositionY());
                }
                this->m_vector_moveSp.pushBack(spMove);
                spMove->runAction(Sequence::create(MoveTo::create(duration,postion),DelayTime::create(0.2f),CallFunc::create([=](){
                    this->m_vector_moveSp.eraseObject(spMove);
                }),RemoveSelf::create(),NULL));  //延时0.2 秒移除
            }
            auto intTemp = (int*)sp->getUserData();
            delete intTemp;
            intTemp = nullptr;
            sp->removeFromParent();
            sp=nullptr;
        }else{  //tag 值小于 4  tag + 1
            static_cast<Label*>(sp->getChildByName("label"))->setString(StringUtils::format("%d",tag));
            sp->setTag(tag);
            sp->setTexture(this->m_texture_dikuai.at(tag));
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
            auto tempData = (int*)sp->getUserData();
            CC_SAFE_DELETE_ARRAY(tempData);
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
