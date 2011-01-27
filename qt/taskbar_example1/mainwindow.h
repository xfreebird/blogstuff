#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "winutils.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetOverlayIcon(const QIcon& icon, QString description);

protected:
    bool winEvent(MSG * message, long * result);

private:
    void createConnections();

    void initTaskbarButton();
    unsigned int m_IDTaskbarButtonCreated;
    ITaskbarList3* m_taskbarInterface;
    Ui::MainWindow *ui;

private slots:
    void updatePorgressBarState(int state);
    void updateProgressBarValue(int value);
    void updateOverlayIcon(int index);
};

#endif // MAINWINDOW_H
