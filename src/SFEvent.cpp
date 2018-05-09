#include "SFEvent.h"

/**
 * Effectively wraps an SDL_Event in our custom event type.
 */
SFEvent::SFEvent(const SDL_Event & event) {
    switch (event.type) {
    case SDL_QUIT:
        code = SFEVENT_QUIT;
        break;
    case SDL_USEREVENT:
        code = SFEVENT_UPDATE;
        break;
    case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
      		case SDLK_UP:
				code = SFEVENT_PLAYER_UP_PRESSED;
				break;
			case SDLK_DOWN:
				code = SFEVENT_PLAYER_DOWN_PRESSED;
				break;
			case SDLK_LEFT:
            	code = SFEVENT_PLAYER_LEFT_PRESSED;
            	break;
        	case SDLK_RIGHT:
            	code = SFEVENT_PLAYER_RIGHT_PRESSED;
            	break;
        	case SDLK_SPACE:
            	code = SFEVENT_FIRE;
            	break;
        	case SDLK_q:
            	code = SFEVENT_QUIT;
            	break;
			case SDLK_m:
				code = SFEVENT_MUTE;
				break;
		}
		break;
   case SDL_KEYUP:
		switch (event.key.keysym.sym) {
      		case SDLK_UP:
				code = SFEVENT_PLAYER_UP_RELEASED;
				break;
			case SDLK_DOWN:
				code = SFEVENT_PLAYER_DOWN_RELEASED;
				break;
			case SDLK_LEFT:
            	code = SFEVENT_PLAYER_LEFT_RELEASED;
            	break;
        	case SDLK_RIGHT:
            	code = SFEVENT_PLAYER_RIGHT_RELEASED;
            	break;
		}
	break;
	default:
		code = SFEVENT_NULL;
		break;
	}
}

SFEVENT SFEvent::GetCode() {
	return code;
}
