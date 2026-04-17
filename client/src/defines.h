#ifndef DEFINES_H
#define DEFINES_H

#define SIDE_SIZE 8
#define BEAT_FIELD_SIZE 0.35
#define BEAT_FIELD_OPACITY 0.27
#define MINIMUM_PIECE_SIZE 25
#define FIXED_SIZE_NUMBERS 20
#define FIXED_SIZE_BUTTON 35
#define FIXED_SIZE_BUTTON_ICON 37
#define FIXED_SIZE_EXIT_BUTTON 25
#define FIXED_SIZE_TYPE_GAME 45
#define MAX_TIME_SECONDS 259200
#define TICK 100
#define MIN 60

#include <QHBoxLayout>
#include <QPixmap>
#include <QString>
#include <QVBoxLayout>

#include <vector>

enum class TypeGame : int {
    UNDEFINED = -1,
    ONLINE = 0,
    OFFLINE,
    NUMBER,
};

enum class TypeChess : int {
    UNDEFINED = -1,
    STANDART = 0,
    _960,
    USER,
    NUMBER,
};

enum class TypeTimeChess : int {
    UNDEFINED = -1,
    BULLET = 0,
    BLITZ,
    RAPID,
    CLASSIC,
    OTHER,
    NO_TIME,
    NUMBER,
};

enum class ResultGame : int {
    WIN_WHITE = 0,
    WIN_BLACK,
    STALE_MATE,
    NUMBER,
};

struct SettingsParams {
    bool hideAll{false};
    bool checkAutoQueen{false};
    bool checkAutoRotate{false};
    bool checkPremove{false};
    bool checkNoticeTime{false};
};

struct StartParams
{
    TypeGame gameType{TypeGame::UNDEFINED};
    TypeChess chessType{TypeChess::UNDEFINED};
    TypeTimeChess timeChessType{TypeTimeChess::UNDEFINED};
    qint32 mainTime{0};
    qint8 minorTime{0};
};

struct ChessBoardParams
{
    std::vector<std::vector<QString>> chessFields;
    std::pair<std::pair<bool, bool>, std::pair<bool, bool>> castling{{true, true}, {true, true}};
    bool chess960{false};
    bool whiteMove{true};
};

struct GameParams
{
    SettingsParams settingsParams;
    StartParams startParams;
    ChessBoardParams boardParams;
};

struct ChessPosParams
{
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posKings;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posRooksWhite;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posRooksBlack;
};

struct PlayerParams
{
    std::pair<QString, QString> nicknames{"", ""};
    std::pair<QPixmap, QPixmap> icons;
    std::pair<qint16, qint16> ratings{0, 0};
    bool mainPlayerWhite{true};
};

class SomeConstans
{
public:
    static SomeConstans &getInstance();

    static void swapWidgetVBox(QVBoxLayout *layout, QWidget *widget1, QWidget *widget2);
    static void swapWidgetHBox(QHBoxLayout *layout, QWidget *widget1, QWidget *widget2);

    static void fillStandartChessField(std::vector<std::vector<QString>> &chessFields);

    const QString &getConfigFile() const;
    const QString &getPathGeneral() const;
    const QString &getPathStyle1() const;
    const QString &getSmallTimeNoticeStyle() const;
    const QString &getFontSizeStyle() const;

    const std::vector<std::string> &getTypeGameStr() const;
    const std::vector<std::string> &getTypeChessStr() const;
    const std::vector<std::string> &getTypeTimeChessStr() const;
    const std::vector<std::vector<std::string>> &getTimeChessStr() const;
    const std::vector<std::vector<std::pair<qint32, qint8>>> &getTimeChessValue() const;

private:
    SomeConstans() = default;
    SomeConstans(const SomeConstans &) = delete;
    SomeConstans(const SomeConstans &&) = delete;
    SomeConstans &operator=(const SomeConstans &) = delete;
    SomeConstans &operator=(const SomeConstans &&) = delete;

    const QString m_configFile = "config.ini";
    const QString m_pathGeneral = ":/src/images/";
    const QString m_pathStyle1 = m_pathGeneral + "style1/";
    const QString m_smallTimeNoticeStyle = "background-color: red; color: black; font-size:24px; padding:10px;";
    const QString m_fontSizeStyle = "font-size: 14px;";

    const std::vector<std::string> m_typeGameStr = {
        "Online",
        "Offline",
    };

    const std::vector<std::string> m_typeChessStr = {
        "Standart",
        "960",
        "User",
    };

    const std::vector<std::string> m_typeTimeChessStr = {
        "Bullet",
        "Blitz",
        "Rapid",
        "Classic",
        "Other",
        "No time",
    };

    const std::vector<std::vector<std::string>> m_timeChessStr = {{"30s", "1m", "1m|1s", "2m", "2m|1s", "2m|2s"},
        {"3m", "3m|2s", "3m|3s", "5m", "5m|3s", "5m|5s"},
        {"10m", "10m|5s", "10m|10s", "30m", "30m|5s", "30m|10s"},
        {"1h", "1h|30s", "2h", "2h|30s", "3h", "3h|30s"}
    };

    const std::vector<std::vector<std::pair<qint32, qint8>>> m_timeChessValue
    = {{{30, 0}, {MIN, 0}, {MIN, 1}, {2 * MIN, 0}, {2 * MIN, 1}, {2 * MIN, 2}},
        {{3 * MIN, 0}, {3 * MIN, 2}, {3 * MIN, 3}, {5 * MIN, 0}, {5 * MIN, 3}, {5 * MIN, 5}},
        {{10 * MIN, 0}, {10 * MIN, 5}, {10 * MIN, 10}, {30 * MIN, 0}, {30 * MIN, 5}, {30 * MIN, 10}},
        {{60 * MIN, 0}, {60 * MIN, 30}, {120 * MIN, 0}, {120 * MIN, 30}, {180 * MIN, 0}, {180 * MIN, 30}}
    };
};

#endif // DEFINES_H
