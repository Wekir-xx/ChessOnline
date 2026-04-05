QT += core gui widgets

CONFIG += c++17

SOURCES += \
    chess_board.cpp \
    chess_game.cpp \
    game_window.cpp \
    main.cpp \
    main_window.cpp \
    src/end_game_window.cpp \
    src/settings_window.cpp

HEADERS += \
    chess_board.h \
    chess_game.h \
    game_window.h \
    main_window.h \
    src/defines.h \
    src/board_layout.h \
    src/end_game_window.h \
    src/event_button.h \
    src/player_info_layout.h \
    src/settings_window.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
