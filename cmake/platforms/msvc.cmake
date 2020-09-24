add_compile_definitions(NOMINMAX)

add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:RelWithDebInfo>:/MTd>
        $<$<CONFIG:Release>:/MT> #--|
)