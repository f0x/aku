project(szipplugin)

set(szipplugin_SRCS
    szipplugin.cpp
)

kde4_add_plugin(aku_szipplugin ${szipplugin_SRCS})
target_link_libraries(aku_szipplugin akucore ${KDE4_KDECORE_LIBS})

install(TARGETS aku_szipplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aku_szipplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR}) 
