#ifndef SFPLAYER_H
#define SFPLAYER_H

#include <memory>

class SFPlayer {
private:	
	SFPlayer Player();	
	int Charge;

public: 
	SFPlayer();
	~SFPlayer();
	
	int			GetCharge();

	//const char*	GetPlayerSprite();
	void		ResetCharge();
	void		IncrementCharge();
};

#endif


