#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

class KeyboardNotificationLayer;

//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class KeyboardNotificationLayer : public cocos2d::LayerColor, public cocos2d::IMEDelegate
{
public:
    KeyboardNotificationLayer();
    virtual void onClickTrackNodeIp(bool bClicked) = 0;
    virtual void onClickTrackNodePort(bool bClicked) = 0;
    
    virtual void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)override;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
protected:
    cocos2d::Node*  _trackNodeIp;
    cocos2d::Node*  _trackNodePort;
    cocos2d::Vec2  _beginPos;
};

class MainScene : public KeyboardNotificationLayer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onClickTrackNodeIp(bool bClicked) override;
    virtual void onClickTrackNodePort(bool bClicked) override;
    virtual void onEnter() override;
    
    void menuCloseCallback(cocos2d::Ref* pSender);

    bool readSettings();
    bool writeSettings();
    
    
    CREATE_FUNC(MainScene);
private:
    std::string _ipAddress;
    std::string _port;
};


#endif // __HELLOWORLD_SCENE_H__
