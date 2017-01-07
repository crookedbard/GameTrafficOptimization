#ifndef __CHARACTERCREATIONLAYER_H_
#define __CHARACTERCREATIONLAYER_H_

#include "GameController.h"

enum {
	kCharacterSlot1,
	kCharacterSlot2,
	kCharacterSlot3,
	kMenuCreate,
	kMenuSelectDelete,
	kMenuCreateCharacter,
	kTextFieldPlayerName,
	kCursorPlayerName,
	kSpriteGender,
	kSpriteHairModal,
	kSpriteHairColor,
	kSpriteSkinColor,
	kSpritePlayer
};

enum {
	kBtnBrown,
	kBtnGreen,
	kBtnPink,
	kBtnRed,
	kBtnYellow,
	kBtnBlue,
	kBtnViolet,
	kBtnCyan,
	kBtnWhite,
	kBtnBlack
};

enum {
	kBtnHair1 = 20,
	kBtnHair2,
	kBtnHair3,
	kBtnHair4,
	kBtnHair5,
	kBtnHair6,
	kBtnHair7,
	kBtnHair8,
	kBtnHair9,
	kBtnHair10,
	kBtnHair11,
	kBtnHair12,
	kBtnHair13,
	kBtnHair14,
	kBtnHair15,
	kBtnHair16,
	kBtnHair17,
	kBtnHair18,
	kBtnHair19,
	kBtnHair20
};

enum {
	kBtnFemale = 40,
	kBtnMale
};

enum {
	kBtnToggle = 48,
	kBtnToggleClicked
};

enum {
	kBtnSkinWhite = 42,
	kBtnSkinBrown,
	kBtnSkinYellow,
	kBtnSkinGreen,
	kBtnSkinSkeleton,
	kBtnSkinPanda
};

class CharacterCreationLayer : public cocos2d::Layer, public cocos2d::TextFieldDelegate, public GameController
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	//static cocos2d::Scene* createScene();

	//Handle key events
	//void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	//Handle touch events
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	// implement the "static node()" method manually
	CREATE_FUNC(CharacterCreationLayer);

	//TextFieldttf delegate begin
	virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF * sender, const char * text, size_t nLen);
	virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF * sender, const char * delText, size_t nLen);
	virtual bool onVisit(cocos2d::TextFieldTTF * sender, cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);
	
	CC_SYNTHESIZE(int, _slot, Slot);
	CC_SYNTHESIZE(std::string, _characterName,CharacterName);
	CC_SYNTHESIZE(int, _gender, Gender);
	CC_SYNTHESIZE(int, _hairModal, HairModal);
	CC_SYNTHESIZE(int, _hairColor, HairColor);
	CC_SYNTHESIZE(int, _skinColor, SkinColor);
	// functions for click even handling
	void onCreate(cocos2d::Ref* pSender);
	void onCancel(cocos2d::Ref* pSender);
	void toggleGender(cocos2d::Ref* pSender);
	void toggleHairModal(cocos2d::Ref* pSender);
	void toggleHairColor(cocos2d::Ref* pSender);
	void toggleSkinColor(cocos2d::Ref* pSender);

	//Dialog methods
	void hideDialog(); //callback = removes dialog
	void showDialog(std::string str);

private:
	cocos2d::SpriteBatchNode *_spriteBatchNode;
	//std::string _name;
	//byte _gender;
	//int _hairModal;
	//int _hairColor;
	//int _skinColor;
	Dialog *_dialog;

	//void _createCharacter(cocos2d::Ref* pSender);
};

#endif // __CHARACTERCREATIONLAYER_H_
