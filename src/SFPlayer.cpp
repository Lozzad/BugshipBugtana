#include "SFPlayer.h"

SFPlayer::SFPlayer() : Charge(Charge) {}

SFPlayer::~SFPlayer() {}

int SFPlayer::GetCharge() { 
	return Charge;
}

void SFPlayer::ResetCharge() {
	Charge = 0;
}

void SFPlayer::IncrementCharge() {
	if (Charge < 240) {
		Charge++;
	}
}




