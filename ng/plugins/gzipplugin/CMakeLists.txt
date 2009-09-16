project(gzipplugin)

set(gzipplugin_SRCS
    gzipplugin.cpp
)

kde4_add_plugin(aku_gzipplugin ${gzipplugin_SRCS})
target_link_libraries(aku_gzipplugin akucore ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS aku_gzipplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aku_gzipplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
