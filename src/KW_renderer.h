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

/**
 * \file KW_renderer.h
 * \brief This file exposes the renderer API used by KiWi.
 * \details The functions in this file are the required ones to properly make
 *          KiWi render widgets - it relies on these functions being present and
 *          working as specified. If you ever wish to implement a different
 *          renderer (e.g, direct OpenGL calls instead of SDL), you should
 *          compile in a different renderer implementing these functions.
 */

#include "SDL.h"

/**
 * \brief   Opaque structure representing internal renderer state. */
typedef struct KW_Renderer * renderer;

/**
 * \brief   Creates a KW_Renderer to be used later to load and render textures.
 * \details This function is supposed to initialize a renderer instance. Be
 *          aware that different renderers might have different pre-conditions
 *          before working properly (e.g, SDL renderer might need to have
 *          SDL initialized and may need to receive
KW_Renderer * KW_CreateRenderer();

/**
 * \brief   Loads a texture from a file and returns it to later use.
 * \param   renderer The KW_Renderer that will load the texture.
 * \param   file The file to load.
 * \returns A KW_Texture instance for later use.
 */

KW_Texture * KW_LoadTexture(const char * file);
