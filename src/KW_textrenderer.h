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
