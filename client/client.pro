QT += core gui widgets

CONFIG += c++17

HEADERS += \
    src/defines.h \
    src/backend/chess_game.h \
    src/backend/style_lib.h \
    src/main_widgets/board_setup_window.h \
    src/main_widgets/game_window.h \
    src/main_widgets/main_window.h \
    src/main_widgets/start_game_window.h \
    src/main_widgets/styles_window.h \
    src/widgets/board_hlayout2.h \
    src/widgets/board_vlayout2.h \
    src/widgets/button_complex.h \
    src/widgets/chess_board.h \
    src/widgets/chess_board_widget.h \
    src/widgets/choose_chess.h \
    src/widgets/end_game_window.h \
    src/widgets/event_button.h \
    src/widgets/player_info_layout.h \
    src/widgets/player_vlayout2.h \
    src/widgets/player_vlayout3.h \
    src/widgets/rotatable_label.h \
    src/widgets/settings_game_window.h \
    src/widgets/settings_window.h \
    src/widgets/time_chess.h

SOURCES += \
    main.cpp \
    src/defines.cpp \
    src/backend/chess_game.cpp \
    src/backend/style_lib.cpp \
    src/main_widgets/board_setup_window.cpp \
    src/main_widgets/game_window.cpp \
    src/main_widgets/main_window.cpp \
    src/main_widgets/start_game_window.cpp \
    src/main_widgets/styles_window.cpp \
    src/widgets/button_complex.cpp \
    src/widgets/chess_board.cpp \
    src/widgets/chess_board_widget.cpp \
    src/widgets/choose_chess.cpp \
    src/widgets/end_game_window.cpp \
    src/widgets/settings_game_window.cpp \
    src/widgets/settings_window.cpp \
    src/widgets/time_chess.cpp

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
