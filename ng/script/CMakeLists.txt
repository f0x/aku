set( script_SRCS
    main.cpp
    akuscriptwidget.cpp
    scriptobject.cpp
)

kde4_add_executable(akuscriptshell ${script_SRCS})
target_link_libraries(akuscriptshell ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${QT_QTSCRIPT_LIBRARY})
install(TARGETS akuscriptshell ${INSTALL_TARGETS_DEFAULT_ARGS})
