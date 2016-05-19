#ifndef RENDERDRIVER_H
#define RENDERDRIVER_H

/**
 * \file KW_renderdriver.h
 *
 * Declares the RenderDriver API to be implemented to create new Render Drivers
 **/

#include "KW_macros.h"

typedef void KW_Texture;
typedef void KW_Font;
typedef void KW_Surface;

typedef struct KW_Rect {
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
} KW_Rect;

#define KW_IsRectEmpty(r) \
  (!(((r.x) > 0) && ((r.y) > 0) && ((r.w) > 0) && ((r.h) > 0)))

/**
 * \brief Holds a color in the RGBA format
 **/
typedef struct KW_Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} KW_Color;

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
 * \brief   Declares the prototype for a RenderText function.
 * \details A RenderText function should be able to receive a font, a textline and a color and
 *          it should be able to produce a surface (pixeldata in CPU's memory) to be later transformed
 *          into a texture.
 * \param   driver the RenderDriver that will render this texture.
 * \param   font the font to use when rendering text.
 * \param   color the color that should be used.
 * \param   style the KW_RenderDriver_TextStyle style to apply.
 * \return  a KW_Surface to be later used as a texture.
 */
typedef KW_Surface * (*KW_RenderTextFunction)(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);

/**
 * \brief   Declares the prototype for a LoadFont function.
 * \details LoadFont should be able to load a fontFile with the specified point size.
 * \param   driver the RenderDriver that will render this texture.
 * \param   fontFile the file containing the font (usually .ttf)
 * \return   a KW_Font suitable to use with KW_RenderText
 */
typedef KW_Font * (*KW_LoadFontFunction)(KW_RenderDriver * driver, const char * fontFile, unsigned ptSize);

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
typedef void (*KW_GetClipRectFunction)(KW_RenderDriver * driver, KW_Rect * clip);

struct KW_RenderDriver {
  KW_RenderCopyFunction        renderCopy;
  KW_RenderTextFunction        renderText;
  KW_LoadFontFunction          loadFont;
  KW_CreateTextureFunction     createTexture;
  KW_CreateSurfaceFunction     createSurface;
  KW_LoadTextureFunction       loadTexture;
  KW_LoadSurfaceFunction       loadSurface;

  KW_GetSurfaceExtentsFunction getSurfaceExtents;
  KW_GetTextureExtentsFunction getTextureExtents;
  KW_BlitSurfaceFunction       blitSurface;

  KW_ReleaseTextureFunction    releaseTexture;
  KW_ReleaseSurfaceFunction    releaseSurface;
  KW_ReleaseFontFunction       releaseFont;

  KW_SetClipRectFunction       setClipRect;
  KW_GetClipRectFunction       getClipRect;

  void * priv;
};

extern DECLSPEC void KW_BlitSurface(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect);
extern DECLSPEC KW_Surface * KW_CreateSurface(KW_RenderDriver * driver, unsigned width, unsigned height);
extern DECLSPEC void KW_GetSurfaceExtents(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height);
extern DECLSPEC void KW_GetTextureExtents(KW_RenderDriver * driver, KW_Texture * texture, unsigned * width, unsigned * height);
extern DECLSPEC void KW_RenderCopy(KW_RenderDriver * driver, KW_Texture * src, const KW_Rect * clip, const KW_Rect * dstRect);
extern DECLSPEC KW_Surface * KW_RenderText(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
extern DECLSPEC KW_Font * KW_LoadFont(KW_RenderDriver * driver, const char * fontFile, unsigned ptSize);
extern DECLSPEC KW_Texture * KW_CreateTexture(KW_RenderDriver * driver, KW_Surface * surface);
extern DECLSPEC KW_Texture * KW_LoadTexture(KW_RenderDriver * driver, const char * file);
extern DECLSPEC KW_Surface * KW_LoadSurface(KW_RenderDriver * driver, const char * file);
extern DECLSPEC void KW_ReleaseTexture(KW_RenderDriver * driver, KW_Texture * texture);
extern DECLSPEC void KW_ReleaseSurface(KW_RenderDriver * driver, KW_Surface * surface);
extern DECLSPEC void KW_ReleaseFont(KW_RenderDriver * driver, KW_Font * font);
extern DECLSPEC void KW_GetClipRect(KW_RenderDriver * driver, KW_Rect * clip);
extern DECLSPEC void KW_SetClipRect(KW_RenderDriver * driver, const KW_Rect * clip, int force);

#endif
