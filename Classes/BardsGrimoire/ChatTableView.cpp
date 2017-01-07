#include "ChatTableView.h"
//#include "VisibleRect.h"

USING_NS_CC;
USING_NS_CC_EXT;

ChatTableView::ChatTableView(Sprite* sp, float w, float h, vector<string> msglist)
{

	if (sp->getChildByTag(1) != NULL)
	{
		sp->removeChildByTag(1);
	}
	n_w = w;
	n_h = h;
	one_h = 16;
	n_msglist = msglist;
	//charsInLine = 30;

	pTableView = TableView::create(this, sp->getContentSize()/*VisibleRect::getVisibleRect().size*/, NULL);
	pTableView->setDirection(ScrollView::Direction::VERTICAL);
	//pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);

	pTableView->setDelegate(this);
	pTableView->setPosition(Point(sp->getContentSize().width*0.04, sp->getContentSize().height*0.14));
	pTableView->setViewSize(Size(sp->getContentSize().width*0.94, sp->getContentSize().height*0.80));
	pTableView->setContentSize(Size(sp->getContentSize().width*0.94, sp->getContentSize().height*0.80));

	pTableView->setBounceable(false);
	pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//pTableView->setContentOffset (pTableView->minContainerOffset());
	sp->addChild(pTableView, 1, 1);
	pTableView->reloadData();
	pTableView->setContentOffset(pTableView->maxContainerOffset());
}

void ChatTableView::addMsg(string text){

	n_msglist.push_back(text);
	pTableView->reloadData();
	pTableView->setContentOffset(pTableView->maxContainerOffset());
}

void ChatTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLog("tableCellTouched");

}

Size ChatTableView::cellSizeForTable(TableView *table)
{
	//计算出高度
	//    
	//    CCSize cellSize =CCSizeMake(n_w, 60);
	return Size(n_w, 60);
}
Size ChatTableView::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
	//    CCLOG("size forindex==%d",idx);
	if (idx < n_msglist.size() - 1)
	{
		idx += 1;
	}
	//Msgdata ms;
	string chatnr = n_msglist[idx];//"asdasdasdasda asdasd as das ";// n_msglist.at(idx).MsgText;
	//string ss1s = chatnr;

	int higth = -1;
	if (chatnr.size()>45)
	{
		higth = 28;
	}
	else
	{
		higth = 14;
	}
	return Size(n_w, higth);
}

TableViewCell* ChatTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *pcell = table->dequeueCell();
	if (!pcell)
	{
		pcell = new TableViewCell();
		pcell->autorelease();
	}

	pcell->removeAllChildren();

	//Msgdata ms;
	string playername = "PlayerName";// n_msglist.at(idx).MyInfo;
	string chattype = "Room";//n_msglist.at(idx).MsgType;
	string chatnr = n_msglist[idx];// "Text";//n_msglist.at(idx).MsgText;

	chattype = "[" + chattype + "]";
	playername = playername + ": ";
	LabelTTF* tf = LabelTTF::create(chattype.c_str(), "fonts/micross.ttf", 10);
	tf->setHorizontalAlignment(kCCTextAlignmentLeft); 
	tf->setColor(ccc3(250, 255, 240));
	tf->setAnchorPoint(Vec2::ZERO);
	pcell->addChild(tf);
	LabelTTF* tf2 = LabelTTF::create(playername.c_str(), "fonts/micross.ttf", 10);
	tf2->setHorizontalAlignment(kCCTextAlignmentLeft); 
	tf2->setColor(ccc3(200, 255, 255));
	tf2->setPosition(Point(tf->getContentSize().width + tf2->getContentSize().width / 2 * 1.1, tf->getContentSize().height / 2));
	pcell->addChild(tf2);

	string st1, st2, st3;
	if (chatnr.size()>45)
	{
		st1 = chatnr.substr(0, 45);
		st2 = chatnr.substr(45, chatnr.size());
		LabelTTF* tf3 = LabelTTF::create(st1.c_str(), "fonts/micross.ttf", 10);
		tf3->setHorizontalAlignment(kCCTextAlignmentLeft); 
		tf3->setColor(ccc3(240, 255, 240));
		tf3->setPosition(Point(tf->getContentSize().width + tf2->getContentSize().width + tf3->getContentSize().width / 2, tf->getContentSize().height / 2));
		pcell->addChild(tf3);
		if (st2.size()>0)
		{
			LabelTTF* tf4 = LabelTTF::create(st2.c_str(), "fonts/micross.ttf", 10);
			tf4->setHorizontalAlignment(kCCTextAlignmentLeft); 
			tf4->setColor(ccc3(240, 255, 240));
			tf4->setPosition(Point(tf->getPosition().x, tf->getPosition().y - tf4->getContentSize().height / 2 * 1.3));
			pcell->addChild(tf4);
		}

	}
	else
	{
		LabelTTF* tf3 = LabelTTF::create(chatnr.c_str(), "fonts/micross.ttf", 10);
		tf3->setHorizontalAlignment(kCCTextAlignmentLeft); 
		tf3->setColor(ccc3(240, 255, 240));
		tf3->setPosition(Point(tf->getContentSize().width + tf2->getContentSize().width + tf3->getContentSize().width / 2, tf->getContentSize().height / 2));
		pcell->addChild(tf3);
	}

	playername.clear();
	chattype.clear();
	chatnr.clear();
	if (idx == n_msglist.size())
	{
		n_msglist.clear();
	}

	return pcell;
}

ssize_t ChatTableView::numberOfCellsInTableView(TableView *table)
{
	int count = -1;
	//CCLOG("n_msglist.size() %d", n_msglist.size());
	if (n_msglist.size()>0)
	{
		count = n_msglist.size();
	}
	else
	{
		count = 0;
	}
	return count;
}

/*void ChatTableView::scrollViewDidZoom(ScrollView* view)
{
	CCLOG("didzoom");
}*/

void ChatTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

	  //  Sprite* note = (Sprite*)cell->getChildByTag(1);
	 //   MenuItemImage* image = (MenuItemImage*)note->getChildByTag(1);
	//    image->selected();
	CCLOG("Highlight");
}

/**
* Delegate to respond a table cell release event
*
* @param table table contains the given cell
* @param cell  cell that is pressed
*/
void ChatTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
	//    CCSprite* note = (CCSprite*)cell->getChildByTag(1);
	//    CCMenuItemImage* image = (CCMenuItemImage*)note->getChildByTag(1);
	//    image->unselected();


	CCLOG("Unhighlight");
}



ChatTableView::~ChatTableView(void)
{

}