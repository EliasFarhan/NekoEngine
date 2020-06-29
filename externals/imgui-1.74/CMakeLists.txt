
add_library(imgui STATIC imgui_demo.cpp imgui_widgets.cpp imgui_draw.cpp imgui.cpp)
set_target_properties (imgui PROPERTIES FOLDER Externals)
target_include_directories(imgui PUBLIC "./")
if(Emscripten)
    #set_property(TARGET imgui APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--shared-memory,--no-check-features ")
endif()