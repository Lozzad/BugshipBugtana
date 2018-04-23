#ifndef SFPLAYER_H
#define SFPLAYER_H

#include <memory>

class SFPlayer {
private:	
	SFPlayer Player();	
	int Charge;
	const int FULL_CHARGE = 120;

public: 
	SFPlayer();
	~SFPlayer();
	
	bool		CanFire();	
	int			GetCharge();

	//const char*	GetPlayerSprite();
	void		ResetCharge();
	void		IncrementCharge();
};

#endif


