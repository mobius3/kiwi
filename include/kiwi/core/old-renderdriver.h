#ifndef KIWI_CORE_OLD_RENDERDRIVER_H
#define KIWI_CORE_OLD_RENDERDRIVER_H

/**
 * \file KW_renderdriver.h
 *
 * Declares the RenderDriver API to be implemented to create new Render Drivers.
 * A Render Driver wraps and abstract calls to a specific rendering backend. For instance,
 * if you want to port KiWi to your own renderer, you would implement all of the functions
 * listed below.
 *
 * You are responsible for creating a valid KW_RenderDriver struct, and you should make
 * available a function for the user to do so: this provide means to create complex
 * initialization routines, enabling you to decouple KiWi initialization from your game
 * rendering initialization. For example, the SDL2 RenderDriver exposes a single function,
 * KW_CreateSDL2RenderDriver(), that receives the renderer and window instance to work with.
 **/

#include "kiwi/core/core-export.h"
#include "rect.h"
#include "bool.h"
#include "color.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KW_OldTexture {
  void * texture;
} KW_OldTexture;

typedef struct KW_OldFont {
  void * font;
} KW_OldFont;

typedef struct KW_Surface {
  void * surface;
} KW_Surface;

/**
 * \brief Multiply a color by a percent amount;
 * @param color The color to multiply
 * @param amount The amount to multiply
 * @return multiplied color
 */
extern KIWI_CORE_EXPORT KW_Color KW_MultiplyColor(KW_Color color, float amount);

typedef struct KW_OldRenderDriver KW_OldRenderDriver;

/**
 * \brief Defines how text should be rendered
 **/
typedef enum KW_RenderDriver_TextStyle {
  KW_TTF_STYLE_NORMAL        = 0x00,
  KW_TTF_STYLE_BOLD          = 0x01,
  KW_TTF_STYLE_ITALIC        = 0x02,
  KW_TTF_STYLE_UNDERLINE     = 0x04,
  KW_TTF_STYLE_STRIKETHROUGH = 0x08
} KW_RenderDriver_TextStyle;


/**
 * \brief   Declares the prototype for a RenderCopy function
 * \details A RenderCopy function deals with Textures that are possibly in GPU's RAM.
 *          It should be able to take a src texture and render it, applying clipping with clipRect
 *          and scaling with dstRect.
 * \param   driver the RenderDriver that will render this texture.
 * \param   src the source texture.
 * \param   clip the clipping rectangle for the source texture (in pixels)
 * \param   dstRect the destination rect. If different that clipping rectangle, it should scale to fit.
 */
typedef void (*KW_RenderCopyFunction)(KW_OldRenderDriver * driver, KW_OldTexture * src, const KW_Rect * clip, const KW_Rect * dstRect);

/**
 * \brief   Declares the prototype for a UTF8TextSize function
 * \details A UTF8TextSize funciton returns wthe width and height of that text if rendered
 *          using the passed in font.
 * \param   driver the RenderDriver that will render this texture.
 * \param   font the font
 * \param   text the text to calculate the size
 * \param   width The returned width
 * \param   height The returned height
 */
typedef void (*KW_UTF8TextSizeFunction)(KW_OldRenderDriver * driver, KW_OldFont * font, const char * text, unsigned * width, unsigned * height);

/**
 * \brief   Declares the prototype for a RenderText function.
 * \details A RenderText function should be able to receive a font, a textline and a color and
 *          it should be able to produce a surface (pixeldata in CPU's memory) to be later transformed
 *          into a texture.
 * \param   driver the RenderDriver that will render this texture.
 * \param   font the font to use when rendering text.
 * \param   color the color that should be used.
 * \param   style the KW_RenderDriver_TextStyle style to apply.
 * \return  a KW_Texture with the rendered text
 */
typedef KW_OldTexture * (*KW_RenderTextFunction)(KW_OldRenderDriver * driver, KW_OldFont * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);

/**
 * \brief   Declares the prototype for a RenderTextWrapped function.
 * \details A RenderTextWrapped function should be able to receive a font, a textline and a color and
 *          it should be able to produce a surface (pixeldata in CPU's memory) to be later transformed
 *          into a texture.
 * \param   driver the RenderDriver that will render this texture.
 * \param   font the font to use when rendering text.
 * \param   color the color that should be used.
 * \param   style the KW_RenderDriver_TextStyle style to apply.
 * \param   wrapwidth the width at which to wrap the text, or 0 if not wrapping should occur
 * \return  a KW_Texture with the rendered text
 */
