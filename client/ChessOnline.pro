QT += core gui widgets

CONFIG += c++17

SOURCES += \
    chess_board.cpp \
    chess_game.cpp \
    game_window.cpp \
    main.cpp \
    main_window.cpp \
    src/button_complex.cpp \
    src/end_game_window.cpp \
    src/settings_window.cpp \
    src/time_chess.cpp \
    start_game_window.cpp

HEADERS += \
    chess_board.h \
    chess_game.h \
    game_window.h \
    main_window.h \
    src/button_complex.h \
    src/defines.h \
    src/board_layout.h \
    src/end_game_window.h \
    src/event_button.h \
    src/player_info_layout2.h \
    src/player_info_layout3.h \
    src/rotatable_label.h \
    src/settings_window.h \
    src/time_chess.h \
    start_game_window.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
