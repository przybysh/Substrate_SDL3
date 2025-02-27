/*
    This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

#include "substrate.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502984
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

double radians(double d) { return d * M_PI / 180; }

double degrees(double r) { return (r * 180 / M_PI); }

static const double STEP = 0.42;

static const int GRID_INIT = 361;

static const int max_gain = 1000000;

static const SDL_Color kCrackColor = {0, 0, 0, 0xff};
static const SDL_Color kBGColor = {0xff, 0xfd, 0xe6, 0xff};

static const SDL_Color kDarkCrackColor = {0xff, 0xff, 0xff, 0xff};
static const SDL_Color kDarkBGColor = {0, 0x2, 0x19, 0xff};

static const SDL_Color kColorMap[] = {
    {0x20, 0x1f, 0x21, 0x0f}, {0x26, 0x2c, 0x2e, 0x0f},
    {0x35, 0x26, 0x26, 0x0f}, {0x37, 0x2b, 0x27, 0x0f},
    {0x30, 0x2c, 0x2e, 0x0f}, {0x39, 0x2b, 0x2d, 0x0f},
    {0x32, 0x32, 0x29, 0x0f}, {0x3f, 0x32, 0x29, 0x0f},
    {0x38, 0x32, 0x2e, 0x0f}, {0x2e, 0x33, 0x3d, 0x0f},
    {0x33, 0x3a, 0x3d, 0x0f}, {0x47, 0x33, 0x29, 0x0f},
    {0x40, 0x39, 0x2c, 0x0f}, {0x40, 0x39, 0x2e, 0x0f},
    {0x47, 0x40, 0x2c, 0x0f}, {0x47, 0x40, 0x2e, 0x0f},
    {0x4e, 0x40, 0x2c, 0x0f}, {0x4f, 0x40, 0x2e, 0x0f},
    {0x4e, 0x47, 0x38, 0x0f}, {0x58, 0x40, 0x37, 0x0f},
    {0x65, 0x47, 0x2d, 0x0f}, {0x6d, 0x5d, 0x3d, 0x0f},
    {0x74, 0x55, 0x30, 0x0f}, {0x75, 0x55, 0x32, 0x0f},
    {0x74, 0x5d, 0x32, 0x0f}, {0x74, 0x64, 0x33, 0x0f},
    {0x7c, 0x6c, 0x36, 0x0f}, {0x52, 0x31, 0x52, 0x0f},
    {0x44, 0x48, 0x42, 0x0f}, {0x4c, 0x56, 0x47, 0x0f},
    {0x65, 0x5d, 0x45, 0x0f}, {0x6d, 0x5d, 0x44, 0x0f},
    {0x6c, 0x5d, 0x4e, 0x0f}, {0x74, 0x6c, 0x43, 0x0f},
    {0x7c, 0x6c, 0x42, 0x0f}, {0x7c, 0x6c, 0x4b, 0x0f},
    {0x6b, 0x73, 0x4b, 0x0f}, {0x73, 0x73, 0x4b, 0x0f},
    {0x7b, 0x7b, 0x4a, 0x0f}, {0x6b, 0x6c, 0x55, 0x0f},
    {0x69, 0x6d, 0x5e, 0x0f}, {0x7b, 0x6c, 0x5d, 0x0f},
    {0x6b, 0x73, 0x53, 0x0f}, {0x6a, 0x74, 0x5d, 0x0f},
    {0x72, 0x7b, 0x52, 0x0f}, {0x7b, 0x7b, 0x52, 0x0f},
    {0x57, 0x74, 0x6e, 0x0f}, {0x68, 0x74, 0x66, 0x0f},
    {0x9c, 0x54, 0x2b, 0x0f}, {0x9d, 0x54, 0x32, 0x0f},
    {0x9d, 0x5b, 0x35, 0x0f}, {0x93, 0x6b, 0x36, 0x0f},
    {0xaa, 0x73, 0x30, 0x0f}, {0xc4, 0x5a, 0x27, 0x0f},
    {0xd9, 0x52, 0x23, 0x0f}, {0xd8, 0x5a, 0x20, 0x0f},
    {0xdb, 0x5a, 0x23, 0x0f}, {0xe5, 0x70, 0x37, 0x0f},
    {0x83, 0x6c, 0x4b, 0x0f}, {0x8c, 0x6b, 0x4b, 0x0f},
    {0x82, 0x73, 0x5c, 0x0f}, {0x93, 0x73, 0x52, 0x0f},
    {0x81, 0x7b, 0x63, 0x0f}, {0x81, 0x7b, 0x6d, 0x0f},
    {0x92, 0x7b, 0x63, 0x0f}, {0xd9, 0x89, 0x3b, 0x0f},
    {0xe4, 0x98, 0x32, 0x0f}, {0xdf, 0xa1, 0x33, 0x0f},
    {0xe5, 0xa0, 0x37, 0x0f}, {0xf0, 0xab, 0x3b, 0x0f},
    {0x8a, 0x8a, 0x59, 0x0f}, {0xb2, 0x9a, 0x58, 0x0f},
    {0x89, 0x82, 0x6b, 0x0f}, {0x9a, 0x82, 0x62, 0x0f},
    {0x88, 0x8b, 0x7c, 0x0f}, {0x90, 0x9a, 0x7a, 0x0f},
    {0xa2, 0x82, 0x62, 0x0f}, {0xa1, 0x8a, 0x69, 0x0f},
    {0xa9, 0x99, 0x68, 0x0f}, {0x99, 0xa1, 0x60, 0x0f},
    {0x99, 0xa1, 0x68, 0x0f}, {0xca, 0x81, 0x48, 0x0f},
    {0xeb, 0x8d, 0x43, 0x0f}, {0xc2, 0x91, 0x60, 0x0f},
    {0xc2, 0x91, 0x68, 0x0f}, {0xd1, 0xa9, 0x77, 0x0f},
    {0xc9, 0xb9, 0x7f, 0x0f}, {0xf0, 0xe2, 0x7b, 0x0f},
    {0x9f, 0x92, 0x8b, 0x0f}, {0xc0, 0xb9, 0x99, 0x0f},
    {0xe6, 0xb8, 0x8f, 0x0f}, {0xc8, 0xc1, 0x87, 0x0f},
    {0xe0, 0xc8, 0x86, 0x0f}, {0xf2, 0xcc, 0x85, 0x0f},
    {0xf5, 0xda, 0x83, 0x0f}, {0xec, 0xde, 0x9d, 0x0f},
    {0xf5, 0xd2, 0x94, 0x0f}, {0xf5, 0xda, 0x94, 0x0f},
    {0xf4, 0xe7, 0x84, 0x0f}, {0xf4, 0xe1, 0x8a, 0x0f},
    {0xf4, 0xe1, 0x93, 0x0f}, {0xe7, 0xd8, 0xa7, 0x0f},
    {0xf1, 0xd4, 0xa5, 0x0f}, {0xf1, 0xdc, 0xa5, 0x0f},
    {0xf4, 0xdb, 0xad, 0x0f}, {0xf1, 0xdc, 0xae, 0x0f},
    {0xf4, 0xdb, 0xb5, 0x0f}, {0xf5, 0xdb, 0xbd, 0x0f},
    {0xf4, 0xe2, 0xad, 0x0f}, {0xf5, 0xe9, 0xad, 0x0f},
    {0xf4, 0xe3, 0xbe, 0x0f}, {0xf5, 0xea, 0xbe, 0x0f},
    {0xf7, 0xf0, 0xb6, 0x0f}, {0xd9, 0xd1, 0xc1, 0x0f},
    {0xe0, 0xd0, 0xc0, 0x0f}, {0xe7, 0xd8, 0xc0, 0x0f},
    {0xf1, 0xdd, 0xc6, 0x0f}, {0xe8, 0xe1, 0xc0, 0x0f},
    {0xf3, 0xed, 0xc7, 0x0f}, {0xf6, 0xec, 0xce, 0x0f},
};

Substrate::Substrate(int width, int height, SDL_Renderer *renderer)
    : _num_cracks(20), _duration(60000 * 2), _grains(50), _circle_perc(30),
      _dark(false), _restart(false), _width(width), _height(height),
      _last_time_check(SDL_GetTicks()) {
  _rect.x = 0;
  _rect.y = 0;
  _rect.w = _width;
  _rect.h = _height;
  _crack_grid = new int *[_width];

  for (int i = 0; i < _width; i++) {
    _crack_grid[i] = new int[_height];
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  _substrate_texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                        SDL_TEXTUREACCESS_TARGET, width, height);

  restart(renderer);
}

Substrate::~Substrate() {
  for (int i = 0; i < _width; i++) {
    delete _crack_grid[i];
  }
  delete _crack_grid;
  if (_substrate_texture) {
    SDL_DestroyTexture(_substrate_texture);
  }
}

void Substrate::restart(SDL_Renderer *renderer) {
  _cracks.clear();
  for (int i = 0; i < _num_cracks; i++)
    add_crack(renderer);

  for (int x = 0; x < _width; x++)
    for (int y = 0; y < _height; y++)
      _crack_grid[x][y] = GRID_INIT;

  _restart = false;
  clear_substrate_texture(renderer);
  _last_time_check = SDL_GetTicks();
}

void Substrate::draw(SDL_Renderer *renderer) {
  if (_restart || SDL_GetTicks() - _last_time_check > _duration) {
    restart(renderer);
  }
  step_cracks(renderer);
}

void Substrate::add_crack(SDL_Renderer *renderer) {
  Crack c;

  int timeout = 10000;
  int x = 0;
  int y = 0;
  do {
    x = SDL_rand(_width);
    y = SDL_rand(_height);
    timeout--;
  } while (_crack_grid[x][y] > 360 && timeout > 0);

  if (timeout != 0) // Move perpendicular
    c.angle = _crack_grid[x][y] + (SDL_rand(1) ? 90 : -90);
  else // Or just pick random direction
    c.angle = SDL_rand(360);

  c.p.x = x;
  c.p.y = y;
  c.color = kColorMap[SDL_rand(ARRAY_SIZE(kColorMap))];
  if (_dark)
    c.color = invert_color(c.color);
  c.gain = SDL_rand(max_gain) / max_gain;

  if ((SDL_rand(100)) <= _circle_perc) {
    c.angle_drawn = 0;
    float r = 10 + (SDL_rand(((_width + _height) / 2)));
    if (SDL_rand(1))
      r *= -1;
    float radinc = STEP / r;
    c.angle_inc = degrees(radinc);
    c.ps.x = r * (1 - cos(radinc));
    c.ps.y = r * sin(radinc);
  } else {
    c.angle_inc = 0;
    c.ps.x = 0;
    c.ps.y = 0;
  }
  _cracks.push_back(c);
}

void Substrate::step_cracks(SDL_Renderer *renderer) {
  // view->SetPenSize(2);
  for (std::list<Crack>::iterator it = _cracks.begin(); it != _cracks.end();) {
    Crack &c = *it;
    Crack old = c;

    if (c.angle_inc != 0) {
      c.p.x += c.ps.y * cos(radians(c.angle));
      c.p.y += c.ps.y * sin(radians(c.angle));

      c.p.x += c.ps.x * cos(radians(c.angle) - M_PI / 2);
      c.p.y += c.ps.x * sin(radians(c.angle) - M_PI / 2);
      c.angle += c.angle_inc;
      c.angle_drawn += fabs(c.angle_inc);
    } else {
      c.p.x += STEP * cos(radians(c.angle));
      c.p.y += STEP * sin(radians(c.angle));
    }

    if (!SDL_PointInRectFloat(&c.p, &_rect) ||
        (_crack_grid[(int)c.p.x][(int)c.p.y] <= 360 &&
         fabs(_crack_grid[(int)c.p.x][(int)c.p.y] - c.angle) > 5) ||
        (c.angle_inc != 0 && c.angle_drawn > 360)) {
      it = _cracks.erase(it);
      add_crack(renderer);
      continue;
    } else {
      ++it;
    }

    _crack_grid[(int)c.p.x][(int)c.p.y] = (int)c.angle;

    draw_sand(&c, renderer);
    SDL_Color useColor = _dark ? kDarkCrackColor : kCrackColor;
    SDL_SetRenderTarget(renderer, _substrate_texture);
    SDL_SetRenderDrawColor(renderer, useColor.r, useColor.g, useColor.b,
                           useColor.a);
    SDL_RenderLine(renderer, old.p.x, old.p.y, c.p.x, c.p.y);
  }
}

void Substrate::draw_sand(Crack *c, SDL_Renderer *renderer) {
  SDL_FPoint r(c->p);
  // We need some headstart here
  r.x += (4 * sin(radians(c->angle)));
  r.y -= (4 * cos(radians(c->angle)));
  // Move perpendicular until we hit the edge or another crack
  while (true) {
    r.x += (0.81 * sin(radians(c->angle)));
    r.y -= (0.81 * cos(radians(c->angle)));

    int rx = (int)r.x;
    int ry = (int)r.y;
    if (rx < 0 || rx >= _width || ry < 0 || ry >= _height ||
        _crack_grid[rx][ry] <= 360) {
      break;
    }
  }

  c->gain += (SDL_rand((float)max_gain - 0.5) / (float)max_gain - 0.5) * 0.2;
  if (c->gain < 0)
    c->gain = 0;
  if (c->gain > 1)
    c->gain = 1;

  float w = c->gain / (_grains - 1);
  SDL_SetRenderTarget(renderer, _substrate_texture);
  SDL_SetRenderDrawColor(renderer, c->color.r, c->color.g, c->color.b,
                         c->color.a);
  int gx_beg = (int)(c->p.x + (r.x - c->p.x) * sin(sin(0 * w)));
  int gy_beg = (int)(c->p.y + (r.y - c->p.y) * sin(sin(0 * w)));
  int gx_end = (int)(c->p.x + (r.x - c->p.x) * sin(sin((_grains - 1) * w)));
  int gy_end = (int)(c->p.y + (r.y - c->p.y) * sin(sin((_grains - 1) * w)));
  SDL_RenderLine(renderer, gx_beg, gy_beg, gx_end, gy_end);
}

SDL_Color invert_color(SDL_Color clr) {
  clr.r = 0xff - clr.r;
  clr.g = 0xff - clr.g;
  clr.b = 0xff - clr.b;
  return clr;
}

void Substrate::clear_substrate_texture(SDL_Renderer *renderer) {

  SDL_SetRenderTarget(renderer, _substrate_texture);
  SDL_Color bg_color = _dark ? kDarkBGColor : kBGColor;
  SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b,
                         bg_color.a);
  SDL_RenderClear(renderer);
}