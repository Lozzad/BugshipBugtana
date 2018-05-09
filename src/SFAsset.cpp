#include "SFAsset.h"

SFAsset::SFAsset(SFASSETTYPE type, std::shared_ptr<SFWindow> window) : type(type), sf_window(window) {
	switch (type) {	
	case SFASSET_PLAYER:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player0.png");
        break;
    case SFASSET_PROJECTILE:
        sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/projectile.png");
        break;
	case SFASSET_WEBBING:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/webbing.png");
		break;
    case SFASSET_SPIDER:
        sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/spider.png");
        break;
    case SFASSET_COIN:
        sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/coin.png");
        break;
	case SFASSET_WALL:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/wall.png");
		break;
	case SFASSET_DAM_WALL:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/damWall.png");
		break;
	case SFASSET_DOOR:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/door.png");
		break;
	case SFASSET_BUILDER:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/builder.png");
		break;
	case SFASSET_QUEEN:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/queen.png");
		break;
	case SFUI_COINS:
		LoadFromRenderedText ( "Coins: 0", {0,0,0});
		break;
	case SFUI_LVL: 
		LoadFromRenderedText ( "Shot Level: 1", {0,0,0});
		break;
	case SFASSET_WIN:
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/win.png");
		break;
    }

    if (!sprite) {
        throw SFException("Couldn't load asset");
    }

    // Get texture width & height
    int w, h;
    SDL_QueryTexture(sprite, NULL, NULL, &w, &h);

    // Initialise bounding box
    bbox = make_shared<SFBoundingBox>(Point2(0.0f, 0.0f), w, h);
}

SFAsset::SFAsset(const SFAsset& a) {
    sprite = a.sprite;
    sf_window = a.sf_window;
    bbox = a.bbox;
    type = a.type;
}

SFAsset::~SFAsset() {
    bbox.reset();
    if (sprite) {
        SDL_DestroyTexture(sprite);
        sprite = nullptr;
    }
}

void SFAsset::SetPosition(Point2 & point) {
    Vector2 v(point.getX(), point.getY());
    bbox->SetPosition(point);
}

Point2 SFAsset::GetPosition() {
    return Point2(bbox->GetX(), bbox->GetY());
}

Point2 SFAsset::GetCenter() {
    return Point2(bbox->GetX() + bbox->GetWidth() / 2, bbox->GetY() + bbox->GetHeight() / 2);
}

void SFAsset::OnRender() {
    // 1. Get the SDL_Rect from SFBoundingBox
    SDL_Rect rect = bbox->GetBox();

    // 2. Blit the sprite onto the level
    SDL_RenderCopy(sf_window->getRenderer(), sprite, NULL, &rect);
}

//used if statements to alter speeds for different objects, not very pretty but works
void SFAsset::GoWest() {
	Vector2 v = Vector2(0.0f, 0.0f);	
	if	(SFASSET_QUEEN == type) {
		v = Vector2(-(SPEED/4), 0);
	}
	else if (SFASSET_PROJECTILE == type) {    
		v = Vector2(-SPEED, 0);		
	} else if (SFASSET_SPIDER == type) {
		v = Vector2(-(SPEED/10), 0);
	}else {    
		v = Vector2(-(SPEED/2), 0);
	}	
	bbox->Translate(v);
}

void SFAsset::GoEast() {
	Vector2 v = Vector2(0.0f, 0.0f);	
	if	(SFASSET_QUEEN == type) {
		v = Vector2(SPEED/4, 0);
	} else if (SFASSET_PROJECTILE == type) {    
		v = Vector2(SPEED, 0);
	} else if (SFASSET_SPIDER == type) {
		v = Vector2(SPEED/10, 0);
	} else {
		v = Vector2((SPEED/2), 0);		
	}
    bbox->Translate(v);
}

void SFAsset::GoNorth() {
	Vector2 v = Vector2(0.0f, 0.0f);
	if	(SFASSET_QUEEN == type) {
		v = Vector2(0, -(SPEED/4));
	} else if (SFASSET_PROJECTILE == type) {    
		v = Vector2(0.0f, -SPEED);
	} else {
		v = Vector2(0.0f, -(SPEED/2));	    
	}
	bbox->Translate(v);
}

