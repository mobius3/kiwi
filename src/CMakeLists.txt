set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
message(STATUS Dir:: ${SDL2_INCLUDE_DIR})
add_library(KiWi SHARED KW_editbox_internal.c  utf8.c KW_editbox.c KW_eventwatcher.c KW_button.c KW_textrenderer.c KW_label.c KW_label_internal.c KW_gui.c KW_frame.c KW_tilerenderer.c KW_widget.c KW_widget_eventhandlers.c)
target_link_libraries(KiWi ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
