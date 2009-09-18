project(szipplugin)

set(7zipplugin_SRCS
    7zipplugin.cpp
)

kde4_add_plugin(aku_szipplugin ${szipplugin_SRCS})
target_link_libraries(aku_7zipplugin akucore ${KDE4_KDECORE_LIBS})

install(TARGETS aku_7zipplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aku_7zipplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR}) 
