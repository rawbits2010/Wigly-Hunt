#ifndef SPRITE_BUFFER_H
#define SPRITE_BUFFER_H

#include <tonc.h>
#include "sprite.h"
#include "double_sprite.h"


// Note: these are hidden by default and needs to be filled from some other Sprite
extern void spritebufferCreateEmpty( Sprite *obj );

// TODO: this is cool but need to think about it a little
extern void spritebufferCloneSprite( Sprite *src, Sprite *dst );

extern void spritebufferInit();
extern void spritebufferAddSprite( Sprite *obj );
extern void spritebufferUploadAll();
extern void spritebufferUpload( u32 len );

#endif // SPRITE_BUFFER_H