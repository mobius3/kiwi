#ifndef RENDERDRIVER_H
#define RENDERDRIVER_H

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

#include "KW_macros.h"
#include "KW_rect.h"
#include "KW_bool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KW_Texture {
  void * texture;
} KW_Texture;

typedef struct KW_Font {
  void * font;
} KW_Font;

typedef struct KW_Surface {
  void * surface;
} KW_Surface;

/**
 * \brief Holds a color in the RGBA format
 **/
typedef struct KW_Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} KW_Color;

/**
 * \brief Multiply a color by a percent amount;
 * @param color The color to multiply
 * @param amount The amount to multiply
 * @return multiplied color
 */
extern DECLSPEC KW_Color KW_MultiplyColor(KW_Color color, float amount);

typedef struct KW_RenderDriver KW_RenderDriver;

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
typedef void (*KW_RenderCopyFunction)(KW_RenderDriver * driver, KW_Texture * src, const KW_Rect * clip, const KW_Rect * dstRect);

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
typedef void (*KW_UTF8TextSizeFunction)(KW_RenderDriver * driver, KW_Font * font, const char * text, unsigned * width, unsigned * height);

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
typedef KW_Texture * (*KW_RenderTextFunction)(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);

/**
 * \brief   Declares the prototype for a LoadFont function.
 * \details LoadFont should be able to load a fontFile with the specified point size.
 * \param   driver the RenderDriver that will render this texture.
 * \param   fontFile the file containing the font (usually .ttf)
 * \return   a KW_Font suitable to use with KW_RenderText
 */
typedef KW_Font * (*KW_LoadFontFunction)(KW_RenderDriver * driver, const char * fontFile, unsigned ptSize);

/**
 * \brief   Declares the prototype for a LoadFontFromMemory function.
 * \details LoadFont should be able to load a fontFile with the specified point size.
 * \param   driver the RenderDriver that will render this texture.
 * \param   fontMemory the memory buffer containing the font.
 * \param   memSize the size of the buffer
 * \return   a KW_Font suitable to use with KW_RenderText
 */
typedef KW_Font * (*KW_LoadFontFromMemoryFunction)(KW_RenderDriver * driver, const void * fontMemory, unsigned long memSize, unsigned ptSize);

/**
 * \brief   Declares the prototype for a CreateTexture function.
 * \details CreateTexture should be able to create a KW_Texture from a KW_Surface.
 * \param   driver the RenderDriver that will render this texture.
 * \param   src the source KW_Surface.
 * \return  a KW_Texture in suitable to use with KW_RenderCopy
 */
typedef KW_Texture * (*KW_CreateTextureFunction)(KW_RenderDriver * driver, KW_Surface * src);


/**
 * \brief   Declares the prototype for a LoadTexture function.
 * \details LoadTexture should be able to create a KW_Texture from a file.
 * \param   driver the RenderDriver that will load this texture.
 * \param   file the file name with the pixeldata.
 * \return  a KW_Texture suitable to use with KW_RenderCopy
 */
typedef KW_Texture * (*KW_LoadTextureFunction)(KW_RenderDriver * driver, const char * file);

/**
 * \brief   Declares the prototype for a LoadSurface function.
 * \details LoadSurface should be able to create a KW_Surface from a file.
 * \param   driver the RenderDriver that will load this surface.
 * \param   file the file name with the pixeldata.
 * \return  a KW_Surface.
 */
