project(aku)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

add_subdirectory(app)
add_subdirectory(base)
