#ifndef _ChatTableView_
#define _ChatTableView_

#include "extensions/cocos-ext.h"
#include "cocos2d.h"
#include <string>
//#include "../GameData/Msgdata.h"



using namespace std;



class ChatTableView :public cocos2d::Layer, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:

	vector<string> n_msglist;

	float n_w, n_h, one_h;
	//int charsInLine;
	cocos2d::extension::TableView* pTableView;

	ChatTableView(cocos2d::Sprite* sp, float w, float h, vector<string> msglist);
	~ChatTableView(void);



	//virtual void tableCellTouched(TableView* table, TableViewCell* cell);

	virtual cocos2d::Size cellSizeForTable(cocos2d::extension::TableView *table);

	//virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) = 0;

	//virtual unsigned int numberOfCellsInTableView(TableView *table);
	//virtual ssize_t numberOfCellsInTableView(TableView *table);
	//virtual Size tableCellSizeForIndex(TableView* table, unsigned int idx);
	//virtual void scrollViewDidScroll(ScrollView* view){};
	//virtual void scrollViewDidZoom(ScrollView* view);

	//高亮显示。选中效果
	virtual void tableCellHighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);

	/**
	* Delegate to respond a table cell release event
	*
	* @param table table contains the given cell
	* @param cell  cell that is pressed
	*/
	virtual void tableCellUnhighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);

	///////////////////////////
	//virtual bool init();

	//void toExtensionsMainLayer(Ref *sender);

	//CREATE_FUNC(ChatTableView);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);

	void addMsg(string text);
private:

};

#endif //_TIPS_FOR_STORY_
