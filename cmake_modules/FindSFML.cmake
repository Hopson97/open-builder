if(SFML_STATIC_LIBRARIES)
    add_definitions(-DSFML_STATIC)
endif()
set(FIND_SFML_PATHS
    ${SFML_ROOT}
    $ENV{SFML_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

find_path(SFML_INCLUDE_DIR SFML/Config.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_SFML_PATHS})

# Added to module, not in original
# Used for shipping the game.
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_path(SFML_BIN_DIR openal32.dll
          PATH_SUFFIXES bin
          PATHS ${FIND_SFML_PATHS})
    if(SFML_BIN_DIR)
        message(STATUS "Found binary release at ${SFML_BIN_DIR}")
        # Now seperate all the release/debug dll's
        foreach(FIND_SFML_COMPONENT ${SFML_FIND_COMPONENTS})
            string(TOLOWER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_LOWER)
            set(FIND_SFML_COMPONENT_NAME sfml-${FIND_SFML_COMPONENT_LOWER})
            if(CMAKE_BUILD_TYPE MATCHES Debug)
                # Set debug dll's'
                list(APPEND SFML_BINARIES ${FIND_SFML_COMPONENT_NAME}-d-2.dll)
            else()
                # Set release dll's
                list(APPEND SFML_BINARIES ${FIND_SFML_COMPONENT_NAME}-2.dll)
            endif()
        endforeach()
    else()
        message(SEND_ERROR "Could not find SFML dll's!")
    endif()
endif()

set(SFML_VERSION_OK TRUE)
if(SFML_FIND_VERSION AND SFML_INCLUDE_DIR)
    if("${SFML_INCLUDE_DIR}" MATCHES "SFML.framework")
        set(SFML_CONFIG_HPP_INPUT "${SFML_INCLUDE_DIR}/Headers/Config.hpp")
    else()
        set(SFML_CONFIG_HPP_INPUT "${SFML_INCLUDE_DIR}/SFML/Config.hpp")
    endif()
    FILE(READ "${SFML_CONFIG_HPP_INPUT}" SFML_CONFIG_HPP_CONTENTS)
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MAJOR ([0-9]+).*" "\\1" SFML_VERSION_MAJOR "${SFML_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SFML_VERSION_MINOR ([0-9]+).*" "\\1" SFML_VERSION_MINOR "${SFML_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SFML_VERSION_PATCH ([0-9]+).*" "\\1" SFML_VERSION_PATCH "${SFML_CONFIG_HPP_CONTENTS}")
    if (NOT "${SFML_VERSION_PATCH}" MATCHES "^[0-9]+$")
        set(SFML_VERSION_PATCH 0)
    endif()
    math(EXPR SFML_REQUESTED_VERSION "${SFML_FIND_VERSION_MAJOR} * 10000 + ${SFML_FIND_VERSION_MINOR} * 100 + ${SFML_FIND_VERSION_PATCH}")
    if (SFML_VERSION_MAJOR)
        math(EXPR SFML_VERSION "${SFML_VERSION_MAJOR} * 10000 + ${SFML_VERSION_MINOR} * 100 + ${SFML_VERSION_PATCH}")
        if(SFML_VERSION LESS SFML_REQUESTED_VERSION)
            set(SFML_VERSION_OK FALSE)
        endif()
    else()
        if (SFML_REQUESTED_VERSION GREATER 10900)
            set(SFML_VERSION_OK FALSE)
            set(SFML_VERSION_MAJOR 1)
            set(SFML_VERSION_MINOR x)
            set(SFML_VERSION_PATCH x)
        endif()
    endif()
endif()

set(SFML_FOUND TRUE) 
foreach(FIND_SFML_COMPONENT ${SFML_FIND_COMPONENTS})
    string(TOLOWER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_LOWER)
    string(TOUPPER ${FIND_SFML_COMPONENT} FIND_SFML_COMPONENT_UPPER)
    set(FIND_SFML_COMPONENT_NAME sfml-${FIND_SFML_COMPONENT_LOWER})

    if(FIND_SFML_COMPONENT_LOWER STREQUAL "main")
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE
                     NAMES ${FIND_SFML_COMPONENT_NAME}
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG
                     NAMES ${FIND_SFML_COMPONENT_NAME}-d
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})
    else()
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE
                     NAMES ${FIND_SFML_COMPONENT_NAME}-s
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG
                     NAMES ${FIND_SFML_COMPONENT_NAME}-s-d
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
                     NAMES ${FIND_SFML_COMPONENT_NAME}
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})
        find_library(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG
                     NAMES ${FIND_SFML_COMPONENT_NAME}-d
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SFML_PATHS})
        if(SFML_STATIC_LIBRARIES)
            if(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE)
                set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE})
            endif()
            if(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG)
                set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG})
            endif()
        else()
            if(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE)
                set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE})
            endif()
            if(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG)
                set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG})
            endif()
        endif()
    endif()

    if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG OR SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE)
        set(SFML_${FIND_SFML_COMPONENT_UPPER}_FOUND TRUE)
        if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG AND SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE)
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY debug     ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG}
                                                          optimized ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE})
        endif()
         
        if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG AND NOT SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE)
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG})
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY         ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG})
        endif()
        if (SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE AND NOT SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG)
             
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE})
            set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY       ${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE})
        endif()
    else()
         
        set(SFML_FOUND FALSE)
        set(SFML_${FIND_SFML_COMPONENT_UPPER}_FOUND FALSE)
        set(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY "")
        set(FIND_SFML_MISSING "${FIND_SFML_MISSING} SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY")
    endif()

    MARK_AS_ADVANCED(SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_RELEASE
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DEBUG
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
                     SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG)

     
    set(SFML_LIBRARIES ${SFML_LIBRARIES} "${SFML_${FIND_SFML_COMPONENT_UPPER}_LIBRARY}")
