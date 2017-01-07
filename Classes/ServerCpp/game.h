#ifndef __GAME_H__
#define __GAME_H__

//maybe later I will add this to Utils.h
#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

//Resolve additional dependancies
#include <iostream>
#include <string>

enum Facing {
	kFacingDownLeft,
	kFacingDownRight,
	kFacingUpLeft,
	kFacingUpRight
};

enum Direction {
	kDirectionDownLeft,
	kDirectionDownRight,
	kDirectionUpLeft,
	kDirectionUpRight
};

enum Gender {
	kGenderFemale,
	kGenderMale
};

enum State {
	kStateIdle,
	kStateMove,
	kStateAttack
};

enum Dimension{
	kBackground = -1,
	kMiddleground,
	kForeground
};

//Put any game variables here, "GM CONSTANTS"
class game
{
	public:
		//Declare colors below:
		static int c_black;
		static int c_blue;
		static int c_green;
		static int c_cyan;
		static int c_red;
		static int c_magenta;
		static int c_brown;
		static int c_lightgrey;
		static int c_darkgrey;
		static int c_lightblue;
		static int c_lightgreen;
		static int c_lightcyan;
		static int c_lightred;
		static int c_lightmagenta;
		static int c_yellow;
		static int c_white;
		static int c_blink;
};

//Include protection
#endif // __GAME_H__