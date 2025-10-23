#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShowEvent>
#include <QLabel>
#include <QPixmap>
#include <QString>

#include <unordered_map>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // --- Constructors/destructors ---

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void showEvent(QShowEvent *event) override;

private:

    // --- Helper methods ---

    void fillMap();
    void fillStandartChessBoard();
    void fillChessScene();

private:

    // --- Fields ---

    Ui::MainWindow *ui;

    std::unordered_map<QString, QPixmap> m_imagesOfPieces;
    std::vector<std::vector<QString>> m_chessBoard;
    std::vector<std::vector<QLabel*>> m_chessBoardLabels;
};
#endif // MAINWINDOW_H
