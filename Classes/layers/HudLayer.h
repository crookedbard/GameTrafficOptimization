#ifndef __HUDLAYER_H_
#define __HUDLAYER_H_

#ifdef USE_HUD_CLASS

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CocosGUI.h"
#include <sstream>
#include <iomanip>
#include <vector>

class HudLayer : public cocos2d::Layer {
public:
	virtual bool init();

	CREATE_FUNC(HudLayer);
};

#endif // USE_HUD_CLASS
#endif // __HUDLAYER_H_
