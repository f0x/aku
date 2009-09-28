find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set( aku_SRCS
     main.cpp
     mainwindow.cpp
     ../base/pluginloader.cpp
     infodialog.cpp
     akutreeview.cpp
     akutreenode.cpp
     akutreemodel.cpp
     pluginsmodel.cpp
     filterwidget.cpp
     sortfiltermodel.cpp
     metawidget.cpp
     passwidget.cpp
     extractiondialog.cpp
     commentwidget.cpp
     statusbar.cpp
   )

kde4_add_ui_files(aku_SRCS extractiondialog.ui)

kde4_add_executable(aku ${aku_SRCS})
target_link_libraries(aku akucore ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KUTILS_LIBS})
install(TARGETS aku ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES akuui.rc 
        DESTINATION  ${DATA_INSTALL_DIR}/aku)
