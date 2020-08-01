add_executable(label label.c)
target_link_libraries(label kiwi::core kiwi::widgets kiwi::sdl2 c-rez::tilesets)