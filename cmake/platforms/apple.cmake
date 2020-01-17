	add_compile_options(-fno-rtti)

    find_package(Boost 1.66.0 REQUIRED COMPONENTS
            program_options
            filesystem
            system
            date_time
            chrono
            )

    if(NOT Boost_FOUND)
        message(FATAL_ERROR "Boost not found")
    endif()

    include_directories(${Boost_INCLUDE_DIRS})
    LIST(APPEND Neko_LIBRARIES ${Boost_LIBRARIES})