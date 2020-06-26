
LIST(APPEND ASSIMP_WRAPPER_LIBS Neko_Core assimp gles3_wrapper)
file(GLOB_RECURSE assimp_wrapper_src include/*.h src/*.cpp)

add_library(assimp_wrapper STATIC ${assimp_wrapper_src})
target_link_libraries(assimp_wrapper PUBLIC ${ASSIMP_WRAPPER_LIBS})

target_include_directories(assimp_wrapper PUBLIC "include/")
neko_lib_config(assimp_wrapper)
#if(Emscripten)
#set_property(TARGET assimp APPEND_STRING PROPERTY LINK_FLAGS " --shared-memory --no-check-features ")
#set_property(TARGET IrrXML APPEND_STRING PROPERTY LINK_FLAGS " --shared-memory --no-check-features ")
#set_property(TARGET zlibstatic APPEND_STRING PROPERTY LINK_FLAGS " --shared-memory --no-check-features ")
#endif()
set_target_properties (assimp_wrapper PROPERTIES FOLDER Neko/Common)