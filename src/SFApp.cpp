#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : chargelvl(0), numCoins(0), maxCharge(100), queenCharge(0), queenChargeMax(120), queenHealth(10), builderState(BUILDER_LEFT), stunTimer(0), buildCharge(0), shotLvl(1), playerNorth(false), playerSouth(false), playerWest(false), playerEast(false), is_running(true), window(window) {
    int canvas_w = window->GetWidth();
    int canvas_h = window->GetHeight();					//-> for pointers, . for the obj		
	
	//load sounds
	bgMusic = Mix_LoadMUS( "assets/bgMusic.wav" );
	if (bgMusic == NULL) {
		throw SFException ( "Failed to initialise music" );
	}
	mothLaser = Mix_LoadWAV( "assets/mothLaser.wav" );
	if (mothLaser == NULL) {
		throw SFException ( "Failed to load moth laser noise" );
	}
	
	//place the ui (in development)
	coinUI = make_shared<SFAsset>(SFUI_COINS, window);
	auto coinUI_pos = Point2( 10, 10);
	coinUI->SetPosition(coinUI_pos);
	
	lvlUI = make_shared<SFAsset>(SFUI_LVL, window);
	auto lvlUI_pos = Point2( 10, 20 + coinUI->GetBoundingBox()->GetHeight());
	lvlUI->SetPosition(lvlUI_pos);

	//place the win screen but dont show it til end (probably change this)
	winGame = make_shared<SFAsset>(SFASSET_WIN, window);
	auto win_pos = Point2(0,0);	
	winGame->SetPosition(win_pos);
	winGame->SetNotAlive();
	
	//place the player		
	player = make_shared<SFAsset>(SFASSET_PLAYER, window);    
	auto player_pos = Point2(canvas_w / 2 - player->GetBoundingBox()->GetWidth() / 2, canvas_h - player->GetBoundingBox()->GetHeight());
	player->SetPosition(player_pos);
	
	//place the exit
	door = make_shared<SFAsset>(SFASSET_DOOR, window);
	auto door_pos = Point2(canvas_w / 2 - door->GetBoundingBox()->GetWidth() / 2, door->GetBoundingBox()->GetHeight());
	door->SetPosition(door_pos);

	//place the queen
	queen = make_shared<SFAsset>(SFASSET_QUEEN, window);
	auto queen_pos = Point2(canvas_w / 2 - queen->GetBoundingBox()->GetWidth() / 2, 2*queen->GetBoundingBox()->GetHeight());
	queen->SetPosition(queen_pos);

	//place the builder
	builder = make_shared<SFAsset>(SFASSET_BUILDER, window);
	auto builder_pos = Point2(canvas_w / 2 - builder->GetBoundingBox()->GetWidth() / 2, 170.0f);
	builder->SetPosition(builder_pos);
	builderBody = make_shared<SFAsset>(SFASSET_SPIDER, window);
	builderBody->SetPosition(builder_pos);

	//place the walls
	const int depth_of_walls = 3;	
	const int number_of_walls = 15;
	for (int j = 0; j < depth_of_walls; j++) {	
		for (int i = 0; i < number_of_walls; i++) {
			auto wall = make_shared<SFAsset>(SFASSET_WALL, window);
			auto pos = Point2((canvas_w / number_of_walls) * i + wall->GetBoundingBox()->GetWidth() / 2, 200.0f+j*wall->GetBoundingBox()->GetHeight());
			wall->SetPosition(pos);
			walls.push_back(wall);
		}
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
		case SFEVENT_PLAYER_UP_PRESSED:
			playerNorth = true;
			break;
		case SFEVENT_PLAYER_DOWN_PRESSED:
			playerSouth = true;
			break;
		case SFEVENT_PLAYER_LEFT_PRESSED:
       		playerWest = true;
       		break;
		case SFEVENT_PLAYER_RIGHT_PRESSED:
       		playerEast = true;
       		break;
		case SFEVENT_PLAYER_UP_RELEASED:
			playerNorth = false;
			break;
		case SFEVENT_PLAYER_DOWN_RELEASED:
			playerSouth = false;
			break;
		case SFEVENT_PLAYER_LEFT_RELEASED:
       		playerWest = false;
       		break;
		case SFEVENT_PLAYER_RIGHT_RELEASED:
       		playerEast = false;
       		break;
		case SFEVENT_FIRE:
			if (player->IsAlive() && chargelvl >= maxCharge) {
				chargelvl = 0;
				FireProjectile();
				Mix_PlayChannel(-1, mothLaser, 0);
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
	
	//player movement
	//"if the player wants to move and isnt at the edge then.." 
	if (playerNorth && player->GetBoundingBox()->GetY() >= 0) { player->GoNorth(); }
	if (playerSouth && player->GetBoundingBox()->GetY() <= window->GetHeight() - player->GetBoundingBox()->GetHeight()) { player->GoSouth(); }
	if (playerWest && player->GetBoundingBox()->GetX() >= 0)  { player->GoWest(); }
	if (playerEast && player->GetBoundingBox()->GetX() <= window->GetWidth() - player->GetBoundingBox()->GetWidth())  { player->GoEast(); }	    
	
	//play music if no music
	if ( Mix_PlayingMusic() == 0 ) {
		Mix_PlayMusic( bgMusic, -1);
	}

	//update player charge
	chargelvl++;
	player->Charge(chargelvl, maxCharge);
	
	// 1. Move / update game objects
    for (auto p : projectiles) {
        if (p->IsAlive()) {
			p->GoNorth();
		}
    }
	for (auto wb : webs) {
		if (wb->IsAlive()) {
			wb->GoSouth();
		}
	}
	//builder 'AI'
	if (builder->IsAlive()) {
		switch (builderState) {
			case BUILDER_LEFT:
				builder->GoWest();
				if (builder->GetBoundingBox()->GetX() <= 0) {
					builderState = BUILDER_RIGHT;
				}
				break;
			case BUILDER_RIGHT:
				builder->GoEast();
				if (builder->GetBoundingBox()->GetX() >= window->GetWidth() - builder->GetBoundingBox()->GetWidth()) {
					builderState = BUILDER_LEFT;
				}
				break;
			case BUILDING:
				buildCharge++;			
				if (buildCharge >= 50) {
					RepairWall();
					buildCharge = 0;
				}
				if (player->GetBoundingBox()->GetX() >= builder->GetBoundingBox()->GetX()) {
					builderState = BUILDER_RIGHT;
				} else {
					builderState = BUILDER_LEFT;
				}
				break;
			case STUNNED:
				stunTimer++;
				if (stunTimer >= 120) {
					stunTimer = 0;
					if (player->GetBoundingBox()->GetX() >= builder->GetBoundingBox()->GetX()) {
						builderState = BUILDER_RIGHT;
					} else {
						builderState = BUILDER_LEFT;
					}
					break;
				}
		}					
	}
	auto builderPos = builder->GetPosition();	
	builderBody->SetPosition(builderPos);
	//queen 'AI'
	if (queen->IsAlive()) {
		auto playerPos = player->GetCenter();
		queenCharge++;	
		if (playerPos.getX() >= queen->GetCenter().getX()+10) {
			queen->GoEast();
		} else if (playerPos.getX() <= queen->GetCenter().getX()-10) {
			queen->GoWest();
		} else {
			if (queenCharge >= queenChargeMax) {
				SpawnSpider();
				queenCharge = 0;
			}
		}
	}

	if (numCoins >= 5) {
		numCoins = 0;
		IncreaseShotSpeed();
		coinUI->UpdateCoins(numCoins);
	}

    // coins
    for (auto c : coins) {
        c->GoSouth();
		if (c->GetBoundingBox()->GetY() >= window->GetHeight()) {
			c->SetNotAlive();
		}
    }
	
    // enemies
    for (auto a : spiders) {    
		a->GoSouth();
		if (a->GetBoundingBox()->GetY() >= window->GetHeight()) {
			a->SetNotAlive();
		}
    }

    // 2. Detect collisions
    for (auto p : projectiles) {
        for (auto a : spiders) {
            if (p->CollidesWith(a)) {
                p->HandleCollision();
                a->HandleCollision();
            }
		}
		for (auto w : walls) {
			if (p->CollidesWith(w)) {
				p->HandleCollision();
				w->DamageWall();
				if (w->IsDamaged() && RandomNumber(6) >= 2) {
					DropCoin(w->GetCenter());
				}
        	}
		}
		if (p->CollidesWith(queen)) {
			p->HandleCollision();			
			if (queenHealth >= 2) {
				queenHealth--;
				std::cout << "Queen health remaining: " << queenHealth << std::endl;
    		} else if (queenHealth == 1) {
				queenHealth--;
				std::cout << "Queen health remaining: " << queenHealth << std::endl;
				queen->SetNotAlive();
			}		
		}
		if (p->CollidesWith(builderBody)) {
			builderState = STUNNED;
			p->HandleCollision();
		}
	}
	
	if (player->CollidesWith(door)) {
		EndGame();
	}
	if (player->CollidesWith(queen) && queen->IsAlive()) {
		auto start_pos = Point2(window->GetWidth() / 2 - player->GetBoundingBox()->GetWidth() / 2, window->GetHeight() - player->GetBoundingBox()->GetHeight());		
		player->SetPosition(start_pos);
		numCoins = 0;
		coinUI->UpdateCoins(numCoins);
		}
	

	for (auto w : walls) {
		if (w->CollidesWith(player)) {
			player->HitWall( playerNorth, playerSouth, playerEast, playerWest);
		}
		if (w->CollidesWith(builder) && w->IsDamaged()) {
			auto pos = Point2(w->GetBoundingBox()->GetX(), builder->GetBoundingBox()->GetY());			
			builder->SetPosition(pos);			
			builderState = BUILDING;
		}
	}
	
	for (auto wb : webs) {
		for (auto w : walls) {
			if (wb->CollidesWith(w) && w->IsDamaged()) {
				w->RepairWall();
				wb->SetNotAlive();
				if (queenChargeMax >= 50) {
					queenChargeMax -= 5;
				}
			}
		}
		if (player->CollidesWith(wb)) {
			wb->SetNotAlive();			
			DecreaseShotSpeed();
		}
	}
	
	for (auto s : spiders) {
		if (s->CollidesWith(player)) {
			auto start_pos = Point2(window->GetWidth() / 2 - player->GetBoundingBox()->GetWidth() / 2, window->GetHeight() - player->GetBoundingBox()->GetHeight());		
			player->SetPosition(start_pos);			
			numCoins = 0;
			s->HandleCollision();
			coinUI->UpdateCoins(numCoins);
		}
	}

	for (auto c : coins) {
		if (c->CollidesWith(player)) {
			c->HandleCollision();
			numCoins++;
			coinUI->UpdateCoins(numCoins);
		}
	}	

    // 3. Remove dead spiders (the long way)
    list<shared_ptr<SFAsset>> tmpA;
    for (auto a : spiders) {
        if (a->IsAlive()) {
            tmpA.push_back(a);
        }
    }
    spiders.clear();
    spiders = list<shared_ptr<SFAsset>>(tmpA);
	
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

	//and webs
	list<shared_ptr<SFAsset>> tmpWb;
	for (auto wb : webs) {
		if (wb->IsAlive()) {
			tmpWb.push_back(wb);
		}
	}
	webs.clear();
	webs = list<shared_ptr<SFAsset>>(tmpWb);
}

void SFApp::OnRender() {
    // 1. Clear visible content
    window->ClearScreen();

    // 2. Draw game objects off-screen
    if (player->IsAlive()) 	{player->OnRender();}
	if (door->IsAlive()) 	{door->OnRender();}
	if (builder->IsAlive())	{builder->OnRender();}
	if (coinUI->IsAlive())	{coinUI->OnRender();}
	if (lvlUI->IsAlive())	{lvlUI->OnRender();}
	if (queen->IsAlive()) 	{queen->OnRender();}
	if (winGame->IsAlive()) {winGame->OnRender();}
	

    for (auto p : projectiles) {
        if (p->IsAlive()) { 
            p->OnRender(); 
        }
    }

    for (auto a : spiders) {
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
	
	for (auto wb : webs) {
		if (wb->IsAlive()) {
			wb->OnRender();
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
    coin->SetPosition(center);
    coins.push_back(coin);
}

int SFApp::RandomNumber( int scope ) {
	int number = rand() % (scope);
	return number;
}

void SFApp::IncreaseShotSpeed() {
	if (shotLvl <= 5) {
		shotLvl++;
		maxCharge *= 0.8;
		lvlUI->UpdateLevel( shotLvl );
	}
}

void SFApp::DecreaseShotSpeed() {
	if (shotLvl >= 1) {
		shotLvl--;
		maxCharge /= 0.8;
		lvlUI->UpdateLevel( shotLvl );
	}	
}

void SFApp::RepairWall() {
	auto pos = Point2(builder->GetBoundingBox()->GetX(), builder->GetBoundingBox()->GetY());	
	auto webbing = make_shared<SFAsset>(SFASSET_WEBBING, window);
	webbing->SetPosition(pos);
	webs.push_back(webbing);
}

void SFApp::SpawnSpider() {
	auto spider = make_shared<SFAsset>(SFASSET_SPIDER, window);
	auto v = queen->GetCenter();	
	auto pos = Point2(v.getX() - spider->GetBoundingBox()->GetWidth() / 2, v.getY() + (queen->GetBoundingBox()->GetHeight() / 2)); 
	spider->SetPosition(pos);
	spiders.push_back(spider);
}

void SFApp::EndGame() {
	player->SetNotAlive();
	door->SetNotAlive();
	queen->SetNotAlive();
	builder->SetNotAlive();
	coinUI->SetNotAlive();
	lvlUI->SetNotAlive();
	for ( auto p : projectiles) {
		p->SetNotAlive();
	}
	for ( auto w : walls) {
		w->SetNotAlive();
	}
	for ( auto s : spiders) {
		s->SetNotAlive();
	}
	for ( auto wb : webs) {
		wb->SetNotAlive();
	}
	for ( auto c : coins) {
		c->SetNotAlive();
	}
	winGame->SetWinAlive();
}
