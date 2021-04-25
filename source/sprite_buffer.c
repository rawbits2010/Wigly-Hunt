#include "sprite_buffer.h"


static OBJ_ATTR obj_buffer[128];
static OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;
static u32 first_free_sprite_idx = 0;


inline void spritebufferInit() {
	oam_init(obj_buffer, 128);
}

inline void spritebufferAddSprite( Sprite *obj ) {
	obj->obj_attr = &obj_buffer[first_free_sprite_idx++];
	spriteSetAnimationFrame(obj); // ugly but does just what we need
}

inline void spritebufferUpload( u32 len ) {
	oam_copy(oam_mem, obj_buffer, len );
}

inline void spritebufferUploadAll() {
	spritebufferUpload(128);
}