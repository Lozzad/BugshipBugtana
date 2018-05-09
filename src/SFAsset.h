#ifndef SFASSET_H
#define SFASSET_H

#include <string>
#include <memory>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFWindow.h"
#include "SFBoundingBox.h"

/**
 * We could create SFPlayer, SFProjectile and SFAsset which are subclasses
 * of SFAsset.  However, I've made an implementation decision to use this
 * enum to mark the type of the SFAsset.  If we add more asset types then
 * the subclassing strategy becomes a better option.
 */
enum SFASSETTYPE { SFASSET_DEAD, SFASSET_PLAYER, SFASSET_PROJECTILE, SFASSET_WEBBING, SFASSET_SPIDER, SFASSET_COIN, SFASSET_WALL, SFASSET_DAM_WALL, SFASSET_DOOR, SFASSET_BUILDER, SFASSET_QUEEN, SFUI_COINS, SFUI_LVL, SFASSET_WIN };

class SFAsset {
public:
    SFAsset(const SFASSETTYPE, const std::shared_ptr<SFWindow>);
    SFAsset(const SFAsset&);
    virtual ~SFAsset();

    virtual void      	SetPosition(Point2 &);
    virtual Point2    	GetPosition();
    virtual Point2    	GetCenter();
    virtual void      	OnRender();
    virtual void      	GoEast();
    virtual void      	GoWest();
    virtual void      	GoNorth();
    virtual void		GoSouth();
    virtual void      	SetNotAlive();
	virtual void		SetWinAlive();
    virtual bool      	IsAlive();
	virtual void		DamageWall();
	virtual bool		IsDamaged();
	virtual void		RepairWall();
	virtual	void		Charge(int Charge, int max);
	virtual void      	HandleCollision();

	virtual void 		HitWall( bool north, bool south, bool east, bool west);
	

    virtual bool        CollidesWith(shared_ptr<SFAsset>);
    virtual shared_ptr<SFBoundingBox> 	GetBoundingBox();
	virtual void		LoadFromRenderedText ( string textureText, SDL_Color textColor);
	virtual void		UpdateCoins( int numCoins );
	virtual void		UpdateLevel( int shotLvl );
    
private:   
	const float					SPEED = 4.0f;	
	shared_ptr<SFPlayer>		SFPlayer1;	
	SDL_Texture             *	sprite;
    shared_ptr<SFBoundingBox>   bbox;
    SFASSETTYPE                 type;
    std::shared_ptr<SFWindow>   sf_window;
	SDL_Color 					textColor;
	TTF_Font* 					gFont;
};

#endif
