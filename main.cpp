/*
 * 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
 *
 */
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include "substrate.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

static const int window_width = 800;
static const int window_height = 600;

static Substrate *substrate = nullptr;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("SUBSTRATE", window_width, window_height,
                                   SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderVSync(renderer, 1);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  substrate = new Substrate(window_width, window_height, renderer);
  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT || event->key.key == SDLK_ESCAPE) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  } else if (event->key.key == SDLK_SPACE && event->type == SDL_EVENT_KEY_UP) {
    substrate->force_restart();
  } else if (event->key.key == SDLK_S && event->type == SDL_EVENT_KEY_UP) {
    substrate->switch_dark();
  }

  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  const char *message = "Hello World!";
  int w = 0, h = 0;
  float x, y;
  const float scale = 4.0f;

  substrate->draw(renderer);
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderTexture(renderer, substrate->get__substrate_texture(), nullptr,
                    nullptr);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (substrate) {
    delete substrate;
  }
}
