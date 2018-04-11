#ifndef SFPLAYER_H
#define SFPLAYER_H

class SFPlayer {
private:	
	int Health, Charge;

public: 
	SFPlayer();
	~SFPlayer();
		
	int			GetCharge() const;
	int			GetHealth() const;

	const char*	GetPlayerSprite();
	void		ResetCharge();
	void		IncrementCharge();
};

#endif


