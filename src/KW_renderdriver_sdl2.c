#include "KW_renderdriver_sdl2.h"
#include "KW_renderdriver.h"
#include "KW_widget.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include <stdio.h>

typedef struct KWSDL {
    SDL_Renderer * renderer;
    SDL_Window * window;
} KWSDL;

static void KWSDL_renderCopy(KW_RenderDriver * driver, KW_Texture * texture, const KW_Rect * src, const KW_Rect * dst);
static KW_Texture * KWSDL_renderText(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
static KW_Font * KWSDL_loadFont(KW_RenderDriver * driver, const char * font, unsigned ptSize);
static KW_Font * KWSDL_loadFontFromMemory(KW_RenderDriver * driver, const void * fontMemory, unsigned long memSize, unsigned ptSize);
static KW_Texture * KWSDL_createTexture(KW_RenderDriver * driver, KW_Surface * surface);
static KW_Surface * KWSDL_createRGBA32Surface(KW_RenderDriver * driver, unsigned width, unsigned height);
static KW_Texture * KWSDL_loadTexture(KW_RenderDriver * driver, const char * texturefile);
static KW_Surface * KWSDL_loadSurface(KW_RenderDriver * driver, const char * texturefile);
static void KWSDL_getSurfaceExtents(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height);
static void KWSDL_getTextureExtents(KW_RenderDriver * driver, KW_Texture * texture, unsigned * width, unsigned * height);
static void KWSDL_releaseTexture(KW_RenderDriver * driver, KW_Texture * texture);
static void KWSDL_releaseFont(KW_RenderDriver * driver, KW_Font * font);
static void KWSDL_blitSurface(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect);
static void KWSDL_releaseSurface(KW_RenderDriver * driver, KW_Surface * font);
static void KWSDL_setClipRect(KW_RenderDriver * driver, const KW_Rect * clip, int force);
static KW_bool KWSDL_getClipRect(KW_RenderDriver * driver, KW_Rect * clip);
static void KWSDL_release(KW_RenderDriver * driver);
static void KWSDL_utf8TextSize(KW_RenderDriver * driver, KW_Font * font, const char * text, unsigned * width, unsigned * height);
static unsigned KWSDL_getPixel(KW_RenderDriver * driver, KW_Surface * surface, unsigned px, unsigned py);
static void KWSDL_renderRect(KW_RenderDriver * driver, KW_Rect * rect, KW_Color color);
static void KWSDL_getViewportSize(KW_RenderDriver * driver, KW_Rect * rect);

struct KW_RenderDriver * KW_CreateSDL2RenderDriver(SDL_Renderer * renderer, SDL_Window * window) {
  struct KWSDL * kwsdl = calloc(sizeof(*kwsdl), 1);
  struct KW_RenderDriver * rd = calloc(sizeof(*rd), 1);

  TTF_Init();

  kwsdl->renderer = renderer;
  kwsdl->window = window;

  rd->renderCopy = KWSDL_renderCopy;
  rd->renderText = KWSDL_renderText;
  rd->utf8TextSize = KWSDL_utf8TextSize;
  rd->loadFont = KWSDL_loadFont;
  rd->loadFontFromMemory = KWSDL_loadFontFromMemory;
  rd->createTexture = KWSDL_createTexture;
  rd->createSurface = KWSDL_createRGBA32Surface;
  rd->loadTexture = KWSDL_loadTexture;
  rd->loadSurface = KWSDL_loadSurface;
  rd->getSurfaceExtents = KWSDL_getSurfaceExtents;
  rd->getTextureExtents = KWSDL_getTextureExtents;
  rd->blitSurface = KWSDL_blitSurface;
  rd->releaseFont = KWSDL_releaseFont;
  rd->releaseSurface = KWSDL_releaseSurface;
  rd->releaseTexture = KWSDL_releaseTexture;
  rd->setClipRect = KWSDL_setClipRect;
  rd->getClipRect = KWSDL_getClipRect;
  rd->getPixel = KWSDL_getPixel;
  rd->release = KWSDL_release;
  rd->renderRect = KWSDL_renderRect;
  rd->getViewportSize = KWSDL_getViewportSize;

  rd->priv = kwsdl;
  return rd;
}

void KWSDL_getViewportSize(KW_RenderDriver * driver, KW_Rect * rect) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Rect r;
  SDL_RenderGetViewport(kwsdl->renderer, &r);
  rect->x = r.x;
  rect->y = r.y;
  rect->w = r.w;
  rect->h = r.h;
}

void KWSDL_utf8TextSize(KW_RenderDriver * driver, KW_Font * font, const char * text, unsigned * width, unsigned * height) {
  (void) driver;
  TTF_SizeUTF8(font->font, text, (int*)width, (int*)height);
}

