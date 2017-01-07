#include "ChatInput.h"
//#include "../GameConfig.h"
//#include "../Commen/PublicShowUI.h"
//#include "../ImagePaths.h"
//#include "../Commen/PublicDoFunc.h"
//#include "../GameData/GlobalInfo.h"
//#include "../ToDoCommen/ToDoSend.h"

#define BASEUI_CHAT_CLASS 60
#define BASEUI_CHAT_SEND 70
#define BASEUI_CHAT_CHANGE_QB 71
#define BASEUI_CHAT_CHANGE_SX 72
#define BASEUI_CHAT_CHANGE_TX 73
#define BASEUI_CHAT_CHANGE_JX 74

//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
ChatInput::ChatInput()
:m__pMenu(NULL)
, m_contentPadding(0)
, m_contentPaddingTop(0)
, m__sfBackGround(NULL)
, m__s9BackGround(NULL)
, m__ltContentText(NULL)
, m__ltTitle(NULL)

, m__sTitle(NULL)
, m__pEditBox(NULL)
{
	winSize = Director::getInstance()->getWinSize();

	CCLog("ChatInput::ChatInput");
}



ChatInput::~ChatInput()
{
	CCLog("ChatInput::~ChatInput");
	CC_SAFE_RELEASE(m__pMenu);
	CC_SAFE_RELEASE(m__sfBackGround);
	CC_SAFE_RELEASE(m__s9BackGround);
	CC_SAFE_RELEASE(m__ltContentText);
	CC_SAFE_RELEASE(m__ltTitle);
	CC_SAFE_RELEASE(m__sTitle);

}


bool ChatInput::init()
{
	CCLog("ChatInput::init;");
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		this->setContentSize(Size::ZERO);

		p_chat_bt_qb0 = "EditBoxTemplate.png";
		p_chat_bt_qb1 = "HUD/btn_wifi_on.png";
		p_chat_input_bd = "CloseSelected.png";

		p_chat_bt_sx0 = "EditBoxTemplate.png";
		p_chat_bt_sx1 = "HUD/btn_wifi_on.png";

		p_chat_bt_tx0 = "EditBoxTemplate.png";
		p_chat_bt_tx1 = "HUD/btn_wifi_on.png";

		p_chat_bt_jx0 = "EditBoxTemplate.png";
		p_chat_bt_jx1 = "HUD/btn_wifi_on.png";

		p_caozuo_send0 = "EditBoxTemplate.png";
		p_caozuo_send1 = "HUD/btn_wifi_on.png";

		p_login_zhanghu = "CloseSelected.png";

		// 初始化需要的 Menu
		Menu* menu = Menu::create();
		menu->setPosition(Vec2::ZERO);
		setMenuButton(menu);
		setTouchEnabled(true);
		bRef = true;
	} while (0);
	return bRef;

}

/*void ChatInput::registerWithTouchDispatcher()
{
	// 这里的触摸优先级设置为 -128 这保证了，屏蔽下方的触摸
	Director::getInstance()->getEventDispatcher->addTargetedDelegate(this, -128, true); // getTouchDispatcher()
}*/

bool ChatInput::TouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

ChatInput* ChatInput::create(const char *backgroundImage)
{
	ChatInput* ml = ChatInput::create();
	return ml;
}

void ChatInput::addEditBox(int tag, int maxlength)
{
	EditBox* m_pEditBox = EditBox::create(Size(300, 40), Scale9Sprite::create(p_chat_input_bd));
	if (maxlength>0)
	{
		m_pEditBox->setMaxLength(maxlength);
	}
	m_pEditBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_pEditBox->setDelegate(this); //必须加这一行，否则不能扑捉到editBoxEditingDidBegin等事件
	m_pEditBox->setFontColor(ccWHITE);
	//    m_pEditBox->setText("请输入...");
	m_pEditBox->setTag(tag);


	MenuItemImage *button_chatclass = MenuItemImage::create(
		p_chat_bt_qb0,
		p_chat_bt_qb1,
		p_chat_bt_qb1,
		button_chatclass,
		menu_selector(ChatInput::sendMsg));
	Menu* pMenu_button_chatclass = Menu::create(button_chatclass, NULL);
	pMenu_button_chatclass->setPosition(ccp(-30, m_pEditBox->getContentSize().height / 2));
	m_pEditBox->addChild(pMenu_button_chatclass, 111, BASEUI_CHAT_CLASS);


	setEditBox(m_pEditBox);

	//必须专门设置一次同layer级别一样的优先级，否则不能触发弹出键盘
	//getEditBox()->setTouchPriority(111);
}

