#ifndef STYLE_H_
#define STYLE_H_

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#    define RGBA32_UNPACK(color) ((color >> (8 * 0)) & 0xFF), ((color >> (8 * 1)) & 0xFF), ((color >> (8 * 2)) & 0xFF), ((color >> (8 * 3)) & 0xFF)
#    define WITH_ALPHA(color, alpha) ((color & 0x00FFFFFF) | alpha)
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
#    define RGBA32_UNPACK(color) ((color >> (8 * 3)) & 0xFF), ((color >> (8 * 2)) & 0xFF), ((color >> (8 * 1)) & 0xFF), ((color >> (8 * 0)) & 0xFF)
#    define WITH_ALPHA(color, alpha) ((color & 0xFFFFFF00) | alpha)
#else
#    error "Unknown endianess"
#endif

#define BACKGROUND_COLOR 0xFF181818
#define GRID_COLOR 0xFF282828
#define PINK_COLOR 0xFFBBAAFF
#define BLUE_COLOR 0xFFFFBBAA
#define YELLOW_COLOR 0xFFAAFFFF
#define RED_COLOR 0xFF0000FF

#endif  // STYLE_H_
