#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <string>
#include <SDL2/SDL_render.h>

struct TextLine {
    std::string text;
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Color color;
};

#endif