project(rarplugin)

set(rarplugin_SRCS
    rarplugin.cpp
)

kde4_add_plugin(aku_rarplugin ${rarplugin_SRCS})
target_link_libraries(aku_rarplugin akucore ${KDE4_KDECORE_LIBS})

install(TARGETS aku_rarplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aku_rarplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR}) 
