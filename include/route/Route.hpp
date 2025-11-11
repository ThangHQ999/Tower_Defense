#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <map>

#include <pages/Page.hpp>

class Page;
class Route {
private:
    SDL_Renderer* renderer;
    Page* currentPage;
    std::map<std::string, SDL_Cursor*> cursorCache;
    std::map<std::string, Mix_Chunk*> soundCache;
    SDL_Cursor* defaultCursor;
    TTF_Font* font;
    Mix_Music* bgMusic;
public:
    Route(SDL_Renderer* renderer);
    ~Route();

    void handleEvent(SDL_Event& e);
    void render();
    void update();
    void setPage(Page* page);
    SDL_Renderer* getRenderer() const { return renderer; }

    // Cursor
    void SetCursor(const std::string& path, int hot_x = 0, int hot_y = 0);
    void SetDefaultCursor();

    // set Font
    void setFont(TTF_Font* font);
    TTF_Font* getFont();
    // Set Âm thanh
    void setBgMusic(Mix_Music* bgMusic);
    Mix_Music* getBgMusic();
    void playSoundEffect(const std::string& path);
};
