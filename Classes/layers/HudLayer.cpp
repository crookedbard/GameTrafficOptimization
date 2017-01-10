#ifdef USE_HUD_CLASS
//#include "layers\HudLayer.h"
#include "HudLayer.h"


USING_NS_CC;
using namespace cocos2d::ui;

const std::string SETTINGS_FILENAME = "Settings.txt";
const std::string FONT_NAME = "fonts/arial.ttf";
const int FONT_SIZE = 15;
const int FONT_SIZE_MESSAGE = 14;
const int DEPTH_HUD = 1;
const int MAX_MESSAGE_COUNT = 20;

bool HudLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		bRet = true;
	} while (0);

	return bRet;
}
#endif // USE_HUD_CLASS
