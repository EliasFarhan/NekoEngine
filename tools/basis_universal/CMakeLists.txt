project(basisu)

cmake_minimum_required(VERSION 3.0)


set(BASISU_SRC_LIST ${COMMON_SRC_LIST} 
	basisu_backend.cpp
	basisu_basis_file.cpp
	basisu_comp.cpp
	basisu_enc.cpp
	basisu_etc.cpp
	basisu_frontend.cpp
	basisu_global_selector_palette_helpers.cpp
	basisu_gpu_texture.cpp
	basisu_pvrtc1_4.cpp
	basisu_resampler.cpp
	basisu_resample_filters.cpp
	basisu_ssim.cpp
	basisu_tool.cpp
	basisu_astc_decomp.cpp
	basisu_uastc_enc.cpp
	basisu_bc7enc.cpp
	lodepng.cpp
	apg_bmp.c
	jpgd.cpp
	transcoder/basisu_transcoder.cpp
	)
add_executable(basisu ${BASISU_SRC_LIST})

if (NOT MSVC)
   target_link_libraries(basisu m pthread)
endif()
set_target_properties (basisu PROPERTIES FOLDER Neko/Tools)