void KWSDL_renderRect(KW_RenderDriver * driver, KW_Rect * rect, KW_Color color) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  KW_Color old;
  SDL_Rect s;
  SDL_BlendMode blendMode;
  SDL_GetRenderDrawColor(kwsdl->renderer, &old.r, &old.g, &old.b, &old.a);
  SDL_GetRenderDrawBlendMode(kwsdl->renderer, &blendMode);

  SDL_SetRenderDrawBlendMode(kwsdl->renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(kwsdl->renderer, color.r, color.g, color.b, color.a);
  s.x = rect->x; s.y = rect->y; s.w = rect->w; s.h = rect->h;
  SDL_RenderFillRect(kwsdl->renderer, &s);
  SDL_SetRenderDrawBlendMode(kwsdl->renderer, blendMode);
  SDL_SetRenderDrawColor(kwsdl->renderer, old.r, old.g, old.b, old.a);
}

struct SDL_Renderer * KW_RenderDriverGetSDL2Renderer(struct KW_RenderDriver * driver) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  return kwsdl->renderer;
}

struct SDL_Window * KW_RenderDriverGetSDL2Window(struct KW_RenderDriver * driver) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  return kwsdl->window;
}

static KW_Texture * KWSDL_wrapTexture(SDL_Texture * texture) {
  KW_Texture * t = SDL_malloc(sizeof(KW_Texture));
  t->texture = texture;
  return t;
}

static KW_Surface * KWSDL_wrapSurface(SDL_Surface * surface) {
  KW_Surface * s = SDL_malloc(sizeof(KW_Surface));
  s->surface = surface;
  return s;
}

static KW_Font * KWSDL_wrapFont(TTF_Font * font) {
  KW_Font * f = SDL_malloc(sizeof(KW_Font));
  f->font = font;
  return f;
}

static KW_Texture * KWSDL_createTexture(KW_RenderDriver * driver, KW_Surface * surface) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Texture * t = SDL_CreateTextureFromSurface(kwsdl->renderer, surface->surface);
  SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
  return KWSDL_wrapTexture(t);
}

static KW_Surface * KWSDL_createRGBA32Surface(KW_RenderDriver * driver, unsigned width, unsigned height) {
  unsigned rmask, gmask, bmask, amask;
  SDL_Surface * s;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  (void) driver;
  s = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
  SDL_SetSurfaceBlendMode(s, SDL_BLENDMODE_NONE);
  return KWSDL_wrapSurface(s);
}

static KW_Font * KWSDL_loadFont(KW_RenderDriver * driver, const char * font, unsigned ptSize) {
  TTF_Font * f = TTF_OpenFont(font, ptSize);
  (void)driver;
  if (f == NULL) {
    fprintf(stdout, "KW_RenderDriver_SDL: Could not load font %s: %s\n", font, TTF_GetError());
    return NULL;
  }
  return KWSDL_wrapFont(f);
}

static KW_Font * KWSDL_loadFontFromMemory(KW_RenderDriver * driver, const void * font, unsigned long memsize, unsigned ptSize) {
  TTF_Font * f = TTF_OpenFontRW(SDL_RWFromConstMem(font, memsize), SDL_FALSE, ptSize);
  (void)driver;
  if (f == NULL) {
    return NULL;
  }
  return KWSDL_wrapFont(f);
}

static KW_Surface * KWSDL_loadSurface(KW_RenderDriver * driver, const char * texturefile) {
  SDL_Surface * s = IMG_Load(texturefile);
  (void) driver;
  if (s == NULL) {
    fprintf(stdout, "KW_RenderDriver_SDL: Could not load texture %s: %s\n", texturefile, IMG_GetError());
    return NULL;
  }
  SDL_SetSurfaceBlendMode(s, SDL_BLENDMODE_NONE);
  return KWSDL_wrapSurface(s);
}

static void KWSDL_getSurfaceExtents(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height) {
  (void)driver;
  if (width) *width = (unsigned)((SDL_Surface*)surface->surface)->w;
  if (height) *height = (unsigned)((SDL_Surface*)surface->surface)->h;
}

static void KWSDL_getTextureExtents(KW_RenderDriver * driver, KW_Texture * texture, unsigned * width, unsigned * height) {
  int w, h;
  (void)driver;
  SDL_QueryTexture(texture->texture, NULL, NULL, &w, &h);
  if (w < 0) w = 0;
  if (h < 0) h = 0;
  *width = (unsigned)w; *height = (unsigned)h;
}