void SFAsset::GoSouth() {
	Vector2 v = Vector2(0.0f, 0.0f);	
	if	(SFASSET_QUEEN == type) {
		v = Vector2(0, SPEED/4);
	} else if (SFASSET_PROJECTILE == type) { 
		v = Vector2(0.0f, SPEED);
	} else {
		v = Vector2(0.0f, SPEED/2);
	}
	bbox->Translate(v);
}

bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
    return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
    return bbox;
}

void SFAsset::SetNotAlive() {    
	type = SFASSET_DEAD;
}

//used to reveal the win screen constantly lurking just out of sight
void SFAsset::SetWinAlive() {
	type = SFASSET_WIN;
}

bool SFAsset::IsAlive() {
    return (SFASSET_DEAD != type);
}

void SFAsset::DamageWall() {
	if (type == SFASSET_WALL) {
		type = SFASSET_DAM_WALL;
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/damWall.png");
	} else if (type == SFASSET_DAM_WALL) {
		type = SFASSET_DEAD;
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/wall.png");
	}
}

bool SFAsset::IsDamaged() {
	return (type == SFASSET_DAM_WALL);
}

void SFAsset::RepairWall() {
	type = SFASSET_WALL;
	sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/wall.png");
}

//Charge function updates the player sprite depending on the charge of their shot
void SFAsset::Charge(int Charge, int max) {
	if (Charge >= max) {
 		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player4.png");
	} else if (Charge < max && Charge >= max*0.75) {
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player3.png");
	} else if (Charge < max*0.75 && Charge >= max*0.5) {
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player2.png");
	} else if (Charge < max*0.5 && Charge >= max*0.25) {
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player1.png");
	} else if (Charge < max*0.25 && Charge >= 0) {
		sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player0.png");
	} else {
		throw SFException("Couldnt Get Sprite");
	}
}

void SFAsset::HandleCollision() {
    if (SFASSET_PROJECTILE == type || SFASSET_SPIDER == type) {
        SetNotAlive();
    }
	if (SFASSET_PROJECTILE == type || SFASSET_WALL == type) {		
		SetNotAlive();
	}
	if (SFASSET_PROJECTILE == type || SFASSET_BUILDER == type) {		
		SetNotAlive();
	}
	if (SFASSET_PLAYER == type || SFASSET_WALL == type) {
		GoSouth();
	}
	if (SFASSET_PLAYER == type || SFASSET_SPIDER == type) {
		SetNotAlive();
	}
	if (SFASSET_PLAYER == type || SFASSET_COIN == type) {
		SetNotAlive();
	}
	if (SFASSET_WEBBING == type || SFASSET_DAM_WALL == type) {
		SetNotAlive();
	}
}

//static barriers that work from any direction
void SFAsset::HitWall( bool north, bool south, bool east, bool west) {
	if (north) { GoSouth(); }
	if (south) { GoNorth(); }
	if (east)  { GoWest(); }
	if (west)  { GoEast(); }
}

//renders text as a font - created using lazyfoo productions' guide as a base 
void SFAsset::LoadFromRenderedText ( string textureText, SDL_Color textColor) {
	gFont = TTF_OpenFont("assets/theFont.ttf", 20);
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if (textSurface == NULL) {
		throw SFException( "Couldnt render text surface (SDL_ttf)" );
	} else {
		sprite = SDL_CreateTextureFromSurface( sf_window->getRenderer(), textSurface );
		if (sprite == NULL) { throw SFException( "Cant create texture from text" );} 
	}
}

//these 2 functions update the UI, could make this nicer but have run out of time
void SFAsset::UpdateCoins( int numCoins ) {
	sprite = NULL;	
	string text = "Coins: ";
	//stops the text from messing up
	text += to_string(numCoins);
	std::cout << text << std::endl;
	LoadFromRenderedText( text, {0,0,0});
}

void SFAsset::UpdateLevel( int shotLvl ) {
	sprite = NULL;	
	string text = "Shot Level: ";

	if (shotLvl == 5) { text += "MAX"; }
	else { text += to_string(shotLvl); }

	LoadFromRenderedText( text, {0,0,0});
}

