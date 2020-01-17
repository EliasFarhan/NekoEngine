include_directories(".")
add_library(imgui STATIC imgui_demo.cpp imgui_widgets.cpp imgui_draw.cpp imgui.cpp)
set_target_properties (imgui PROPERTIES FOLDER Externals)
