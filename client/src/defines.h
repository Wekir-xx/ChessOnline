#ifndef DEFINES_H
#define DEFINES_H

#define EMPTY -1
#define BEAT_FIELD_SIZE 0.35
#define BEAT_FIELD_OPACITY 0.27
#define MINIMUM_PIECE_SIZE 25
#define FIXED_SIZE_NUMBERS 20
#define FIXED_SIZE_EXIT_BUTTON 25
#define FIXED_SIZE_TYPE_GAME 45

#include <QString>

const QString pathGeneral = ":/src/images/";
const QString pathStyle1 = pathGeneral + "style1/";

enum class TypeChess {
    STANDART,
    STANDART960,
    USER,
    USER960,
};

enum class TypeTimeChess {
    BULLET,
    BLITZ,
    RAPID,
    CLASSIC,
    OTHER,
};

enum class TypeGame {
    ONLINE,
    OFFLINE,
};

enum class ResultGame {
    WIN_WHITE,
    WIN_BLACK,
    STALE_MATE,
};

#endif // DEFINES_H
