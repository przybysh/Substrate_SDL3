/*
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
 */

#ifndef _SUBSTRATE_H
#define _SUBSTRATE_H

#include <SDL3/SDL.h>

#include <list>

struct Crack {
  SDL_FPoint p;
  SDL_FPoint ps;
  SDL_Color color;
  double angle;
  double angle_inc;
  double angle_drawn;
  double gain;
};

class Substrate {
public:
  Substrate(int width, int height, SDL_Renderer *renderer);
  ~Substrate();

  void draw(SDL_Renderer *renderer);
  void force_restart() { _restart = true; }
  void switch_dark() {
    _dark = !_dark;
    force_restart();
  }
  SDL_Texture *get__substrate_texture() { return _substrate_texture; }

private:
  void add_crack(SDL_Renderer *renderer);
  void step_cracks(SDL_Renderer *renderer);
  void draw_sand(Crack *c, SDL_Renderer *renderer);
  void restart(SDL_Renderer *renderer);
  void clear_substrate_texture(SDL_Renderer *renderer);

  int _width;
  int _height;
  SDL_FRect _rect;

  std::list<Crack> _cracks;
  int **_crack_grid;

  int _num_cracks;
  int _duration;
  int _grains;
  int _circle_perc;
  bool _dark;

  bool _restart;
  SDL_Texture *_substrate_texture;

  uint64_t _last_time_check;
};

SDL_Color invert_color(SDL_Color clr);

#endif