void ChatInput::sendMsg(Ref *pSender)
{
	/*string nrstr = GlobalInfo::getInstance()->get_chatnrstr();
	if (nrstr.length()>0)
	{
		MainRoledata* roleinfo = GlobalInfo::getInstance()->get_mainrole();

		//        if(roleinfo->user_level>10||roleinfo->User_zhiye=="天仙"||roleinfo->User_zhiye=="金仙")
		//        {
		if (GlobalInfo::getInstance()->get_chatclassname() == "全部")
		{
			ToDoSend::send_sendMyMsg(CCString::create("888"), CCString::create(nrstr.c_str()), CCString::create(roleinfo->Game_UserName), CCString::create(GlobalInfo::getInstance()->get_chatclassname().c_str()), CCString::create("0"));
			GlobalInfo::getInstance()->set_chat_close_flag(true);
		}
		else if (GlobalInfo::getInstance()->get_chatclassname() == "天仙")
		{
			if (roleinfo->User_zhiye == "天仙" || roleinfo->User_zhiye == "金仙")
			{
				ToDoSend::send_sendMyMsg(CCString::create("888"), CCString::create(nrstr.c_str()), CCString::create(roleinfo->Game_UserName), CCString::create(GlobalInfo::getInstance()->get_chatclassname().c_str()), CCString::create("0"));
				GlobalInfo::getInstance()->set_chat_close_flag(true);
			}
			else
			{
				GlobalInfo::getInstance()->set_showtipstring("晋升到天仙以上才能进行天仙传音");
				GlobalInfo::getInstance()->set_showtipflag(true);
			}
		}
		else if (GlobalInfo::getInstance()->get_chatclassname() == "金仙")
		{
			if (roleinfo->User_zhiye == "金仙")
			{
				ToDoSend::send_sendMyMsg(CCString::create("888"), CCString::create(nrstr.c_str()), CCString::create(roleinfo->Game_UserName), CCString::create(GlobalInfo::getInstance()->get_chatclassname().c_str()), CCString::create("0"));
				GlobalInfo::getInstance()->set_chat_close_flag(true);

			}
			else
			{
				GlobalInfo::getInstance()->set_showtipstring("晋升到金仙以上才能进行金仙传音");
				GlobalInfo::getInstance()->set_showtipflag(true);
			}
		}
		//        }
		//        else
		//        {
		//            GlobalInfo::getInstance()->set_showtipstring("修炼到散仙10层才能使用传音功能！");
		//            GlobalInfo::getInstance()->set_showtipflag(true);
		//        }
	}
	else
	{
		GlobalInfo::getInstance()->set_showtipstring("请输入聊天内容");
		GlobalInfo::getInstance()->set_showtipflag(true);
	}*/

}


