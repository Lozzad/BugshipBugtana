#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : is_running(true), window(window) {
    int canvas_w = window->GetWidth();
    int canvas_h = window->GetHeight();					//-> for pointers, . for the obj
	
	//place the player	
	player = make_shared<SFAsset>(SFASSET_PLAYER, window);    
	auto player_pos = Point2(canvas_w / 2 - player->GetBoundingBox()->GetWidth() / 2, canvas_h - player->GetBoundingBox()->GetHeight());
	player->SetPosition(player_pos);
	

	//place the aliens
    const int number_of_aliens = 10; 
	for (int i = 0; i < number_of_aliens; i++) {
        // place an alien at width/number_of_aliens * i
        auto alien = make_shared<SFAsset>(SFASSET_ALIEN, window);
        auto pos = Point2((canvas_w / number_of_aliens) * i + alien->GetBoundingBox()->GetWidth() / 2, 100.0f);
        alien->SetPosition(pos);
        aliens.push_back(alien);
    }

	//place the walls
	const int number_of_walls = 10;
	for (int i = 0; i < number_of_walls; i++) {
		auto wall = make_shared<SFAsset>(SFASSET_WALL, window);
		auto pos = Point2((canvas_w / number_of_walls) * i + wall->GetBoundingBox()->GetWidth() / 2, 200.0f);
		wall->SetPosition(pos);
		walls.push_back(wall);
	}
}

SFApp::~SFApp() {}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
 	SFEVENT the_event = event.GetCode();
   	switch (the_event) {
   		case SFEVENT_QUIT:
       		is_running = false;
       		break;
   		case SFEVENT_UPDATE:
       		OnUpdate();
       		OnRender();
       		break;
		case SFEVENT_PLAYER_UP:
			player->GoNorth();
			break;
		case SFEVENT_PLAYER_DOWN:
			player->GoSouth();
			break;
		case SFEVENT_PLAYER_LEFT:
       		player->GoWest();
       		break;
		case SFEVENT_PLAYER_RIGHT:
       		player->GoEast();
       		break;
		case SFEVENT_FIRE:
			if (chargelvl >= 240) {
				FireProjectile();
				chargelvl = 0;
			}
			break;
    }
}

void SFApp::StartMainLoop() {
    SDL_Event event;
    while (SDL_WaitEvent(&event) && is_running) {
        
        // wrap an SDL_Event with our SFEvent
        SFEvent sfevent((const SDL_Event)event);
        
        // handle our SFEvent
        OnEvent(sfevent);
    }
}

void SFApp::OnUpdate() {
    
    // 1. Move / update game objects
    for (auto p : projectiles) {
        if (p->IsAlive()) {
			p->GoNorth();
		}
    }
	
	//increase weapon charge	
	chargelvl++;
	player->Charge(chargelvl);
	
    // coins
    for (auto c : coins) {
        c->GoSouth();
    }

    // enemies
    for (auto a : aliens) {
        // do something here
    }

	//walls
	for (auto w : walls) {
	}

    // 2. Detect collisions
    for (auto p : projectiles) {
        for (auto a : aliens) {
            if (p->CollidesWith(a)) {
                p->HandleCollision();
                a->HandleCollision();
				DropCoin(a->GetCenter());
            }
		}
		for (auto w : walls) {
			if (p->CollidesWith(w)) {
				p->HandleCollision();
				w->HandleCollision();
        	}
		}
    }
	for (auto w : walls) {
		if (w->CollidesWith(player)) {
			player->HandleCollision();
		}
	}
	for (auto c : coins) {
		if (c->CollidesWith(player)) {
			c->HandleCollision();
		}
	}	

    // 3. Remove dead aliens (the long way)
    list<shared_ptr<SFAsset>> tmpA;
    for (auto a : aliens) {
        if (a->IsAlive()) {
            tmpA.push_back(a);
        }
    }
    aliens.clear();
    aliens = list<shared_ptr<SFAsset>>(tmpA);
	
	//Remove dead walls too
	list<shared_ptr<SFAsset>> tmpW;
	for (auto w : walls) {
		if (w->IsAlive()) {
			tmpW.push_back(w);
		}
	}
	walls.clear();
	walls = list<shared_ptr<SFAsset>>(tmpW);
	
	//and projectiles
	list<shared_ptr<SFAsset>> tmpP;
	for (auto p : projectiles) {
		if (p->IsAlive()) {
			tmpP.push_back(p);
		}
	}
	projectiles.clear();
	projectiles = list<shared_ptr<SFAsset>>(tmpP);

	//and coins
	list<shared_ptr<SFAsset>> tmpC;
	for (auto c : coins) {
		if (c->IsAlive()) {
			tmpC.push_back(c);
		}
	}
	coins.clear();
	coins = list<shared_ptr<SFAsset>>(tmpC);
}

void SFApp::OnRender() {
    // 1. Clear visible content
    window->ClearScreen();

    // 2. Draw game objects off-screen
    player->OnRender();

    for (auto p : projectiles) {
        if (p->IsAlive()) { 
            p->OnRender(); 
        }
    }

    for (auto a : aliens) {
        if (a->IsAlive()) { 
            a->OnRender(); 
        }
    }

	for (auto w : walls) {
		if (w->IsAlive()) {
			w->OnRender();
		}
	}

    for (auto c : coins) {
        if (c->IsAlive()) {
			c->OnRender();
		}    
	}

    // 3. Switch the off-screen buffer to be on-screen
    window->ShowScreen();
}
		

void SFApp::FireProjectile() {
    auto bullet = make_shared<SFAsset>(SFASSET_PROJECTILE, window);
    auto v = player->GetCenter();
    auto pos = Point2(v.getX() - bullet->GetBoundingBox()->GetWidth() / 2, v.getY());
    bullet->SetPosition(pos);
    projectiles.push_back(bullet);
}

void SFApp::DropCoin(Point2 center) {
    auto coin = make_shared<SFAsset>(SFASSET_COIN, window);
    auto v = center;
	auto pos = Point2(v.getX() - coin->GetBoundingBox()->GetWidth() / 2, v.getY());
    coin->SetPosition(pos);
    coins.push_back(coin);
}
