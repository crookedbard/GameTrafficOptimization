#ifndef __GAMECONFIG_H__
#define __GAMECONFIG_H__

#define MAX_LETTERS 12

static const double port = 1337;
static const std::string ipAddress = "127.0.0.1";//vln: 192.168.1.73 ,darbo - 172.16.1.166//78.57.167.11//192.168.1.193//127.0.0.1//78.56.33.195

enum Race {
	kRaceHumanWhite,
	kRaceHumanBlack,
	kRaceHumanYellow,
	kRaceHumanGreen,
	kRaceSkeleton,
	kRacePanda,
	kRaceFish
};

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

enum Dimension {
	kBackground = -1,
	kMiddleground,
	kForeground
};

//define game notifications


#endif  //__GAMECONFIG_H__