void ChatInput::ShowChangeclass(Ref *pSender)
{
	//获取原按钮
	MenuItemImage *button = (MenuItemImage*)pSender;

	//    button->setNormalSpriteFrame(CCSpriteFrame::create(p_chat_bt_tx0, CCRectMake(0, 0, button->getContentSize().width,button->getContentSize().height)));
	//全部
	if (button->getChildByTag(BASEUI_CHAT_CHANGE_QB) != NULL)
	{
		button->removeChildByTag(BASEUI_CHAT_CHANGE_QB);
	}
	else
	{
		MenuItemImage *button_class_qb = MenuItemImage::create(
			p_chat_bt_qb0,
			p_chat_bt_qb1,
			p_chat_bt_qb1,
			button_class_qb,
			menu_selector(ChatInput::toChangeclass));
		button_class_qb->setTag(BASEUI_CHAT_CHANGE_QB);
		Menu* pMenu_button_qb = Menu::create(button_class_qb, NULL);
		pMenu_button_qb->setPosition(ccp(button->getContentSize().width / 2, button->getContentSize().height*1.45));
		button->addChild(pMenu_button_qb, 111, BASEUI_CHAT_CHANGE_QB);
	}
	//散仙
	if (button->getChildByTag(BASEUI_CHAT_CHANGE_SX) != NULL)
	{
		button->removeChildByTag(BASEUI_CHAT_CHANGE_SX);
	}
	else
	{
		MenuItemImage *button_class_sx = MenuItemImage::create(
			p_chat_bt_sx0,
			p_chat_bt_sx1,
			p_chat_bt_sx1,
			button_class_sx,
			menu_selector(ChatInput::toChangeclass));
		button_class_sx->setTag(BASEUI_CHAT_CHANGE_SX);
		Menu* pMenu_button_sx = Menu::create(button_class_sx, NULL);
		pMenu_button_sx->setPosition(ccp(button->getContentSize().width / 2, button->getContentSize().height*2.4));
		button->addChild(pMenu_button_sx, 111, BASEUI_CHAT_CHANGE_SX);
	}
	//天仙
	if (button->getChildByTag(BASEUI_CHAT_CHANGE_TX) != NULL)
	{
		button->removeChildByTag(BASEUI_CHAT_CHANGE_TX);
	}
	else
	{
		MenuItemImage *button_class_tx = MenuItemImage::create(
			p_chat_bt_tx0,
			p_chat_bt_tx1,
			p_chat_bt_tx1,
			button_class_tx,
			menu_selector(ChatInput::toChangeclass));
		button_class_tx->setTag(BASEUI_CHAT_CHANGE_TX);
		Menu* pMenu_button_tx = Menu::create(button_class_tx, NULL);
		pMenu_button_tx->setPosition(ccp(button->getContentSize().width / 2, button->getContentSize().height*3.4));
		button->addChild(pMenu_button_tx, 111, BASEUI_CHAT_CHANGE_TX);
	}
	//金仙
	if (button->getChildByTag(BASEUI_CHAT_CHANGE_JX) != NULL)
	{
		button->removeChildByTag(BASEUI_CHAT_CHANGE_JX);
	}
	else
	{
		MenuItemImage *button_class_jx = MenuItemImage::create(
			p_chat_bt_jx0,
			p_chat_bt_jx1,
			p_chat_bt_jx1,
			button_class_jx,
			menu_selector(ChatInput::toChangeclass));
		button_class_jx->setTag(BASEUI_CHAT_CHANGE_JX);
		Menu* pMenu_button_jx = Menu::create(button_class_jx, NULL);
		pMenu_button_jx->setPosition(ccp(button->getContentSize().width / 2, button->getContentSize().height*4.4));
		button->addChild(pMenu_button_jx, 111, BASEUI_CHAT_CHANGE_JX);
	}
}

void ChatInput::togai(float togai)
{
	//    this->getChildByTag(BASEUI_CHAT_CLASS)->setVisible(false);
	/*if (GlobalInfo::getInstance()->get_todo_changeChatClass_flag() == true)
	{

		CCPoint nowp = this->getChildByTag(BASEUI_CHAT_CLASS)->getPosition();
		this->removeChildByTag(BASEUI_CHAT_CLASS);
		if (GlobalInfo::getInstance()->get_todo_changeChatClass_int() == BASEUI_CHAT_CHANGE_SX)
		{
			CCMenuItemImage *button_class_sx = CCMenuItemImage::create(
				p_chat_bt_sx0,
				p_chat_bt_sx1,
				p_chat_bt_sx1,
				this,
				menu_selector(ChatInput::ShowChangeclass));
			CCMenu* pMenu_button_sx = CCMenu::create(button_class_sx, NULL);
			pMenu_button_sx->setPosition(nowp);
			this->addChild(pMenu_button_sx, 111, BASEUI_CHAT_CLASS);
		}
		else if (GlobalInfo::getInstance()->get_todo_changeChatClass_int() == BASEUI_CHAT_CHANGE_QB)
		{
			CCMenuItemImage *button_class_qb = CCMenuItemImage::create(
				p_chat_bt_qb0,
				p_chat_bt_qb1,
				p_chat_bt_qb1,
				this,
				menu_selector(ChatInput::ShowChangeclass));
			CCMenu* pMenu_button_qb = CCMenu::create(button_class_qb, NULL);
			pMenu_button_qb->setPosition(nowp);
			this->addChild(pMenu_button_qb, 111, BASEUI_CHAT_CLASS);
		}
		else if (GlobalInfo::getInstance()->get_todo_changeChatClass_int() == BASEUI_CHAT_CHANGE_TX)
		{
			CCMenuItemImage *button_class_tx = CCMenuItemImage::create(
				p_chat_bt_tx0,
				p_chat_bt_tx1,
				p_chat_bt_tx1,
				this,
				menu_selector(ChatInput::ShowChangeclass));
			CCMenu* pMenu_button_tx = CCMenu::create(button_class_tx, NULL);
			pMenu_button_tx->setPosition(nowp);
			this->addChild(pMenu_button_tx, 111, BASEUI_CHAT_CLASS);
		}
		else if (GlobalInfo::getInstance()->get_todo_changeChatClass_int() == BASEUI_CHAT_CHANGE_JX)
		{
			CCMenuItemImage *button_class_jx = CCMenuItemImage::create(
				p_chat_bt_jx0,
				p_chat_bt_jx1,
				p_chat_bt_jx1,
				this,
				menu_selector(ChatInput::ShowChangeclass));
			CCMenu* pMenu_button_jx = CCMenu::create(button_class_jx, NULL);
			pMenu_button_jx->setPosition(nowp);
			this->addChild(pMenu_button_jx, 111, BASEUI_CHAT_CLASS);
		}

		GlobalInfo::getInstance()->set_todo_changeChatClass_flag(false);
	}*/

}

