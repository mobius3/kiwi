add_executable(label label.c)
target_link_libraries(label kiwi::core kiwi::widgets c-rez::tilesets)