
#add_compile_options(
#        $<$<CONFIG:>:/MT> #---------|
#        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
#        $<$<CONFIG:RelWithDebInfo>:/MTd>
#        $<$<CONFIG:Release>:/MT> #--|
#)#

SET(NEKO_COMPILE_FLAGS  "/arch:AVX2;/Oi;/GR-;/EHs-c-;/GL;/fp:fast" CACHE INTERNAL "Compile flags")
SET(NEKO_LINK_FLAGS  "/LTCG" CACHE INTERNAL "Link flags")

SET(NEKO_EDITOR_COMPILE_FLAGS  "/arch:AVX2;/Oi;/EHs-c-;/GL;/fp:fast" CACHE INTERNAL "Compile flags")
SET(NEKO_EDITOR_LINK_FLAGS  "/LTCG" CACHE INTERNAL "Link flags")