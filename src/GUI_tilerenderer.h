/*
  Copyright (c) 2014, Leonardo Guilherme de Freitas
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef GUI_TILERENDERER_H
#define GUI_TILERENDERER_H

/**
 * \file GUI_tilerenderer.h
 * 
 * Declare functions for rendering tiles from a tile set.
 * The macro TILESIZE defines the size of the tile (w/h).
 */

#include "SDL.h"
#include "GUI_macros.h"
#define TILESIZE 8

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   Render a single tile from a tile set.
 * \param   renderer The SDL_Renderer that will render this tile.
 * \param   tileset The tileset texture to get this tile from.
 * \param   column The column in the tile set (starts at 0).
 * \param   line The line in the tile set (starts at 0).
 * \param   x The x coordinate in the screen to render the tile.
 * \param   y The y coordinate in the screen to render the tile.
 */
void DECLSPEC GUI_RenderTile(SDL_Renderer * renderer, SDL_Texture * tileset, int column, int line, int x, int y);

/**
 * \brief   Render a tile filling the whole w/h specified.
 * \details The tile rendering will start at x/y and will fill until x+w/y+h, clipping when necessary.
 * \param   renderer The SDL_Renderer that will render this tile.
 * \param   tileset The tileset texture to get this tile from.
 * \param   column The column in the tile set (starts at 0).
 * \param   line The line in the tile set (starts at 0).
 * \param   x The x coordinate in the screen to render the tile.
 * \param   y The y coordinate in the screen to render the tile.
 * \param   w The width to fill.
 * \param   h The height to fill.
 */
void DECLSPEC GUI_RenderTileFill(SDL_Renderer * renderer, SDL_Texture * tileset, int column, int line, int x, int y, int w, int h);

#ifdef __cplusplus
}
#endif

#endif