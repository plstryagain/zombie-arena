set(SOURCES main.cpp
            player.cpp
            create_background.cpp
            texture_holder.cpp
            zombie.cpp
            create_horde.cpp
            bullet.cpp
            pickup.cpp
)

add_library(main_obj OBJECT ${SOURCES})