#ifndef ANIMATION_H
#define ANIMATION_H

#include <tonc.h>


typedef struct Animation {

	// comes from the gfx conversion
	const unsigned int *tiles;	// pointer to tile_array
	u32 tile_size;				// tile size in bytes - 4bpp -> 32, 8bpp -> 64
	u32 tiles_per_frame;		// tile per animation frame
	u32 frame_count;			// how many animation frames the tile array cover
	
	// fill this after loading the data - now it's filled internally
	u32 tilemem_start_idx;

	// will be set internally by sprite animation functions
	u32 curr_frame_idx;			// current animation frame - zero based
	u32 equi_frames_to_change;	// change when reached
	u32 curr_frame_count;		// this is where we are at in time

	// TODO: these would be better as flags

	// should it loop? - one shot anims will reset to default
	bool do_loop;
	bool done;	// internal to sign if the animation stopped

	// by default, animation is looping
	bool do_pong;	// this can override and make it go ping-pong
	s32 pong_dir;	// handled internally (+1 or -1) (2 bits)

	// positioning this animation from the base anim
	s32 h_offset;

} Animation;

extern void animationInit( Animation *obj, u32 speed, bool do_loop, bool do_pong );
extern void animationLoadTiles( Animation *obj );
extern bool animationAdvance( Animation *obj );
extern void animationReset( Animation *obj );

#endif // ANIMATION_H