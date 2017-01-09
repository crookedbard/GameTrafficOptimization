#ifndef __HUDLAYER_H_
#define __HUDLAYER_H_

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
#include "ui\CocosGUI.h"
#include <sstream>
#include <iomanip>
#include <vector>

class HudLayer : public cocos2d::Layer {
public:
	virtual bool init();

	CREATE_FUNC(HudLayer);
};

#endif // __HUDLAYER_H_