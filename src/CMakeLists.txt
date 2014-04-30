find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
add_library(GuiGUI SHARED GUI_eventwatcher.c GUI_button.c GUI_textrenderer.c GUI_label.c GUI_gui.c GUI_frame.c GUI_tilerenderer.c GUI_widget.c)
target_link_libraries(GuiGUI ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