endforeach()
 
if(SFML_STATIC_LIBRARIES)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(FIND_SFML_OS_WINDOWS 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(FIND_SFML_OS_LINUX 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
        set(FIND_SFML_OS_FREEBSD 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(FIND_SFML_OS_MACOSX 1)
    endif()
     
    set(SFML_DEPENDENCIES)
    set(FIND_SFML_DEPENDENCIES_NOTFOUND)

    macro(find_sfml_dependency output friendlyname)
        find_library(${output} NAMES ${ARGN} PATHS ${FIND_SFML_PATHS} PATH_SUFFIXES lib NO_SYSTEM_ENVIRONMENT_PATH)
        if(${${output}} STREQUAL "${output}-NOTFOUND")
            unset(output)
            set(FIND_SFML_DEPENDENCIES_NOTFOUND "${FIND_SFML_DEPENDENCIES_NOTFOUND} ${friendlyname}")
        endif()
    endmacro()

    list(FIND SFML_FIND_COMPONENTS "system" FIND_SFML_SYSTEM_COMPONENT)
    if(NOT ${FIND_SFML_SYSTEM_COMPONENT} EQUAL -1)
        if(FIND_SFML_OS_LINUX OR FIND_SFML_OS_FREEBSD OR FIND_SFML_OS_MACOSX)
            set(SFML_SYSTEM_DEPENDENCIES "pthread")
        endif()
        if(FIND_SFML_OS_LINUX)
            set(SFML_SYSTEM_DEPENDENCIES ${SFML_SYSTEM_DEPENDENCIES} "rt")
        endif()
        if(FIND_SFML_OS_WINDOWS)
            set(SFML_SYSTEM_DEPENDENCIES "winmm")
        endif()
        set(SFML_DEPENDENCIES ${SFML_SYSTEM_DEPENDENCIES} ${SFML_DEPENDENCIES})
    endif()
     
    list(FIND SFML_FIND_COMPONENTS "network" FIND_SFML_NETWORK_COMPONENT)
    if(NOT ${FIND_SFML_NETWORK_COMPONENT} EQUAL -1)
        if(FIND_SFML_OS_WINDOWS)
            set(SFML_NETWORK_DEPENDENCIES "ws2_32")
        endif()
        set(SFML_DEPENDENCIES ${SFML_NETWORK_DEPENDENCIES} ${SFML_DEPENDENCIES})
    endif()
     
    list(FIND SFML_FIND_COMPONENTS "window" FIND_SFML_WINDOW_COMPONENT)
    if(NOT ${FIND_SFML_WINDOW_COMPONENT} EQUAL -1)
        if(FIND_SFML_OS_LINUX OR FIND_SFML_OS_FREEBSD)
            find_sfml_dependency(X11_LIBRARY "X11" X11)
            find_sfml_dependency(XRANDR_LIBRARY "Xrandr" Xrandr)
        endif()

        if(FIND_SFML_OS_LINUX)
            find_sfml_dependency(UDEV_LIBRARIES "UDev" udev libudev)
        endif()
         
        if(FIND_SFML_OS_WINDOWS)
            set(SFML_WINDOW_DEPENDENCIES ${SFML_WINDOW_DEPENDENCIES} "opengl32" "winmm" "gdi32")
        elseif(FIND_SFML_OS_LINUX)
            set(SFML_WINDOW_DEPENDENCIES ${SFML_WINDOW_DEPENDENCIES} "GL" ${X11_LIBRARY} ${XRANDR_LIBRARY} ${UDEV_LIBRARIES})
        elseif(FIND_SFML_OS_FREEBSD)
            set(SFML_WINDOW_DEPENDENCIES ${SFML_WINDOW_DEPENDENCIES} "GL" ${X11_LIBRARY} ${XRANDR_LIBRARY} "usbhid")
        elseif(FIND_SFML_OS_MACOSX)
            set(SFML_WINDOW_DEPENDENCIES ${SFML_WINDOW_DEPENDENCIES} "-framework OpenGL -framework Foundation -framework AppKit -framework IOKit -framework Carbon")
        endif()
        set(SFML_DEPENDENCIES ${SFML_WINDOW_DEPENDENCIES} ${SFML_DEPENDENCIES})
    endif()

    list(FIND SFML_FIND_COMPONENTS "graphics" FIND_SFML_GRAPHICS_COMPONENT)

    if(NOT ${FIND_SFML_GRAPHICS_COMPONENT} EQUAL -1)
        find_sfml_dependency(FREETYPE_LIBRARY "FreeType" freetype)
        find_sfml_dependency(JPEG_LIBRARY "libjpeg" jpeg)
        set(SFML_GRAPHICS_DEPENDENCIES ${FREETYPE_LIBRARY} ${JPEG_LIBRARY})
        set(SFML_DEPENDENCIES ${SFML_GRAPHICS_DEPENDENCIES} ${SFML_DEPENDENCIES})
    endif()

     
    list(FIND SFML_FIND_COMPONENTS "audio" FIND_SFML_AUDIO_COMPONENT)
    if(NOT ${FIND_SFML_AUDIO_COMPONENT} EQUAL -1)

         
        find_sfml_dependency(OPENAL_LIBRARY "OpenAL" openal openal32)
        find_sfml_dependency(OGG_LIBRARY "Ogg" ogg)
        find_sfml_dependency(VORBIS_LIBRARY "Vorbis" vorbis)
        find_sfml_dependency(VORBISFILE_LIBRARY "VorbisFile" vorbisfile)
        find_sfml_dependency(VORBISENC_LIBRARY "VorbisEnc" vorbisenc)
        find_sfml_dependency(FLAC_LIBRARY "FLAC" FLAC)

         
        set(SFML_AUDIO_DEPENDENCIES ${OPENAL_LIBRARY} ${FLAC_LIBRARY} ${VORBISENC_LIBRARY} ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY} ${OGG_LIBRARY})
        set(SFML_DEPENDENCIES ${SFML_DEPENDENCIES} ${SFML_AUDIO_DEPENDENCIES})
    endif()
endif()

if(NOT SFML_VERSION_OK)
    set(FIND_SFML_ERROR "SFML found but version too low (requested: ${SFML_FIND_VERSION}, found: ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}.${SFML_VERSION_PATCH})")
    set(SFML_FOUND FALSE)
elseif(SFML_STATIC_LIBRARIES AND FIND_SFML_DEPENDENCIES_NOTFOUND)
    set(FIND_SFML_ERROR "SFML found but some of its dependencies are missing (${FIND_SFML_DEPENDENCIES_NOTFOUND})")
    set(SFML_FOUND FALSE)
elseif(NOT SFML_FOUND)
    set(FIND_SFML_ERROR "Could NOT find SFML (missing: ${FIND_SFML_MISSING})")
endif()

if (NOT SFML_FOUND)
    if(SFML_FIND_REQUIRED)
        message(FATAL_ERROR ${FIND_SFML_ERROR})
    elseif(NOT SFML_FIND_QUIETLY)
        message("${FIND_SFML_ERROR}")
    endif()
endif()

if(SFML_FOUND AND NOT SFML_FIND_QUIETLY)
    message(STATUS "Found SFML ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}.${SFML_VERSION_PATCH} in ${SFML_INCLUDE_DIR}")
endif()