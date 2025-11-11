#include <route/Route.hpp>
#include <pages/Page.hpp>
#include <pages/Home.hpp>
#include <pages/Battlefields.hpp>
#include <iostream>

Home* home;

Route::Route(SDL_Renderer* renderer) : currentPage(nullptr), renderer(renderer) {
    setPage(new Home(*this));
    defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

    // set Font chữ mặc định
    TTF_Font* fontLoad = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    setFont(fontLoad);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    // set nhạc mặc định
    Mix_Music* bgMusic = Mix_LoadMUS("../assets/music/victory.mp3");
    if (!bgMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    }
    Mix_PlayMusic(bgMusic, -1);
    setBgMusic(bgMusic);
}

Route::~Route() {
    for (auto const& [path, cursor] : cursorCache) {
        SDL_FreeCursor(cursor);
    }
    SDL_FreeCursor(defaultCursor);

    if (bgMusic) {
        Mix_FreeMusic(bgMusic); 
    }

    if (font) {
        TTF_CloseFont(font);
    }

    for (auto const& [path, sound] : soundCache) {
        Mix_FreeChunk(sound);
    }
}

void Route::handleEvent(SDL_Event& e) {
    if (currentPage) currentPage->handleEvent(e);
}

void Route::setPage(Page* page) {
    if (currentPage) delete currentPage;
    currentPage = page;
}

void Route::render() {
    if (currentPage) {
        currentPage->render(renderer);
    }
}

void Route::update() {
    if (currentPage) {
        currentPage->update();
    }
}

void Route::SetCursor(const std::string& path, int hot_x, int hot_y) {
    if (cursorCache.find(path) != cursorCache.end()) {
        SDL_SetCursor(cursorCache[path]);
        return;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load cursor image: " << path << " | Error: " << IMG_GetError() << std::endl;
        SetDefaultCursor();
        return;
    }

    SDL_Cursor* newCursor = SDL_CreateColorCursor(surface, hot_x, hot_y);
    SDL_FreeSurface(surface);

    if (!newCursor) {
        std::cerr << "Failed to create custom cursor from: " << path << " | Error: " << SDL_GetError() << std::endl;
        SetDefaultCursor();
        return;
    }

    cursorCache[path] = newCursor;
    SDL_SetCursor(newCursor);
}

void Route::SetDefaultCursor() {
    if (defaultCursor) {
        SDL_SetCursor(defaultCursor);
    }
}

void Route::setFont(TTF_Font* currentFont) {
    font = currentFont;
}

TTF_Font* Route::getFont() {
    return font;
}

void Route::setBgMusic(Mix_Music* music) {
    if (bgMusic && bgMusic != music) {
        Mix_FreeMusic(bgMusic);
    }

    bgMusic = music;

    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    if (bgMusic) {
        Mix_PlayMusic(bgMusic, -1);
    }
}

Mix_Music* Route::getBgMusic() {
    return bgMusic;
}

void Route::playSoundEffect(const std::string& path) {
    Mix_Chunk* sound = nullptr;

    if (soundCache.find(path) != soundCache.end()) {
        sound = soundCache[path];
    }
    else {
        sound = Mix_LoadWAV(path.c_str()); 
        if (!sound) {
            std::cerr << "Failed to load sound effect: " << path << " | Error: " << Mix_GetError() << std::endl;
            return;
        }
        soundCache[path] = sound;
    }

    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        std::cerr << "Failed to play sound effect: " << Mix_GetError() << std::endl;
    }
}