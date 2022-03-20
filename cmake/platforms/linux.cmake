SET(NEKO_COMPILE_FLAGS  "-march=haswell;-fno-rtti;-fno-exceptions;-flto;-ffast-math" CACHE INTERNAL "Compile flags")
SET(NEKO_LINK_FLAGS  "-flto" CACHE INTERNAL "Link flags")

SET(NEKO_EDITOR_COMPILE_FLAGS  "-march=haswell;-flto;-ffast-math" CACHE INTERNAL "Compile flags")
SET(NEKO_EDITOR_LINK_FLAGS  "-flto" CACHE INTERNAL "Link flags")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
