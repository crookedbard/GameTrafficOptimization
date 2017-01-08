#include "MainScene.h"
#include "VisibleRect.h"

USING_NS_CC;

const std::string SETTINGS_FILENAME = "Settings.txt";
const std::string FONT_NAME = "fonts/Marker Felt.ttf";
const int FONT_SIZE = 16;
const int DEPTH_HUD = 1;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init()) return false;
    if (!readSettings()) return false;
    
    LayerColor::initWithColor(cocos2d::Color4B(0,0,0,0));
    
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainScene::menuCloseCallback, this));
    
    closeItem->setPosition(VisibleRect::rightBottom().x - closeItem->getContentSize().width/2 ,
                                VisibleRect::rightBottom().y + closeItem->getContentSize().height/2);


    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, DEPTH_HUD);

//    auto labelIpAddress = Label::createWithTTF(_ipAddress.c_str(), FONT_NAME.c_str(), FONT_SIZE);
//    labelIpAddress->setPosition(VisibleRect::center().x - labelIpAddress->getWidth(), VisibleRect::center().y + labelIpAddress->getHeight());
//    this->addChild(labelIpAddress, DEPTH_HUD);
    
    return true;
}

//////////////////////////////////////////////////////////////////////////
// implement KeyboardNotificationLayer
//////////////////////////////////////////////////////////////////////////

static Rect getRect(Node * node)
{
    Rect rc;
    rc.origin = node->getPosition();
    rc.size = node->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

KeyboardNotificationLayer::KeyboardNotificationLayer()
: _trackNodeIp(0), _trackNodePort(0)
{
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void KeyboardNotificationLayer::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
          info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);
    
    if (! _trackNodeIp && !_trackNodePort)
    {
        return;
    }
    
    auto rectTracked = getRect(_trackNodeIp);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
          rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
    
    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! rectTracked.intersectsRect(info.end))
    {
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
    
    // move all the children node of KeyboardNotificationLayer
    auto& children = getChildren();
    Node * node = 0;
    ssize_t count = children.size();
    Vec2 pos;
    for (int i = 0; i < count; ++i)
    {
        node = children.at(i);
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
    }
    
    //port
    
    auto rectTrackedPort = getRect(_trackNodePort);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
          rectTrackedPort.origin.x, rectTrackedPort.origin.y, rectTrackedPort.size.width, rectTrackedPort.size.height);
    
    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! rectTrackedPort.intersectsRect(info.end))
    {
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVertPort = info.end.getMaxY() - rectTrackedPort.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVertPort);
    
    // move all the children node of KeyboardNotificationLayer
    auto& childrenPort = getChildren();
    Node * nodePort = 0;
    ssize_t countPort = childrenPort.size();
    Vec2 posPort;
    for (int i = 0; i < countPort; ++i)
    {
        nodePort = childrenPort.at(i);
        posPort = nodePort->getPosition();
        posPort.y += adjustVertPort;
        nodePort->setPosition(posPort);
    }
}

bool KeyboardNotificationLayer::onTouchBegan(Touch  *touch, Event  *event)
{
    CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
    _beginPos = touch->getLocation();
    return true;
}

void KeyboardNotificationLayer::onTouchEnded(Touch  *touch, Event  *event)
{
    if (!_trackNodeIp && !_trackNodePort)
    {
        return;
    }
    
    auto endPos = touch->getLocation();
    
    float delta = 5.0f;
    if (std::abs(endPos.x - _beginPos.x) > delta
        || std::abs(endPos.y - _beginPos.y) > delta)
    {
        // not click
        _beginPos.x = _beginPos.y = -1;
        return;
    }
    
    // decide the trackNode is clicked.
    Rect rect, rect2;
    rect.size = _trackNodeIp->getContentSize();
    rect2.size = _trackNodePort->getContentSize();
    auto clicked = isScreenPointInRect(endPos, Camera::getVisitingCamera(), _trackNodeIp->getWorldToNodeTransform(), rect, nullptr);
    auto clicked2 = isScreenPointInRect(endPos, Camera::getVisitingCamera(), _trackNodePort->getWorldToNodeTransform(), rect2, nullptr);
    this->onClickTrackNodeIp(clicked);
    this->onClickTrackNodePort(clicked2);
    CCLOG("----------------------------------");
}

void MainScene::onClickTrackNodeIp(bool bClicked)
{
    auto pTextField = (TextFieldTTF*)_trackNodeIp;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}
void MainScene::onClickTrackNodePort(bool bClicked)
{
    auto pTextField = (TextFieldTTF*)_trackNodePort;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void MainScene::onEnter()
{
    KeyboardNotificationLayer::onEnter();
    
    // add TextFieldTTF
    auto s = Director::getInstance()->getWinSize();
    
    auto labelIpAddress = TextFieldTTF::textFieldWithPlaceHolder(_ipAddress,
                                                             FONT_NAME,
                                                             FONT_SIZE);
    auto labelPort = TextFieldTTF::textFieldWithPlaceHolder(_port,
                                                             FONT_NAME,
                                                             FONT_SIZE);
    addChild(labelIpAddress, DEPTH_HUD);
    addChild(labelPort, DEPTH_HUD);

    labelIpAddress->setPosition(VisibleRect::center().x - labelIpAddress->getWidth(), VisibleRect::center().y + 100);
    labelPort->setPosition(VisibleRect::center().x - labelPort->getWidth(), VisibleRect::center().y + 100 - FONT_SIZE);

    
    _trackNodeIp = labelIpAddress;
    _trackNodePort = labelPort;
}

bool MainScene::readSettings()
{
    auto sharedFileUtils = FileUtils::getInstance();
    const int settingsCount = 2; //ipAddress and port
    
    bool exist = sharedFileUtils->isFileExist(SETTINGS_FILENAME);
    if(!exist) return false;
    

    std::string str = sharedFileUtils->getStringFromFile(SETTINGS_FILENAME);
    
    if(str.empty()) return false;
    std::stringstream ss(str);
    std::string segment;
    std::vector<std::string> seglist;
    
    while(std::getline(ss, segment, '\n'))
    {
        seglist.push_back(segment);
    }
    if(seglist.size() < settingsCount) return false;

    _ipAddress = seglist[0];
    _port = seglist[1];
    
    return true;
}

bool MainScene::writeSettings()
{
    auto sharedFileUtils = FileUtils::getInstance();
    bool exist = sharedFileUtils->isFileExist(SETTINGS_FILENAME);
    if(!exist) return false;
    std::string writeDataStr = _ipAddress + "\n" + _port + "\n";
    if (!sharedFileUtils->writeStringToFile(writeDataStr, SETTINGS_FILENAME.c_str()))
        return false;
    return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
