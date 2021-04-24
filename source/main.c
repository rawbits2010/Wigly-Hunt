#include <tonc.h>

#include "worm.h"


OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;


int main() {

	// Places the glyphs of a 4bpp boxed metroid sprite 
	//   into LOW obj memory (cbb == 4)
	memcpy(&tile_mem[4][0], wormTiles, wormTilesLen);
	memcpy(pal_obj_mem, wormPal, wormPalLen);

	oam_init(obj_buffer, 128);
	REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;

	// enable isr switchboard and VBlank interrupt
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	int x= 96, y= 32;
	u32 tid= 0, pb= 0;		// tile id, pal-bank

	OBJ_ATTR *worm= &obj_buffer[0];
	obj_set_attr(worm, 
		ATTR0_SQUARE,				// Square, regular sprite
		ATTR1_SIZE_16,					// 64x64p, 
		ATTR2_PALBANK(pb) | tid);		// palbank 0, tile 0

	// position sprite (redundant here; the _real_ position
	// is set further down
	obj_set_pos(worm, x, y);


	while(1) {

		VBlankIntrWait();

		oam_copy(oam_mem, obj_buffer, 1);

	}

	return 0;
}