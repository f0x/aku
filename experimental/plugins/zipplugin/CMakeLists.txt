project(zipplugin)

include_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

set(zipplugin_SRCS
    zipplugin.cpp
)

kde4_add_ui_files(zipplugin_SRCS zipconfig.ui)

kde4_add_plugin(aku_zipplugin ${zipplugin_SRCS})
target_link_libraries(aku_zipplugin akucore ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS aku_zipplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aku_zipplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
