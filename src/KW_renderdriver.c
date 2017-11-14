#include "KW_renderdriver.h"

void KW_BlitSurface(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect) {
    driver->blitSurface(driver, src, srcRect, dst, dstRect);
}

KW_Surface * KW_CreateSurface(KW_RenderDriver * driver, unsigned width, unsigned height) {
    return driver->createSurface(driver, width, height);
}

void KW_GetSurfaceExtents(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height) {
    driver->getSurfaceExtents(driver, surface, width, height);
}

void KW_GetTextureExtents(KW_RenderDriver * driver, KW_Texture * texture, unsigned * width, unsigned * height) {
    driver->getTextureExtents(driver, texture, width, height);
}

void KW_RenderCopy(KW_RenderDriver * driver, KW_Texture * src, const KW_Rect * clip, const KW_Rect * dstRect) {
    driver->renderCopy(driver, src, clip, dstRect);
}

KW_Texture * KW_RenderText(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color,
                  KW_RenderDriver_TextStyle style) {
    return driver->renderText(driver, font, text, color, style);
}

void KW_RenderRect(KW_RenderDriver * driver, KW_Rect * rect, KW_Color color) {
    driver->renderRect(driver, rect, color);
}

KW_Font * KW_LoadFont(KW_RenderDriver * driver, const char * fontFile, unsigned ptSize) {
    return driver->loadFont(driver, fontFile, ptSize);
}

KW_Font * KW_LoadFontFromMemory(KW_RenderDriver * driver, const void * fontMemory, unsigned long memSize, unsigned ptSize) {
    return driver->loadFontFromMemory(driver, fontMemory, memSize, ptSize);
}

KW_Texture * KW_CreateTexture(KW_RenderDriver * driver, KW_Surface * surface) {
    return driver->createTexture(driver, surface);
}

KW_Texture * KW_LoadTexture(KW_RenderDriver * driver, const char * file) {
    return driver->loadTexture(driver, file);
}

KW_Surface * KW_LoadSurface(KW_RenderDriver * driver, const char * file) {
    return driver->loadSurface(driver, file);
}

void KW_ReleaseTexture(KW_RenderDriver * driver, KW_Texture * texture) {
    driver->releaseTexture(driver, texture);
}

void KW_ReleaseSurface(KW_RenderDriver * driver, KW_Surface * surface) {
    driver->releaseSurface(driver, surface);
}

void KW_ReleaseFont(KW_RenderDriver * driver, KW_Font * font) {
    driver->releaseFont(driver, font);
}

void KW_SetClipRect(KW_RenderDriver * driver, const KW_Rect * clip, int force) {
  driver->setClipRect(driver, clip, force);
}
KW_bool KW_GetClipRect(KW_RenderDriver * driver, KW_Rect * clip) {
  return driver->getClipRect(driver, clip);
}
void KW_ReleaseRenderDriver(KW_RenderDriver * driver) {
    driver->release(driver);
}

void KW_UTF8TextSize(KW_RenderDriver * driver, KW_Font * font, const char * text, unsigned * width, unsigned * height) {
    driver->utf8TextSize(driver, font, text, width, height);
}

unsigned KW_GetPixel(KW_RenderDriver * driver, KW_Surface * surface, unsigned px, unsigned py) {
   return driver->getPixel(driver, surface, px, py);
}


static int imax(int i, int j) {
  return (i < j) ? j : i;
}

static int imin(int i, int j) {
  return (i < j) ? i : j;
}

KW_Color KW_MultiplyColor(KW_Color color, float amount) {
  KW_Color r;
  r.r = (unsigned char) imin(imax((int)(color.r * amount), 255), 0);
  r.g = (unsigned char) imin(imax((int)(color.g * amount), 255), 0);
  r.b = (unsigned char) imin(imax((int)(color.b * amount), 255), 0);
  r.a = color.a;
  return r;
}

void KW_GetViewportSize(KW_RenderDriver * driver, KW_Rect * rect) {
    driver->getViewportSize(driver, rect);
}
