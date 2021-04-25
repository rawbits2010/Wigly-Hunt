#include "animation.h"

#include <tonc.h>
#include <string.h>

static u32 first_free_tile_idx = 0;



void animationInit( Animation *obj, u32 speed, bool do_loop, bool do_pong ) {

	animationLoadTiles( obj );

	obj->equi_frames_to_change = speed;

	obj->do_loop = do_loop;
	obj->done = false;

	obj->do_pong = do_pong;
	if( do_pong ) {
		obj->pong_dir = 1;
	}

}

inline bool animationAdvance( Animation *obj ) {
	bool changed = false;

	if( obj->done ) {
		return false;
	}

	// advance time
	obj->curr_frame_count++;
	if( obj->curr_frame_count >= obj->equi_frames_to_change ) {
		obj->curr_frame_count = 0;
		changed = true;

		// advance frame
		if( obj->do_pong ) {

			obj->curr_frame_idx += obj->pong_dir;
			if( obj->pong_dir > 0 ) {
				if( obj->curr_frame_idx >= obj->frame_count-1 ) {

					obj->pong_dir *= -1;

				}
			} else {
				if( obj->curr_frame_idx <= 0 ) {

					obj->pong_dir *= -1;

					if( !obj->do_loop ) {
						obj->done = true;
					}

				}
			}

		} else {

			obj->curr_frame_idx++;
			if( obj->curr_frame_idx >= obj->frame_count-1 ) {

				obj->curr_frame_idx = 0;

				if( !obj->do_loop ) {
					obj->done = true;
				}

			}
		}

	}

	return changed;
}

inline void animationReset( Animation *obj ) {

	obj->curr_frame_count = 0;
	obj->curr_frame_idx = 0;
	
	obj->done = false;

	if( obj->do_pong ) {
		obj->pong_dir = 1;
	}

}


static u32 s_LoadTiles( const unsigned int *tiles, u32 tile_count, u32 tile_size ) {

	u32 bytesize = tile_count*tile_size;
	memcpy(&tile_mem[4][first_free_tile_idx], tiles, bytesize);

	u32 temp_idx = first_free_tile_idx;
	first_free_tile_idx += tile_count;

	return temp_idx;
}

// NOTE: probably not needed a separate
inline void animationLoadTiles( Animation *obj ) {
	obj->tilemem_start_idx = s_LoadTiles(obj->tiles, obj->frame_count * obj->tiles_per_frame, obj->tile_size);
}