void ChatInput::toChangeclass(Ref *pSender)
{
	Node * nd = (Node*)pSender;
	int TAGS = nd->getTag();
	if (TAGS == BASEUI_CHAT_CHANGE_SX)
	{
		//GlobalInfo::getInstance()->set_chatclassname("散仙");
	}
	else if (TAGS == BASEUI_CHAT_CHANGE_QB)
	{
		//GlobalInfo::getInstance()->set_chatclassname("全部");
	}
	else if (TAGS == BASEUI_CHAT_CHANGE_TX)
	{
		//GlobalInfo::getInstance()->set_chatclassname("天仙");
	}
	else if (TAGS == BASEUI_CHAT_CHANGE_JX)
	{
		//GlobalInfo::getInstance()->set_chatclassname("金仙");
	}
	//GlobalInfo::getInstance()->set_todo_changeChatClass_flag(true);
	//GlobalInfo::getInstance()->set_todo_changeChatClass_int(nd->getTag());
}

void ChatInput::addEditBox(int tag)
{
	//GlobalInfo::getInstance()->set_chatclassname("全部");
	EditBox* m_pEditBox = EditBox::create(Size(210, 28), Scale9Sprite::create(p_chat_input_bd));
	m_pEditBox->setMaxLength(120);
	m_pEditBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_pEditBox->setDelegate(this); //必须加这一行，否则不能扑捉到editBoxEditingDidBegin等事件
	m_pEditBox->setFontColor(ccWHITE);
	//    m_pEditBox->setText("请输入...");
	//    m_pEditBox->setFontSize(5);
	m_pEditBox->setTag(tag);
	//设置键盘输入模式
	m_pEditBox->setInputMode(EditBox::InputMode::ANY);

	MenuItemImage *button_chatclass = MenuItemImage::create(
		p_chat_bt_qb0,
		p_chat_bt_qb1,
		p_chat_bt_qb1,
		this,
		menu_selector(ChatInput::ShowChangeclass));
	Menu* pMenu_button_chatclass = Menu::create(button_chatclass, NULL);
	pMenu_button_chatclass->setPosition(ccp(-button_chatclass->getContentSize().width*0.5, m_pEditBox->getContentSize().height / 2));
	m_pEditBox->addChild(pMenu_button_chatclass, 111, BASEUI_CHAT_CLASS);

	MenuItemImage *button_send = MenuItemImage::create(
		p_caozuo_send0,
		p_caozuo_send1,
		p_caozuo_send1,
		button_chatclass,
		menu_selector(ChatInput::sendMsg));
	Menu* pMenu_button_send = Menu::create(button_send, NULL);
	pMenu_button_send->setPosition(ccp(m_pEditBox->getContentSize().width*1.15, m_pEditBox->getContentSize().height / 2));
	m_pEditBox->addChild(pMenu_button_send, 112, BASEUI_CHAT_SEND);

	setEditBox(m_pEditBox);

	m_pEditBox->schedule(schedule_selector(ChatInput::togai));

	//必须专门设置一次同layer级别一样的优先级，否则不能触发弹出键盘
	//getEditBox()->setTouchPriority(-128);
}



