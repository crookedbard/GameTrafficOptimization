#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "string"
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
class ChatInput : public Layer, public EditBoxDelegate
{
public:
	ChatInput();
	~ChatInput();


	virtual bool init();
	CREATE_FUNC(ChatInput);

	// 需要重写触摸注册函数，重新给定触摸级别
	//virtual void registerWithTouchDispatcher(void);
	// 重写触摸函数，永远返回 true ，屏蔽其它层，达到 “模态” 效果
	bool TouchBegan(Touch *pTouch, Event *pEvent);
	void TouchEnded(Touch *pTouch, Event *pEvent);

	// 构架，并设置对话框背景图片
	static ChatInput* create(const char* backgroundImage);

	//添加输入框
	void addEditBox(int tag);
	void addEditBox(int tag, int maxlength);
	//点击菜单按钮的回调
	void buttonCallback(Ref* pSender);

	// 为了在显示层时之前的属性生效，选择在 onEnter 里动态展示
	virtual void onEnter();
	virtual void onExit();

	virtual void editBoxEditingDidBegin(EditBox* editBox);
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(EditBox* editBox);

	//    virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info);
	//    virtual void keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo& info);
	//    virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& info);
	//    virtual void keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo& info);

	EditBox* m_pEditName;

	void ShowChangeclass(Ref *pSender);
	void toChangeclass(Ref *pSender);

	void sendMsg(Ref *pSender);
private:
	Size winSize;

	string p_chat_bt_qb0;
	string p_chat_bt_qb1;
	string p_chat_input_bd;

	string p_chat_bt_sx0;
	string p_chat_bt_sx1;

	string p_chat_bt_tx0;
	string p_chat_bt_tx1;

	string p_chat_bt_jx0;
	string p_chat_bt_jx1;

	string p_caozuo_send0;
	string p_caozuo_send1;

	string p_login_zhanghu;

	void menuToggleCallback(Ref* pSender);

	void togai(float togai);

	// 文字内容两边的空白区
	int m_contentPadding;
	int m_contentPaddingTop;


	CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);

	CC_SYNTHESIZE_RETAIN(Sprite*, m__sTitle, SpriteTitle);

	CC_SYNTHESIZE_RETAIN(EditBox*, m__pEditBox, EditBox);



	CCRect thisRect;//弹框的rect
	CCRect getRect();
};

//#endif