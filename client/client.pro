QT += core gui widgets

CONFIG += c++17

SOURCES += \
    board_setup_window.cpp \
    main_window.cpp \
    game_window.cpp \
    start_game_window.cpp \
    src/settings_game_window.cpp \
    src/settings_window.cpp \
    src/style_lib.cpp \
    src/chess_board.cpp \
    src/chess_game.cpp \
    src/chess_board_widget.cpp \
    src/choose_chess.cpp \
    src/end_game_window.cpp \
    src/defines.cpp \
    src/button_complex.cpp \
    src/time_chess.cpp \
    main.cpp \
    styles_window.cpp

HEADERS += \
    board_setup_window.h \
    main_window.h \
    game_window.h \
    start_game_window.h \
    src/board_vlayout2.h \
    src/settings_game_window.h \
    src/settings_window.h \
    src/style_lib.h \
    src/chess_board.h \
    src/chess_game.h \
    src/board_hlayout2.h \
    src/chess_board_widget.h \
    src/choose_chess.h \
    src/player_vlayout2.h \
    src/player_vlayout3.h \
    src/end_game_window.h \
    src/defines.h \
    src/button_complex.h \
    src/time_chess.h \
    src/event_button.h \
    src/rotatable_label.h \
    styles_window.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
