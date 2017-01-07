#ifndef __HUD_LAYER_H__
#define __HUD_LAYER_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "MainmenuScene.h"
#include "Utils.h"
#include "39dll.h"
#include "ChatTableView.h"
#include <thread>
//#include "VisibleRect.h"

//Message constants
/*#define MSG_SERVER 0 //unused
#define MSG_LOGIN 1
#define MSG_REGISTER 2
#define MSG_CHANGESCENE 3 //request change scene (mainly when pressed esc key)
#define MSG_PLAYER 4
#define MSG_OTHERPLAYER 5
#define MSG_CHARACTERSELECTION 6
#define MSG_TEXT 50 //for test
*/
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class HUDLayer : public Layer, public EditBoxDelegate//, public TableViewDataSource, public TableViewDelegate
{
public:
    virtual bool init();

	Size visibleSize;
	Point origin;
	bool connected;
	double serverSocket;

	void DrawHUD();
	void OnMenuClose(Ref* pSender);
	void OnMenuWifi(Ref* pSender);

	virtual void editBoxReturn(EditBox* editBox);
	//virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	void packetListener();
	void packetHandler();
	void displayMsg(string msg);
	thread _thread;
	mutex _mutex;

	ChatTableView *chat;

	/*virtual void scrollViewDidScroll(ScrollView* view) {};
	virtual void scrollViewDidZoom(ScrollView* view) {}
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);*/
    CREATE_FUNC(HUDLayer);
};


#endif // __HUD_LAYER_H__
