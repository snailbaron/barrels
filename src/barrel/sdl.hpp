#pragma once

#include "error.hpp"

#include <SDL.h>

class SdlError : public Error {};

template <class T>
T* sdlCheck(T* ptr)
{
    if (!ptr) {
        throw SdlError{} << SDL_GetError();
    }
    return ptr;
}

inline void sdlCheck(int statusCode)
{
    if (statusCode != 0) {
        throw SdlError{} << SDL_GetError();
    }
}
