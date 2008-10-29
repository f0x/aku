include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

set( aku_SRCS
     main.cpp
     mainwindow.cpp
     openarchive.cpp 
     maintree.cpp
     pluginview.cpp
     ../base/pluginloader.cpp
     akutreemodel.cpp
     akutreenode.cpp
 )

kde4_add_executable(aku ${aku_SRCS})
target_link_libraries(aku akucore ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
install(TARGETS aku DESTINATION ${BIN_INSTALL_DIR})
install(FILES akuui.rc 
        DESTINATION ${DATA_INSTALL_DIR}/aku)