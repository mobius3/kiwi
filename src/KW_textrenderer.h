/*
  Copyright (c) 2014, Leonardo Guilherme de Freitas
  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

     1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

     2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

     3. This notice may not be removed or altered from any source
     distribution.
*/

#ifndef KW_TEXTRENDERER_H
#define KW_TEXTREDNERER_H
#include "SDL_ttf.h"
#include "KW_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file KW_textrenderer.h
 * 
 * Helper functions for text rendering with SDL_ttf
 */

/**
 * \brief   Renders a single line of text.
 * \details Renders a new texture from text.
 * \param   font The TTF_Font to use in text rendering.
 * \param   renderer The renderer that will render the returned texture.
 * \param   text The text to render.
 * \param   color The color to render this text with.
 * \param   styleflags A list of TTF_STYLE_* style flags to pass.
 * \return  The resulting texture.
 */
SDL_Texture DECLSPEC * KW_RenderTextLine(TTF_Font * font, SDL_Renderer * renderer, const char * text, SDL_Color color, int styleflags);

#ifdef __cplusplus
}
#endif

#endif
