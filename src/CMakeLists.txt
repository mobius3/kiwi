set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(API_HEADERS
  KW_gui.h
  KW_button.h
  KW_editbox.h
  KW_frame.h
  KW_label.h
  KW_macros.h
  KW_tilerenderer.h
  KW_textrenderer.h
  KW_widget.h
)

include_directories(${SDL2_INCLUDE_DIR})
add_library(KiWi SHARED KW_editbox_internal.c utf8.c KW_editbox.c KW_eventwatcher.c KW_button.c KW_textrenderer.c KW_label.c KW_label_internal.c KW_gui.c KW_frame.c KW_tilerenderer.c KW_widget.c KW_widget_eventhandlers.c)
target_link_libraries(KiWi ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# Configure install stuff
install(TARGETS KiWi EXPORT KiWiTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(FILES ${API_HEADERS} DESTINATION include/KiWi)
