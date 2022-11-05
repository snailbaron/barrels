#include "config.hpp"
#include "view.hpp"
#include "world.hpp"

#include <tempo.hpp>

#include <SDL.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    auto world = World{};
    world.init();

    auto view = View{};

    events.deliver();

    auto timer = tempo::FrameTimer{config().gameFps};
    size_t frameIndex = 0;
    for (;;) {
        if (!view.processInput()) {
            break;
        }
        events.deliver();

        if (int framesPassed = timer(); framesPassed > 0) {
            frameIndex += framesPassed;
            events.push(evt::Frame{.frameIndex = frameIndex});

            for (int i = 0; i < framesPassed; i++) {
                world.update(timer.delta());
            }
            events.deliver();

            view.update(timer.delta() * framesPassed);
            view.present();
        }
    }

    SDL_Quit();
}
