#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QChar>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>

#include <unordered_map>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // --- Constructors/destructors ---

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    // --- Helper methods ---

    void clickField(const QString &nameField);
    void takePiece(int i, int j);
    void untakePiece();
    bool checkBeat(int i, int j);
    void checkField(int i, int j);
    void uncheckField(int i, int j);
    void fillMap();
    void fillStandartChessBoard();
    void fillChessScene();

private:

    // --- Fields ---

    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<QString>> m_chessBoard;
    std::vector<std::vector<QPushButton *>> m_chessBoardLabels;

    std::pair<int, int> m_takenPiece;
    std::vector<std::pair<int, int>> m_beatField;
    std::pair<std::pair<int, int>, std::pair<int, int>> m_lastBeat;

    bool m_whiteMove{true};
};

#endif // MAINWINDOW_H
