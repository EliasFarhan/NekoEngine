project(Remotery)


include_directories(.)

if(APPLE)
    add_library(Remotery STATIC Remotery.c RemoteryMetal.mm)
else()
    add_library(Remotery STATIC Remotery.c)
    if(UNIX)
        target_link_libraries(Remotery dl m pthread)
    endif(UNIX)
endif(APPLE)
target_compile_definitions(Remotery PUBLIC RMT_USE_OPENGL=1)