#include "OtherPlayer.h"

#include "Utils.h"

USING_NS_CC;
using namespace std;

OtherPlayer::OtherPlayer(Race race, Gender gender, Facing facing)
{
	_race = race;
	_gender = gender;
	_facing = facing;

	players::list2.pushBack(this);
}

OtherPlayer::~OtherPlayer()
{
	//CC_SAFE_RELEASE(moveAction);
	
	//Remove from lists
	players::list2.eraseObject(this);
	/*for (unsigned int i = 0; i<players::list.size(); i++)
	{
		if (players::list[i] == this)
		{
			players::list.erase(players::list.begin() + i);
		}
	}*/
}

OtherPlayer* OtherPlayer::create(Race race, Gender gender, Facing facing) {
	OtherPlayer * sprite = new OtherPlayer(race, gender, facing);
	bool flip = false;
	char g;
	char s;
	if (facing == kFacingDownRight || facing == kFacingUpRight)flip = true;
	(gender == kGenderFemale) ? g = 'f' : g = 'm';
	(facing == kFacingDownRight || facing == kFacingDownLeft) ? s = 'd' : s = 'u';
	if (sprite && sprite->initWithSpriteFrameName("char_" + to_string(race + 1) + "_" + g + "_" + s)) {
		sprite->setAnchorPoint(Point(0.5, 0));
		if (flip)sprite->setFlippedX(true);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

/*bool OtherPlayer::init(){
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Sprite::init());



		bRet = true;
	} while (0);

	return bRet;
}*/

void OtherPlayer::initPlayer(int id, string name, int hairModal, int hairColor, int x, int y)
{
	Id = id;
	_name = name;
	_hairModal = hairModal;
	_hairColor = hairColor;
	_currentPos.x = x;
	_currentPos.y = y;
	_nextPos.x = x;
	_nextPos.y = y;
	_state = kStateIdle;

	_currentFrame = 0;

	/*actionMoveDL = Sequence::create(Repeat::create(Sequence::create(DelayTime::create(static_cast<float>(0.03)), CallFunc::create(CC_CALLBACK_0(player::movingDL, this)), NULL), 16), CallFunc::create(CC_CALLBACK_0(player::moveFinished, this)), NULL);
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

	setLocation(Point(x, y));
}



/*void player::move(){

if (_currentPos.x == _nextPos.x && _currentPos.y < _nextPos.y) { this->setPosition(Point(this->getPosition().x - 4, this->getPosition().y - 2));return; }
if (_currentPos.x < _nextPos.x && _currentPos.y == _nextPos.y) { this->setPosition(Point(this->getPosition().x + 4, this->getPosition().y - 2)); return; } //DR
if (_currentPos.x > _nextPos.x && _currentPos.y == _nextPos.y) { this->setPosition(Point(this->getPosition().x - 4, this->getPosition().y + 2)); return; } //UL
if (_currentPos.x == _nextPos.x && _currentPos.y > _nextPos.y) { this->setPosition(Point(this->getPosition().x + 4, this->getPosition().y + 2)); return; } //UR
}*/

void OtherPlayer::moveFinished(){
	_state = kStateIdle;
	_currentPos = _nextPos;
}

void OtherPlayer::setLocation(Point position){
	int x = (position.x - position.y) * 64 / 2;
	int y = ((position.x + position.y) * 32 / 2 + 16);
	this->setPosition(GLCORDS(Point(x, y)));
}

void OtherPlayer::update(float dt) {
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

	/*int byX = 0;
	int byY = 0;
	if (_currentPos.x == _nextPos.x && _currentPos.y < _nextPos.y) { byX = -8; byY = 4; } //DL
	if (_currentPos.x < _nextPos.x && _currentPos.y == _nextPos.y) { byX = 8; byY = 4; } //DR
	if (_currentPos.x > _nextPos.x && _currentPos.y == _nextPos.y) { byX = -8; byY = -4; } //UL
	if (_currentPos.x == _nextPos.x && _currentPos.y > _nextPos.y) { byX = 8; byY = -4; } //UR

	auto move = MoveTo::create(static_cast<float>(0.25), Point(getPosition().x + byX, getPosition().y + byY));
	auto moveDone = CallFunc::create(this, callfunc_selector(player::moveFinished));
	auto moveAction = Sequence::create(Repeat::create(move, 4), moveDone, NULL);
	this->runAction(moveAction);*/

	//CCLOG("%f, %f", _nextPos.x, _nextPos.y);

	//if (_currentPos.x == _nextPos.x && _currentPos.y < _nextPos.y) { this->runAction(actionMoveDL); return; } //DL
	//if (_currentPos.x < _nextPos.x && _currentPos.y == _nextPos.y) { this->runAction(actionMoveDR); return; } //DR
	//if (_currentPos.x > _nextPos.x && _currentPos.y == _nextPos.y) { this->runAction(actionMoveUL); return; } //UL
	//if (_currentPos.x == _nextPos.x && _currentPos.y > _nextPos.y) { this->runAction(actionMoveUR); return; } //UR

	//this->runAction(actionMove);
}

/*void OtherPlayer::move(Point position) {
	int x = (position.x - position.y) * 64 / 2;
	int y = ((position.x + position.y) * 32 / 2 + 16);
	this->setPosition(GLCORDS(Point(x, y)));
}*/

