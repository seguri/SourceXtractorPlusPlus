# - Locate the readline library
# Defines:
#
#  READLINE_FOUND
#  READLINE_INCLUDE_DIR
#  READLINE_INCLUDE_DIRS (not cached)
#  READLINE_LIBRARY
#  READLINE_LIBRARIES (not cached)


if(NOT READLINE_FOUND)

    find_path(READLINE_INCLUDE_DIR readline/readline.h
            HINTS ENV READLINE_ROOT_DIR READLINE_INSTALL_DIR)

    find_library(READLINE_LIBRARY readline
            HINTS ENV READLINE_ROOT_DIR READLINE_INSTALL_DIR
            PATH_SUFFIXES lib)

    set(READLINE_LIBRARIES ${READLINE_LIBRARY})
    set(READLINE_INCLUDE_DIRS ${READLINE_INCLUDE_DIR})

    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Readline DEFAULT_MSG READLINE_INCLUDE_DIRS READLINE_LIBRARIES)

    mark_as_advanced(READLINE_FOUND READLINE_INCLUDE_DIRS READLINE_LIBRARIES)

    list(REMOVE_DUPLICATES READLINE_LIBRARIES)
    list(REMOVE_DUPLICATES READLINE_INCLUDE_DIRS)

endif()