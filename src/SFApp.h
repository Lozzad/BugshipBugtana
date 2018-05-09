#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <list>     // Pull in list
#include <string>
//#include <ctime>	//for rand (seemingly not needed?)

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"

/**
 * Represents the StarshipFontana application.  It has responsibilities for:
 * 1. Process game events
 * 2. Update game world
 * 3. Render game world
 */
enum BUILDER_STATES {	BUILDER_LEFT, BUILDER_RIGHT, BUILDING, STUNNED };

class SFApp {
public:
    SFApp(std::shared_ptr<SFWindow>);
    ~SFApp();
    
	void 	OnEvent(SFEvent &);
    void 	StartMainLoop();
    void 	OnUpdate();
    void 	OnRender();

	void	FireProjectile();
	void	DropCoin(Point2 center);
	int		RandomNumber(int scope);
	void	IncreaseShotSpeed();
	void	DecreaseShotSpeed();
	void	RepairWall();
	void	SpawnSpider();
	
private:
    bool is_running, playerNorth, playerSouth, playerEast, playerWest;

	int chargelvl, numCoins, maxCharge, builderState, buildCharge, queenCharge, queenChargeMax, queenHealth, stunTimer, shotLvl;

	shared_ptr<SFWindow> 	window;

    shared_ptr<SFAsset> 	player;
	shared_ptr<SFAsset>		door;
	shared_ptr<SFAsset>		builder;
	shared_ptr<SFAsset>		builderBody;
	shared_ptr<SFAsset>		queen;
	shared_ptr<SFAsset>		coinUI;
	shared_ptr<SFAsset>		lvlUI;
    
    list<shared_ptr<SFAsset> > 	projectiles;
	list<shared_ptr<SFAsset> > 	webs;
    list<shared_ptr<SFAsset> > 	spiders;
    list<shared_ptr<SFAsset> > 	coins;
	list<shared_ptr<SFAsset> > 	walls;

	Mix_Music *bgMusic = NULL;
	Mix_Chunk *mothLaser = NULL;
};

#endif
