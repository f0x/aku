project (libaku) 

set (akucore_SRCS
    akuplugin.cpp
)

kde4_add_library(akucore SHARED ${akucore_SRCS})

target_link_libraries(akucore ${KDE4_KDEUI_LIBS})

install(TARGETS akucore ${INSTALL_TARGETS_DEFAULT_ARGS})

set (akucore_HDRS
    akuplugin.h
    aku_macros.h
)

install(FILES ${akucore_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/aku)
install(FILES aku_plugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
