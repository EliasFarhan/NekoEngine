add_library(imgui STATIC imgui_demo.cpp imgui_widgets.cpp imgui_draw.cpp
        imgui.cpp imgui_tables.cpp imgui_widgets.cpp)
set_target_properties (imgui PROPERTIES FOLDER Externals)
target_include_directories(imgui PUBLIC "./")
