#ifndef RENDERDRIVER_H
#define RENDERDRIVER_H

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
  (((r.x) > 0) && ((r.y) > 0) && ((r.w) > 0) && ((r.h) > 0))

typedef struct KW_Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} KW_Color;

typedef struct KW_RenderDriver KW_RenderDriver;

typedef enum KW_RenderDriver_TextStyle {
  KW_TTF_STYLE_NORMAL        = 0x00,
  KW_TTF_STYLE_BOLD          = 0x01,
  KW_TTF_STYLE_ITALIC        = 0x02,
  KW_TTF_STYLE_UNDERLINE     = 0x04,
  KW_TTF_STYLE_STRIKETHROUGH = 0x08
} KW_RenderDriver_TextStyle;


typedef void (*KW_RenderCopyFunction)(KW_RenderDriver * driver, KW_Texture * src, const KW_Rect * clip, const KW_Rect * dstRect);
typedef KW_Surface * (*KW_RenderTextFunction)(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
typedef KW_Font * (*KW_LoadFontFunction)(KW_RenderDriver * driver, const char * fontFile, unsigned ptSize);
typedef KW_Texture * (*KW_CreateTextureFunction)(KW_RenderDriver * driver, KW_Surface * Surface);
typedef KW_Texture * (*KW_LoadTextureFunction)(KW_RenderDriver * driver, const char * file);
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