void ChatInput::buttonCallback(Ref *pSender){
	//    CCNode* node = dynamic_cast<CCNode*>(pSender);
	//    CCLog("menubutton tag: %d", node->getTag());
	//    if(node->getTag()==LAYER_HALL_EDITBOX_SEND){
	//        CCLog("发送:%s len=%d",getEditBox()->getText(),strlen(getEditBox()->getText()));
	//        const char* text = getEditBox()->getText();
	//        if(strlen(text)>0)
	//        {
	//            CCArray *data = CCArray::create();
	//            data->addObject(CCString::create(text));
	//            SocketThread::sharedSocketThread()->Send(EVENT_LOUDChatInput,data);
	//        }
	//    }else{
	//        CCLog("关闭");
	//    }
	//    if (m_callback && m_callbackListener){
	//        CCLog("调用注册函数");
	//        (m_callbackListener->*m_callback)();
	//    }
	//    this->removeFromParentAndCleanup(true);
}


void ChatInput::onEnter()
{
	Layer::onEnter();

	Size winSize = Director::getInstance()->getWinSize();

	Size contentSize = getContentSize();


	// 显示EditBox
	if (getEditBox())
	{
		Sprite* ChatInputBg = Sprite::create(p_login_zhanghu);
		getEditBox()->setAnchorPoint(Vec2::ZERO);
		getEditBox()->setPosition(Point(ChatInputBg->getPositionX(), ChatInputBg->getPositionY() + ChatInputBg->getContentSize().height + 2.0f));
		this->addChild(getEditBox());
	}

	// 添加按钮，并设置其位置
	if (getMenuButton()->getChildrenCount()>0)
	{
		//        CCLog("size=%d",getMenuButton()->getChildrenCount());
		float btnWidth = 100.0;
		//        CCLog("btnwidth=%f",btnWidth);
		/*Array* array = getMenuButton()->getChildren();
		Object* pObj = NULL;
		int i = 0;
		CCARRAY_FOREACH(array, pObj)
		{
			CCNode* node = (CCNode*)pObj;
			switch (i){
			case 0:
				//                    CCLog("btn width=%f",node->getContentSize().width);
				node->setAnchorPoint(CCPointZero);
				node->setPosition(ccp(getEditBox()->getPositionX() + getEditBox()->getContentSize().width + 2.0f, getEditBox()->getPositionY()));
				break;
			case 1:
				node->setPosition(ccp(getEditBox()->getPositionX() + 240, getEditBox()->getPositionY()));
				break;
			}
			i++;
		}*/
		this->addChild(getMenuButton());
	}

	thisRect = getRect();


}

void ChatInput::editBoxEditingDidBegin(EditBox* editBox)
{
	CCLog("editBox %p DidBegin !", editBox);
	//    editBox->setText("");
}

void ChatInput::editBoxEditingDidEnd(EditBox* editBox)
{
	CCLog("editBox %p DidEnd !", editBox);
}

void ChatInput::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
	//GlobalInfo::getInstance()->set_chatnrstr(text.c_str());
}

void ChatInput::editBoxReturn(EditBox* editBox)
{
	CCLog("->>>>>>>>>>>>>>>>> %d ", editBox->getTag());

	//    CCArray* arraylist = CCArray::create();
	//    arraylist->addObject(editBox);
	//    arraylist->addObject(CCString::create(editBox->getText()));
	/*if (editBox->getTag() == INPUT_NAME)
	{
		GlobalInfo::getInstance()->set_inputname_str(editBox->getText());
	}
	else
	{
		//        PublicDoFunc::toDoFuncN(LAYER_LOGIN,callfuncN_selector(LoginScenes::set_input_zhanghu),(CCNode*)editBox);
	}*/
}


CCRect ChatInput::getRect()
{
	//获取精灵区域大小
	return CCRectMake(getEditBox()->getPositionX() - getEditBox()->getContentSize().width  * getEditBox()->getAnchorPoint().x, getEditBox()->getPositionY() - getEditBox()->getContentSize().height* getEditBox()->getAnchorPoint().y, getEditBox()->getContentSize().width + 50.0f, getEditBox()->getContentSize().height);
}

void ChatInput::TouchEnded(Touch *pTouch, Event *pEvent)
{
	Point point = pTouch->getLocation();
	//    CCLog("%f,%f",thisRect.size.width,thisRect.size.height);
	point = this->convertToNodeSpace(point);
	if (this->thisRect.containsPoint(point) == false)
	{
		//播放点击音效
		this->removeFromParentAndCleanup(true);
	}
}

void ChatInput::onExit()
{
	CCLog("ChatInput::onExit");
	Layer::onExit();
}
//#endif