typedef KW_Surface * (*KW_LoadSurfaceFunction)(KW_RenderDriver * driver, const char * file);
typedef void (*KW_ReleaseTextureFunction)(KW_RenderDriver * driver, KW_Texture * texture);
typedef void (*KW_ReleaseSurfaceFunction)(KW_RenderDriver * driver, KW_Surface * surface);
typedef void (*KW_ReleaseFontFunction)(KW_RenderDriver * driver, KW_Font * font);
typedef KW_Surface * (*KW_CreateSurfaceFunction)(KW_RenderDriver * driver, unsigned width, unsigned height);
typedef void (*KW_GetSurfaceExtentsFunction)(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height);
typedef void (*KW_GetTextureExtentsFunction)(KW_RenderDriver * driver, KW_Texture * texture, unsigned * width, unsigned * height);
typedef void (*KW_BlitSurfaceFunction)(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect);
typedef void (*KW_SetClipRectFunction)(KW_RenderDriver * driver, const KW_Rect * clip, int force);
typedef KW_bool (*KW_GetClipRectFunction)(KW_RenderDriver * driver, KW_Rect * clip);
typedef unsigned int (*KW_GetPixelFunction)(KW_RenderDriver * driver, KW_Surface * surface, unsigned px, unsigned py);
typedef void (*KW_RenderRectFunction)(KW_RenderDriver * driver, KW_Rect * rect, KW_Color color);
typedef void (*KW_GetViewportSizeFunction)(KW_RenderDriver * driver, KW_Rect * rect);

/**
 * \brief   Declares the prototype for a ReleaseDriver function.
 * \details ReleaseDriver should be able to release a render driver private data
 *                        and the KW_RenderDriver struct itself.
 * \param   driver the RenderDriver that will be released.
 */
typedef void (*KW_ReleaseDriverFunction)(KW_RenderDriver * driver);

struct KW_RenderDriver {
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
};

extern DECLSPEC void KW_RenderRect(KW_RenderDriver * driver, KW_Rect * rect, KW_Color color);
extern DECLSPEC void KW_BlitSurface(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect);
extern DECLSPEC KW_Surface * KW_CreateSurface(KW_RenderDriver * driver, unsigned width, unsigned height);
extern DECLSPEC void KW_GetSurfaceExtents(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height);
extern DECLSPEC void KW_GetTextureExtents(KW_RenderDriver * driver, KW_Texture * texture, unsigned * width, unsigned * height);
extern DECLSPEC void KW_RenderCopy(KW_RenderDriver * driver, KW_Texture * src, const KW_Rect * clip, const KW_Rect * dstRect);
extern DECLSPEC KW_Texture * KW_RenderText(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
extern DECLSPEC KW_Font * KW_LoadFont(KW_RenderDriver * driver, const char * fontFile, unsigned ptSize);
extern DECLSPEC KW_Font * KW_LoadFontFromMemory(KW_RenderDriver * driver, const void * fontMemory, unsigned long memSize, unsigned ptSize);
extern DECLSPEC KW_Texture * KW_CreateTexture(KW_RenderDriver * driver, KW_Surface * surface);
extern DECLSPEC KW_Texture * KW_LoadTexture(KW_RenderDriver * driver, const char * file);
extern DECLSPEC KW_Surface * KW_LoadSurface(KW_RenderDriver * driver, const char * file);
extern DECLSPEC void KW_ReleaseTexture(KW_RenderDriver * driver, KW_Texture * texture);
extern DECLSPEC void KW_ReleaseSurface(KW_RenderDriver * driver, KW_Surface * surface);
extern DECLSPEC void KW_ReleaseFont(KW_RenderDriver * driver, KW_Font * font);
extern DECLSPEC KW_bool KW_GetClipRect(KW_RenderDriver * driver, KW_Rect * clip);
extern DECLSPEC void KW_GetViewportSize(KW_RenderDriver * driver, KW_Rect * rect);
extern DECLSPEC void KW_SetClipRect(KW_RenderDriver * driver, const KW_Rect * clip, int force);
extern DECLSPEC void KW_ReleaseRenderDriver(KW_RenderDriver * driver);
extern DECLSPEC void KW_UTF8TextSize(KW_RenderDriver * driver, KW_Font * font, const char * text, unsigned * width, unsigned * height);
extern DECLSPEC unsigned int KW_GetPixel(KW_RenderDriver * driver, KW_Surface * surface, unsigned x, unsigned y);

#ifdef __cplusplus
}
#endif

#endif
