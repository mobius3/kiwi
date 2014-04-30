find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
add_library(KiWi SHARED KW_eventwatcher.c KW_button.c KW_textrenderer.c KW_label.c KW_gui.c KW_frame.c KW_tilerenderer.c KW_widget.c)
target_link_libraries(KiWi ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
