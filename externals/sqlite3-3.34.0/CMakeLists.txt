include_directories(.)
add_library(sqlite3 STATIC shell.c sqlite3.c)
set_target_properties (sqlite3 PROPERTIES FOLDER Externals/)
