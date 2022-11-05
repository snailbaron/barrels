add_executable(barrel
    config.cpp
    main.cpp
    view.cpp
    world.cpp
)
target_link_libraries(barrel
    SDL2::SDL2-static

    evening
    tempo
    thing
    ve
)
