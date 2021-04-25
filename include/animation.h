#ifndef ANIMATION_H
#define ANIMATION_H

typedef struct Animation {

	// comes from the gfx conversion
	const unsigned int *tiles;	// pointer to tile_array
	u32 tile_size;				// tile size in bytes - 4bpp -> 32, 8bpp -> 64
	u32 tiles_per_frame;		// tile per animation frame
	u32 frame_count;			// how many animation frames the tile array cover
	const short unsigned int *palette;
	u32 palette_startidx;
	u32 palette_count;
	
	// fill this after loading the data - now it's filled internally
	u32 tilemem_start_idx;
	u32 palmem_start_idx;

} ALIGN4 Animation;



#endif // ANIMATION_H