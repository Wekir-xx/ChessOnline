QT += core gui widgets

include(src/LayoutCustom/LayoutCustom.pri)

CONFIG += c++17

SOURCES += \
    chess_board.cpp \
    chess_game.cpp \
    game_window.cpp \
    main.cpp

HEADERS += \
    chess_board.h \
    chess_game.h \
    defines.h \
    game_window.h

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
