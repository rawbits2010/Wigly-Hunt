#include "end_screen.h"

#include <string.h>

#include "dead.h"



void endscreenInit() {
	//
	// BG

	// Load palette
	// Load tiles into CBB 0
	//memcpy(&tile_mem[0][0], deadTiles, deadTilesLen);
	// Load map into SBB 30
	memcpy(&se_mem[28][0], deadMap, deadMapLen);

}

void endscreenReset( bool win ) {
	REG_BG0VOFS = 0;
	if( !win ) {
		// Load tiles into CBB 0
		memcpy(pal_bg_mem, deadPal, 16*2);
		memcpy(&tile_mem[0][0], deadTiles, deadTilesLen);

	}
}

bool endscreenHandleInput() {
	key_poll();

	if( key_hit(KEY_A) || key_hit(KEY_START) ) {
		return true;
	}

	return false;
}