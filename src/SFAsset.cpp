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
    case SFASSET_ALIEN:
        sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
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

void SFAsset::GoWest() {
	Vector2 v = Vector2(0.0f, 0.0f);	
	if (SFASSET_PROJECTILE == type) {    
		v = Vector2(-SPEED, 0);		
	} else {    
		v = Vector2(-(SPEED/2), 0);
	}	
	bbox->Translate(v);
}

void SFAsset::GoEast() {
	Vector2 v = Vector2(0.0f, 0.0f);	
	if (SFASSET_PROJECTILE == type) {    
		v = Vector2(SPEED, 0);
	} else {
		v = Vector2((SPEED/2), 0);		
	}
    bbox->Translate(v);
}

void SFAsset::GoNorth() {
	Vector2 v = Vector2(0.0f, 0.0f);
	if (SFASSET_PROJECTILE == type) {    
		v = Vector2(0.0f, -SPEED);
	} else {
		v = Vector2(0.0f, -(SPEED/2));	    
	}
	bbox->Translate(v);
}

void SFAsset::GoSouth() {
	Vector2 v = Vector2(0.0f, 0.0f);	
	if (SFASSET_PROJECTILE == type) { 
		v = Vector2(0.0f, SPEED);
	} else {
		v = Vector2(0.0f, (SPEED/2));
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
    if (SFASSET_PROJECTILE == type || SFASSET_ALIEN == type) {
        SetNotAlive();
    }
	if (SFASSET_PROJECTILE == type || SFASSET_WALL == type) {		
		SetNotAlive();
	}
	if (SFASSET_PLAYER == type || SFASSET_WALL == type) {
		GoSouth();
	}
	if (SFASSET_PLAYER == type || SFASSET_COIN == type) {
		SetNotAlive();
	}
	if (SFASSET_WEBBING == type || SFASSET_DAM_WALL == type) {
		SetNotAlive();
	}
}

void SFAsset::HitWall( bool north, bool south, bool east, bool west) {
	if (north) { GoSouth(); }
	if (south) { GoNorth(); }
	if (east)  { GoWest(); }
	if (west)  { GoEast(); }
}
