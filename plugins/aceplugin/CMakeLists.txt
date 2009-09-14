project(aceplugin)

set(aceplugin_SRCS
    aceplugin.cpp
)

kde4_add_plugin(aku_aceplugin ${aceplugin_SRCS})
target_link_libraries(aku_aceplugin akucore ${KDE4_KDECORE_LIBS})

install(TARGETS aku_aceplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aku_aceplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR}) 
