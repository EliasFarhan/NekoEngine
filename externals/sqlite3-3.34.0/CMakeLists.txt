
add_library(sqlite3 STATIC shell.c sqlite3.c)
target_include_directories(sqlite3 PUBLIC .)
set_target_properties (sqlite3 PROPERTIES FOLDER Externals/)
