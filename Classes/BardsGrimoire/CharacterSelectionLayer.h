#ifndef __CHARACTERSELECTIONLAYER_H_
#define __CHARACTERSELECTIONLAYER_H_

#include "GameController.h"
#include "CharacterCreationLayer.h"

/*enum {
	kCharacterSlot1,
	kCharacterSlot2,
	kCharacterSlot3,
	kMenuCreate,
	kMenuSelectDelete
};*/

struct CharacterInSlot
{
	bool isEmpty = true;
	int charID = -1;
	std::string name = "";
	int character = 0;
	int gender = 0;
	int hairModal = 0;
	int hairColor = 0;
};

class CharacterSelectionLayer : public cocos2d::Layer, public GameController
{
public:

	~CharacterSelectionLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	//~CharacterSelectionLayer();
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::Scene* createScene(cocos2d::Layer *layer);

	//Handle key events
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	//Handle touch events
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	// functions for click even handling
	void onCreate(cocos2d::Ref* pSender);
	void onSelect(cocos2d::Ref* pSender);
	void onDelete(cocos2d::Ref* pSender);
	void onBack(cocos2d::Ref* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(CharacterSelectionLayer);

	void selectSlot();
	void setSlotData(CharacterInSlot slotData[3]);
	void addCharacter(int charID);
	void deleteCharacter();
	void refreshSlotData();
	//void testCallback(cocos2d::Ref* pSender);
	
	

	//Dialog methods
	virtual void showDialog(std::string str);
	void showDialogCallbackLogin(std::string text);
	//void showDialogCallbackLoading(std::string text);

	CC_SYNTHESIZE(int, _slotSelected, SlotSelected);
	CC_SYNTHESIZE(CharacterCreationLayer *, _characterCreationLayer, CharacterCreationLayer);
private:
	//int _slotSelected;
	//cocos2d::SpriteFrameCache *_spriteFrameCache;
	cocos2d::SpriteBatchNode *_spriteBatchNode;

	//CharacterCreationLayer *_characterCreationLayer;
	//characterInSlot *_slotData;
	//std::vector<CharacterInSlot> _slotData;
	//CharacterInSlot *_slotData;//= new CharacterInSlot[3];
	CharacterInSlot _slotData[3];
	Dialog *_dialog;

	void _returnToLogin(cocos2d::Ref* pSender);
	//void _createCharacter(cocos2d::Ref* pSender);
	void _deleteCharacter(cocos2d::Ref* pSender);
};

#endif // __CHARACTERSELECTIONLAYER_H_