static void KWSDL_blitSurface(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect) {
  SDL_Rect s, d;
  (void)driver;
  s.x = srcRect->x; s.y = srcRect->y; s.w = srcRect->w; s.h = srcRect->h;
  d.x = dstRect->x; d.y = dstRect->y; d.w = dstRect->w; d.h = dstRect->h;
  if (d.w != s.w || d.h != s.h) {
    SDL_BlitScaled(src->surface, &s, dst->surface, &d);
  } else {
    SDL_BlitSurface(src->surface, &s, dst->surface, &d);
  }
}

static KW_Texture * KWSDL_loadTexture(KW_RenderDriver * driver, const char * texturefile) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Texture * t = IMG_LoadTexture(kwsdl->renderer, texturefile);
  if (t == NULL) {
    fprintf(stdout, "KW_RenderDriver_SDL: Could not load texture %s: %s\n", texturefile, IMG_GetError());
    return NULL;
  }
  SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);

  return KWSDL_wrapTexture(t);
}

static KW_Texture * KWSDL_renderText(KW_RenderDriver * driver, KW_Font * kwfont, const char * text, KW_Color color, KW_RenderDriver_TextStyle style) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  int previousstyle;
  SDL_Color sdlcolor;
  SDL_Surface * textsurface;
  SDL_Texture * ret;
  TTF_Font * font = kwfont->font;

  sdlcolor.r = color.r, sdlcolor.g = color.g, sdlcolor.b = color.b, sdlcolor.a = color.a;
  if (font == NULL || text == NULL)
    return NULL;

  previousstyle = TTF_GetFontStyle(font);
  TTF_SetFontStyle(font, style);
  textsurface = TTF_RenderUTF8_Blended(font, text, sdlcolor);
  ret = SDL_CreateTextureFromSurface(kwsdl->renderer, textsurface);
  SDL_FreeSurface(textsurface);
  TTF_SetFontStyle(font, previousstyle);
  return KWSDL_wrapTexture(ret);
}

static void KWSDL_releaseTexture(KW_RenderDriver * driver, KW_Texture * texture) {
  (void)driver;
  SDL_DestroyTexture(texture->texture);
  SDL_free(texture);
}

static void KWSDL_releaseSurface(KW_RenderDriver * driver, KW_Surface * surface) {
  (void)driver;
  SDL_FreeSurface(surface->surface);
  SDL_free(surface);
}

static void KWSDL_releaseFont(KW_RenderDriver * driver, KW_Font * font) {
  (void)driver;
  TTF_CloseFont(font->font);
  SDL_free(font);
}

static void KWSDL_renderCopy(KW_RenderDriver * driver, KW_Texture * texture, const KW_Rect * src, const KW_Rect * dst) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Rect srcRect, dstRect;
  if (src) { srcRect.x = src->x; srcRect.y = src->y; srcRect.w = src->w; srcRect.h = src->h; }
  if (dst) { dstRect.x = dst->x; dstRect.y = dst->y; dstRect.w = dst->w; dstRect.h = dst->h; }
  SDL_RenderCopy(kwsdl->renderer, texture->texture, src ? &srcRect : NULL, dst? &dstRect : NULL);
}

static void KWSDL_setClipRect(KW_RenderDriver * driver, const KW_Rect * clip, int force) {
  SDL_Renderer * renderer = ((KWSDL *)driver->priv)->renderer;
  SDL_Rect cliprect;

  (void) force;

  if (!clip) {
    SDL_RenderSetClipRect(renderer, NULL);
  } else {
    cliprect.x = clip->x; cliprect.y = clip->y; cliprect.w = clip->w; cliprect.h = clip->h;
    SDL_RenderSetClipRect(renderer, &cliprect);
  }
}

static KW_bool KWSDL_getClipRect(KW_RenderDriver * driver, KW_Rect * clip) {
  SDL_Rect c;
  SDL_Renderer * renderer = ((KWSDL *)driver->priv)->renderer;
  SDL_RenderGetClipRect(renderer, &c);
  clip->x = c.x; clip->y = c.y; clip->w = c.w; clip->h = c.h;
  return (KW_bool) SDL_RenderIsClipEnabled(renderer);
}

static unsigned int KWSDL_getPixel(KW_RenderDriver * driver, KW_Surface * surface, unsigned x, unsigned y) {
  SDL_Surface * s = surface->surface;
  Uint32 * pixels = NULL;
  (void) driver;
  if (SDL_MUSTLOCK(s)) SDL_LockSurface(s);
  pixels = s->pixels;
  return *((Uint32 *) ((Uint8 *) pixels) + y * s->pitch + x * s->format->BytesPerPixel);

}

static void KWSDL_release(KW_RenderDriver * driver) {
  TTF_Quit();
  free(driver->priv);
  free(driver);
}
