
set( aku_SRCS
     main.cpp
     mainwindow.cpp
     openarchive.cpp )

kde4_add_executable(aku ${aku_SRCS})
target_link_libraries(aku ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
install(TARGETS aku DESTINATION ${BIN_INSTALL_DIR})
install(FILES akuui.rc 
        DESTINATION ${DATA_INSTALL_DIR}/aku)