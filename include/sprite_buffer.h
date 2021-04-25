#ifndef SPRITE_BUFFER_H
#define SPRITE_BUFFER_H

#include <tonc.h>
#include "sprite.h"


extern void spritebufferInit();
extern void spritebufferAddSprite( Sprite *obj );
extern void spritebufferUploadAll();
extern void spritebufferUpload( u32 len );

#endif // SPRITE_BUFFER_H