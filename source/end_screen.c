#include "end_screen.h"

#include <string.h>

#include "dead.h"
#include "win.h"



void endscreenInit() {

}

void endscreenReset( bool win ) {
	REG_BG0VOFS = 0;
	if( !win ) {
		memcpy(&se_mem[28][0], deadMap, deadMapLen);
		memcpy(pal_bg_mem, deadPal, 16*2);
		memcpy(&tile_mem[0][0], deadTiles, deadTilesLen);

	} else {
		memcpy(&se_mem[28][0], winMap, winMapLen);
		memcpy(pal_bg_mem, winPal, 16*2);
		memcpy(&tile_mem[0][0], winTiles, winTilesLen);

	}
}

bool endscreenHandleInput() {
	key_poll();

	if( key_hit(KEY_A) || key_hit(KEY_START) ) {
		return true;
	}

	return false;
}