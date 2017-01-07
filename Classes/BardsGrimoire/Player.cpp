#include "Player.h"
#include "Players.h"
#include "Utils.h"
//#include "GameLayer.h"

USING_NS_CC;
using namespace std;

//Action* player::actionMoveDL;
//Action* player::actionMoveDR;
//Action* player::actionMoveUL;
//Action* player::actionMoveUR;

//GameLayer* player::gameLayer;
//Action* player::actionMove;

player::player(Race race, Gender gender, Facing facing)
{
	//_screenSize = CCDirector::sharedDirector()->getWinSize();
	//init basic stuff
	//_id = id;
	//_name = name;
	_race = race;
	_gender = gender;
	_facing = facing;
	//_x = 0;
	//_y = 0;

	players::list.push_back(this);

	//Print success message
	//CCLOG("%s", "New player joined the server: " + _name);
}

player::~player()
{
	//CC_SAFE_RELEASE(moveAction);

	//Remove from lists
	for (unsigned int i = 0; i<players::list.size(); i++)
	{
		if (players::list[i] == this)
		{
			players::list.erase(players::list.begin() + i);
		}
	}
}

player* player::create(Race race, Gender gender, Facing facing) {
	player * sprite = new player(race, gender, facing);
	bool flip = false;
	//char g;
	char s;
	if (facing == kFacingDownRight || facing == kFacingUpRight)flip = true;
	//(gender == kGenderFemale) ? g = 'f' : g = 'm';
	//(facing == kFacingDownRight || facing == kFacingDownLeft) ? s = 'd' : s = 'u';
	(facing == kFacingDownRight || facing == kFacingDownLeft) ? s = '0' : s = '1';
	//this->setSpriteFrame("plr_" + to_string(_character) + "_" + to_string(_gender) + "_" + facing + "_0_0");
	//if (sprite && sprite->initWithSpriteFrameName("char_" + to_string(character + 1) + "_" + g + "_" + s)) {
	if (sprite && sprite->initWithSpriteFrameName("plr_" + to_string(race) + "_" + to_string(gender) + "_" + s + "_0_0")) {
		sprite->setAnchorPoint(Point(0.5, 1));
		if (flip)sprite->setFlippedX(true);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

/*void player::initialize(GameLayer *layer){
	_gameLayer = layer;
}*/

void player::initPlayer(int id, string name, int hairModal, int hairColor, int x, int y)
{
	_id = id;
	_name = name;
	_hairModal = hairModal;
	_hairColor = hairColor;
	_currentPos.x = x;
	_currentPos.y = y;
	_nextPos.x = x;
	_nextPos.y = y;
	_state = kStateIdle;

	_currentFrame = 0;
	_lastUpdateTime = 0;
	//_stepOne = false;
	/*auto actionMove = CallFunc::create([&](){
		int byX = 0;
		int byY = 0;
		if (_currentPos.x == _nextPos.x && _currentPos.y < _nextPos.y) { byX = -8; byY = 4; } //DL
		if (_currentPos.x < _nextPos.x && _currentPos.y == _nextPos.y) { byX = 8; byY = 4; } //DR
		if (_currentPos.x > _nextPos.x && _currentPos.y == _nextPos.y) { byX = -8; byY = -4; } //UL
		if (_currentPos.x == _nextPos.x && _currentPos.y > _nextPos.y) { byX = 8; byY = -4; } //UR

		auto move = MoveTo::create(static_cast<float>(0.5), Point(getPosition().x + byX, getPosition().y + byY));
		//return move;
		this->runAction(move);
	});
	
	auto actionMoveDone = CallFunc::create(this, callfunc_selector(player::moveFinished));
	moveAction = Sequence::create(Repeat::create(actionMove, 4), actionMoveDone, NULL);
	moveAction->retain();*/
	//auto actionTest = Repeat::create(Sequence::create(DelayTime::create(static_cast<float>(0.03)), CallFunc::create(this, callfunc_selector(player::movingDL)), NULL), 16);
	//auto actionOld = MoveBy::create(static_cast<float>(0.48), Vec2(-32, -16));

	/*actionMoveDL = Sequence::create(actionTest, CallFunc::create(this, callfunc_selector(player::moveFinished)), NULL);
	actionMoveDL->retain();
	actionMoveDR = Sequence::create(MoveBy::create(static_cast<float>(0.48), Vec2(32, -16)), CallFunc::create(this, callfunc_selector(player::moveFinished)), NULL);
	actionMoveDR->retain();
	actionMoveUL = Sequence::create(MoveBy::create(static_cast<float>(0.48), Vec2(-32, 16)), CallFunc::create(this, callfunc_selector(player::moveFinished)), NULL);
	actionMoveUL->retain();
	actionMoveUR = Sequence::create(MoveBy::create(static_cast<float>(0.48), Vec2(32, 16)), CallFunc::create(this, callfunc_selector(player::moveFinished)), NULL);
	actionMoveUR->retain();*/

	//Vector<SpriteFrame*> animFrames(15);

	//Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
	//auto anim = RepeatForever::create(Animate::create(animation));

	/*actionMoveDL = Sequence::create(Repeat::create(Sequence::create(DelayTime::create(static_cast<float>(0.03)), CallFunc::create(CC_CALLBACK_0(player::movingDL, this)), NULL), 16), CallFunc::create(CC_CALLBACK_0(player::moveFinished,this)), NULL);
	actionMoveDL->retain();
	actionMoveDR = Sequence::create(Repeat::create(Sequence::create(DelayTime::create(static_cast<float>(0.03)), CallFunc::create(this, callfunc_selector(player::movingDR)), NULL), 16), CallFunc::create(this, callfunc_selector(player::moveFinished)), NULL);
	actionMoveDR->retain();
	actionMoveUL = Sequence::create(Repeat::create(Sequence::create(DelayTime::create(static_cast<float>(0.03)), CallFunc::create(this, callfunc_selector(player::movingUL)), NULL), 16), CallFunc::create(this, callfunc_selector(player::moveFinished)), NULL);
	actionMoveUL->retain();
	actionMoveUR = Sequence::create(Repeat::create(Sequence::create(DelayTime::create(static_cast<float>(0.03)), CallFunc::create(this, callfunc_selector(player::movingUR)), NULL), 16), CallFunc::create(this, callfunc_selector(player::moveFinished)), NULL);
	actionMoveUR->retain();*/
	//auto test = MoveBy
	//actionMove = Sequence::create(Repeat::create(Sequence::create(DelayTime::create(static_cast<float>(0.25)), CallFunc::create(this, callfunc_selector(player::move)),NULL), 4), CallFunc::create(this, callfunc_selector(player::moveFinished)),NULL);
	//actionMove->retain();

	setLocation(Point(x,y));
}

void player::move(){
	switch (_facing){
	case kFacingDownLeft: this->setPosition(Point(this->getPosition().x - 2, this->getPosition().y - 1));
		break;
	case kFacingDownRight: this->setPosition(Point(this->getPosition().x + 2, this->getPosition().y - 1));
		break;
	case kFacingUpLeft: this->setPosition(Point(this->getPosition().x - 2, this->getPosition().y + 1));
		break;
	case kFacingUpRight: this->setPosition(Point(this->getPosition().x + 2, this->getPosition().y + 1));
		break;
	}
	//gameLayer->setViewPoint2(this->getPosition());
	
	_currentFrame++;

	if (_currentFrame == 0 || _currentFrame == 4 || _currentFrame == 8 || _currentFrame == 12 || _currentFrame == 16){
		char facing;
		(_facing == kFacingDownLeft || _facing == kFacingDownRight) ? facing = '0' : facing = '1';
		int frame = floor(_currentFrame / WALK_FRAMES);
		if (frame != 4){
			this->setSpriteFrame("plr_" + to_string(_race) + "_" + to_string(_gender) + "_" + facing + "_1_" + to_string(frame));
		}
		else{
			this->setSpriteFrame("plr_" + to_string(_race) + "_" + to_string(_gender) + "_" + facing + "_0_0");
		}
		
		if (_facing == kFacingDownRight || _facing == kFacingUpRight){ this->setFlippedX(true); }
		else{ this->setFlippedX(false); }
	}
	//_gameLayer->setViewPointPlrCenter();
	if (_currentFrame == 16){ 
		_currentFrame = 0; 
		_state = kStateIdle;
		_currentPos = _nextPos;
	}
}
/*
void player::movingDL(){

	char facing;
	bool flip;
	(_facing == kFacingDownLeft || _facing == kFacingDownRight) ? facing = '0' : facing = '1';
	(_facing == kFacingDownRight || _facing == kFacingUpRight) ? flip = true : flip = false;
	this->setSpriteFrame("plr_" + to_string(_character) + "_" + to_string(_gender) + "_" + facing + "_1_0");
	if (flip)this->setFlippedX(true);

	this->setPosition(Point(this->getPosition().x - 2, this->getPosition().y - 1));
	//gameLayer->setViewPointPlrCenter();
	//gameLayer->setPosition(Point(100,100));
}

void player::movingDR(){
	this->setPosition(Point(this->getPosition().x + 2, this->getPosition().y - 1));
	//gameLayer->setViewPointPlrCenter();
	//gameLayer->setPosition(Point(-500, -500));
}

void player::movingUL(){
	this->setPosition(Point(this->getPosition().x - 2, this->getPosition().y + 1));
	//gameLayer->setViewPointPlrCenter();
}

void player::movingUR(){
	this->setPosition(Point(this->getPosition().x + 2, this->getPosition().y + 1));
	//gameLayer->setViewPointPlrCenter();
	//int t = visibleSize.width;
}*/

/*void player::move(){
	
	if (_currentPos.x == _nextPos.x && _currentPos.y < _nextPos.y) { this->setPosition(Point(this->getPosition().x - 4, this->getPosition().y - 2));return; } 
	if (_currentPos.x < _nextPos.x && _currentPos.y == _nextPos.y) { this->setPosition(Point(this->getPosition().x + 4, this->getPosition().y - 2)); return; } //DR
	if (_currentPos.x > _nextPos.x && _currentPos.y == _nextPos.y) { this->setPosition(Point(this->getPosition().x - 4, this->getPosition().y + 2)); return; } //UL
	if (_currentPos.x == _nextPos.x && _currentPos.y > _nextPos.y) { this->setPosition(Point(this->getPosition().x + 4, this->getPosition().y + 2)); return; } //UR
}*/

/*void player::moveEnd(){
	_state = kStateIdle;
	_currentPos = _nextPos;
}*/

void player::setLocation(Point position){
	int x = (position.x - position.y) * 64 / 2;
	int y = ((position.x + position.y) * 32 / 2 + 16 - 58);
	this->setPosition(GLCORDS(Point(x, y)));
}

void player::update(float dt) {
	_lastUpdateTime += dt;

	switch (_state){
	case kStateIdle:
		//_gameLayer->checkPressed();
			if(steps.empty())return;
			switch (steps[0]){
			case kDirectionDownLeft: _nextPos.y++; _facing = kFacingDownLeft;
				break;
			case kDirectionDownRight: _nextPos.x++; _facing = kFacingDownRight;
				break;
			case kDirectionUpLeft: _nextPos.x--; _facing = kFacingUpLeft;
				break;
			case kDirectionUpRight: _nextPos.y--; _facing = kFacingUpRight;
				break;
			}
			steps.erase(steps.begin());
			_state = kStateMove;
			_lastUpdateTime = dt;
		break;
	case kStateMove: if (_lastUpdateTime >= 0.03){ move(); _lastUpdateTime -= 0.03f; }
		break;
	}

}

/*void player::update(float dt) {
	//CCLOG("update since last: %f",dt);

	if (_state != kStateIdle || steps.empty())return;
	switch (steps[0]){
	case kDirectionDownLeft: _nextPos.y++; _facing = kFacingDownLeft;
		break;
	case kDirectionDownRight: _nextPos.x++; _facing = kFacingDownRight;
		break;
	case kDirectionUpLeft: _nextPos.x--; _facing = kFacingUpLeft;
		break;
	case kDirectionUpRight: _nextPos.y--; _facing = kFacingUpRight;
		break;
	}
	steps.erase(steps.begin());
	_state = kStateMove;

	if (_currentPos.x == _nextPos.x && _currentPos.y < _nextPos.y) { this->runAction(actionMoveDL); return; } //DL
	if (_currentPos.x < _nextPos.x && _currentPos.y == _nextPos.y) { this->runAction(actionMoveDR); return; } //DR
	if (_currentPos.x > _nextPos.x && _currentPos.y == _nextPos.y) { this->runAction(actionMoveUL); return; } //UL
	if (_currentPos.x == _nextPos.x && _currentPos.y > _nextPos.y) { this->runAction(actionMoveUR); return; } //UR
	
	//this->runAction(actionMove);
}*/

/*void player::move(Point position) {
	int x = (position.x - position.y) * 64 / 2;
	int y = ((position.x + position.y) * 32 / 2 + 16);
	this->setPosition(GLCORDS(Point(x, y)));
}
*/