typedef KW_OldTexture * (*KW_RenderTextWrappedFunction)(KW_OldRenderDriver * driver, KW_OldFont * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style, int wrapwidth);

/**
 * \brief   Declares the prototype for a LoadFont function.
 * \details LoadFont should be able to load a fontFile with the specified point size.
 * \param   driver the RenderDriver that will render this texture.
 * \param   fontFile the file containing the font (usually .ttf)
 * \return   a KW_OldFont suitable to use with KW_RenderText
 */
typedef KW_OldFont * (*KW_LoadFontFunction)(KW_OldRenderDriver * driver, const char * fontFile, unsigned ptSize);

/**
 * \brief   Declares the prototype for a LoadFontFromMemory function.
 * \details LoadFont should be able to load a fontFile with the specified point size.
 * \param   driver the RenderDriver that will render this texture.
 * \param   fontMemory the memory buffer containing the font.
 * \param   memSize the size of the buffer
 * \return   a KW_OldFont suitable to use with KW_RenderText
 */
typedef KW_OldFont * (*KW_LoadFontFromMemoryFunction)(KW_OldRenderDriver * driver, const void * fontMemory, unsigned long memSize, unsigned ptSize);

/**
 * \brief   Declares the prototype for a CreateTexture function.
 * \details CreateTexture should be able to create a KW_Texture from a KW_Surface.
 * \param   driver the RenderDriver that will render this texture.
 * \param   src the source KW_Surface.
 * \return  a KW_Texture in suitable to use with KW_RenderCopy
 */
typedef KW_OldTexture * (*KW_CreateTextureFunction)(KW_OldRenderDriver * driver, KW_Surface * src);


/**
 * \brief   Declares the prototype for a LoadTexture function.
 * \details LoadTexture should be able to create a KW_Texture from a file.
 * \param   driver the RenderDriver that will load this texture.
 * \param   file the file name with the pixeldata.
 * \return  a KW_Texture suitable to use with KW_RenderCopy
 */
typedef KW_OldTexture * (*KW_LoadTextureFunction)(KW_OldRenderDriver * driver, const char * file);

/**
 * \brief   Declares the prototype for a LoadSurface function.
 * \details LoadSurface should be able to create a KW_Surface from a file.
 * \param   driver the RenderDriver that will load this surface.
 * \param   file the file name with the pixeldata.
 * \return  a KW_Surface.
 */
typedef KW_Surface * (*KW_LoadSurfaceFunction)(KW_OldRenderDriver * driver, const char * file);
typedef void (*KW_ReleaseTextureFunction)(KW_OldRenderDriver * driver, KW_OldTexture * texture);
typedef void (*KW_ReleaseSurfaceFunction)(KW_OldRenderDriver * driver, KW_Surface * surface);
typedef void (*KW_ReleaseFontFunction)(KW_OldRenderDriver * driver, KW_OldFont * font);
typedef KW_Surface * (*KW_CreateSurfaceFunction)(KW_OldRenderDriver * driver, unsigned width, unsigned height);
typedef void (*KW_GetSurfaceExtentsFunction)(KW_OldRenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height);
typedef void (*KW_GetTextureExtentsFunction)(KW_OldRenderDriver * driver, KW_OldTexture * texture, unsigned * width, unsigned * height);
typedef void (*KW_BlitSurfaceFunction)(KW_OldRenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect);
typedef void (*KW_SetClipRectFunction)(KW_OldRenderDriver * driver, const KW_Rect * clip, int force);
typedef KW_bool (*KW_GetClipRectFunction)(KW_OldRenderDriver * driver, KW_Rect * clip);
typedef unsigned int (*KW_GetPixelFunction)(KW_OldRenderDriver * driver, KW_Surface * surface, unsigned px, unsigned py);
typedef void (*KW_RenderRectFunction)(KW_OldRenderDriver * driver, KW_Rect * rect, KW_Color color);
typedef void (*KW_GetViewportSizeFunction)(KW_OldRenderDriver * driver, KW_Rect * rect);

/**
 * \brief   Declares the prototype for a ReleaseDriver function.
 * \details ReleaseDriver should be able to release a render driver private data
 *                        and the KW_RenderDriver struct itself.
 * \param   driver the RenderDriver that will be released.
 */
typedef void (*KW_ReleaseDriverFunction)(KW_OldRenderDriver * driver);

