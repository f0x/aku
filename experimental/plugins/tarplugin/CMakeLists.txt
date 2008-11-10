project(tarplugin)

set(tarplugin_SRCS
    tarplugin.cpp
)

kde4_add_plugin(aku_tarplugin ${tarplugin_SRCS})
target_link_libraries(aku_tarplugin akucore ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS aku_tarplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aku_tarplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
