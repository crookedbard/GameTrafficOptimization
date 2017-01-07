#include "MainmenuScene.h"

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool MainMenu::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! Layer::init());

		// Enable touch/click actions
		this->setTouchEnabled(true);

		// Enable keypad/keyboard actions
		this->setKeypadEnabled(true);

		// Main menu with 3 items
		auto item1 = MenuItemFont::create( "New Game", this, menu_selector(MainMenu::onNewGame) );
		auto item2 = MenuItemFont::create( "Options", this, menu_selector(MainMenu::onOptions) );
		auto item3 = MenuItemFont::create( "Quit", this, menu_selector(MainMenu::onQuit) );
    
		// combine to form a menu and allign Vertically
		auto menu = Menu::create( item1, item2, item3, NULL );
		menu->alignItemsVertically();
    
		// add this to the layer
		this->addChild( menu, 1 );

        bRet = true;
    } while (0);

    return bRet;
}


void MainMenu::onNewGame(Ref* pSender)
{
	// Start the game 
	Director::getInstance()->replaceScene(CharacterCreation::createScene());
}


void MainMenu::onOptions(Ref* pSender)
{
}


void MainMenu::onQuit(Ref* pSender)
{
	// Exit game
	Director::getInstance()->end();
}

void MainMenu::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event){

	switch (keycode){
	case EventKeyboard::KeyCode::KEY_BACK:
		Director::getInstance()->end();
		break;
	case EventKeyboard::KeyCode::KEY_MENU:

		MessageBox("message goes here", "MSG TITLE");
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:

		MessageBox("message goes here", "MSG TITLE");
		break;
	}
}