struct KW_OldRenderDriver {
  KW_RenderCopyFunction        renderCopy;
  KW_RenderTextFunction        renderText;
  KW_RenderRectFunction        renderRect;
  KW_UTF8TextSizeFunction      utf8TextSize;
  KW_LoadFontFunction          loadFont;
  KW_LoadFontFromMemoryFunction loadFontFromMemory;
  KW_CreateTextureFunction     createTexture;
  KW_CreateSurfaceFunction     createSurface;
  KW_LoadTextureFunction       loadTexture;
  KW_LoadSurfaceFunction       loadSurface;

  KW_GetSurfaceExtentsFunction getSurfaceExtents;
  KW_GetTextureExtentsFunction getTextureExtents;
  KW_BlitSurfaceFunction       blitSurface;

  KW_GetViewportSizeFunction           getViewportSize;

  KW_ReleaseTextureFunction    releaseTexture;
  KW_ReleaseSurfaceFunction    releaseSurface;
  KW_ReleaseFontFunction       releaseFont;

  KW_SetClipRectFunction       setClipRect;
  KW_GetClipRectFunction       getClipRect;

  KW_GetPixelFunction          getPixel;

  KW_ReleaseDriverFunction     release;

  void * priv;
  KW_RenderTextWrappedFunction renderTextWrapped;
};

extern KIWI_CORE_EXPORT void KW_RenderRect(KW_OldRenderDriver * driver, KW_Rect * rect, KW_Color color);
extern KIWI_CORE_EXPORT void KW_BlitSurface(KW_OldRenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect);
extern KIWI_CORE_EXPORT KW_Surface * KW_CreateSurface(KW_OldRenderDriver * driver, unsigned width, unsigned height);
extern KIWI_CORE_EXPORT void KW_GetSurfaceExtents(KW_OldRenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height);
extern KIWI_CORE_EXPORT void KW_GetTextureExtents(KW_OldRenderDriver * driver, KW_OldTexture * texture, unsigned * width, unsigned * height);
extern KIWI_CORE_EXPORT void KW_RenderCopy(KW_OldRenderDriver * driver, KW_OldTexture * src, const KW_Rect * clip, const KW_Rect * dstRect);
extern KIWI_CORE_EXPORT KW_OldTexture * KW_RenderText(KW_OldRenderDriver * driver, KW_OldFont * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
extern KIWI_CORE_EXPORT KW_OldTexture * KW_RenderTextWrapped(KW_OldRenderDriver * driver, KW_OldFont * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style, int wrapwidth);
extern KIWI_CORE_EXPORT KW_OldFont * KW_LoadFont(KW_OldRenderDriver * driver, const char * fontFile, unsigned ptSize);
extern KIWI_CORE_EXPORT KW_OldFont * KW_LoadFontFromMemory(KW_OldRenderDriver * driver, const void * fontMemory, unsigned long memSize, unsigned ptSize);
extern KIWI_CORE_EXPORT KW_OldTexture * KW_CreateTexture(KW_OldRenderDriver * driver, KW_Surface * surface);
extern KIWI_CORE_EXPORT KW_OldTexture * KW_LoadTexture(KW_OldRenderDriver * driver, const char * file);
extern KIWI_CORE_EXPORT KW_Surface * KW_LoadSurface(KW_OldRenderDriver * driver, const char * file);
extern KIWI_CORE_EXPORT void KW_ReleaseTexture(KW_OldRenderDriver * driver, KW_OldTexture * texture);
extern KIWI_CORE_EXPORT void KW_ReleaseSurface(KW_OldRenderDriver * driver, KW_Surface * surface);
extern KIWI_CORE_EXPORT void KW_ReleaseFont(KW_OldRenderDriver * driver, KW_OldFont * font);
extern KIWI_CORE_EXPORT KW_bool KW_GetClipRect(KW_OldRenderDriver * driver, KW_Rect * clip);
extern KIWI_CORE_EXPORT void KW_GetViewportSize(KW_OldRenderDriver * driver, KW_Rect * rect);
extern KIWI_CORE_EXPORT void KW_SetClipRect(KW_OldRenderDriver * driver, const KW_Rect * clip, int force);
extern KIWI_CORE_EXPORT void KW_ReleaseRenderDriver(KW_OldRenderDriver * driver);
extern KIWI_CORE_EXPORT void KW_UTF8TextSize(KW_OldRenderDriver * driver, KW_OldFont * font, const char * text, unsigned * width, unsigned * height);
extern KIWI_CORE_EXPORT unsigned int KW_GetPixel(KW_OldRenderDriver * driver, KW_Surface * surface, unsigned x, unsigned y);

#ifdef __cplusplus
}
#endif

#endif
