#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <list>     // Pull in list
#include <string>

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
	
private:
    bool is_running;
	
	int chargelvl;
   
	shared_ptr<SFWindow> 	window;

    shared_ptr<SFAsset> 	player;
    
    list<shared_ptr<SFAsset> > projectiles;
    list<shared_ptr<SFAsset> > aliens;
    list<shared_ptr<SFAsset> > coins;
	list<shared_ptr<SFAsset> > walls;
};

#endif
