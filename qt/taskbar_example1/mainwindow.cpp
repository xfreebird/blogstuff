#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "winutils.h"

#include <QDebug>


void MainWindow::updatePorgressBarState(int state) {
    if (m_taskbarInterface) {
        switch(state) {
        case 1:
            m_taskbarInterface->SetProgressState(winId(), TBPF_INDETERMINATE);
            break;

        case 2:
            m_taskbarInterface->SetProgressState(winId(), TBPF_NORMAL);
            ui->progressValue->setValue(1);
            ui->progressValue->setValue(0);
            break;

        case 3:
            m_taskbarInterface->SetProgressState(winId(), TBPF_ERROR);
            break;

        case 4:
            m_taskbarInterface->SetProgressState(winId(), TBPF_PAUSED);
            break;

        default:
            m_taskbarInterface->SetProgressState(winId(), TBPF_NOPROGRESS);
            ui->progressValue->setValue(1);
            ui->progressValue->setValue(0);
            break;
        }
    }

}

void MainWindow::updateProgressBarValue(int value) {
    if (m_taskbarInterface) {
        if (ui->progressState->currentIndex() > 1) {
            m_taskbarInterface->SetProgressValue(winId(), value, 99);
        }
    }
}

void MainWindow::updateOverlayIcon(int index) {
    if (m_taskbarInterface) {
        QString icon_res(":/icon_" + QVariant(index).toString());

        // a NULL icon will reset the current overlay icon
        SetOverlayIcon(index > 0 ? QIcon(icon_res) : QIcon(), "Description");
    }
}

void MainWindow::createConnections() {
    connect(ui->progressState, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePorgressBarState(int)));
    connect(ui->progressValue, SIGNAL(valueChanged(int)), this, SLOT(updateProgressBarValue(int)));
    connect(ui->iconOption, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOverlayIcon(int)));
}

void MainWindow::SetOverlayIcon(const QIcon& icon, QString description) {
    if (m_taskbarInterface) {

        HICON overlay_icon = icon.isNull() ? NULL : icon.pixmap(48).toWinHICON();
        m_taskbarInterface->SetOverlayIcon(winId(), overlay_icon, description.toStdWString().c_str());

        if (overlay_icon) {
            DestroyIcon(overlay_icon);
            return;
        }
    }

    return;
}


//init the taskbar button
void MainWindow::initTaskbarButton() {
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<void**> (&(m_taskbarInterface)));

    if (SUCCEEDED(hr)) {
        hr = m_taskbarInterface->HrInit();

        if (FAILED(hr)) {
            m_taskbarInterface->Release();
            m_taskbarInterface = NULL;
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createConnections();

    m_taskbarInterface = NULL;

    // Compute the value for the TaskbarButtonCreated message
    m_IDTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");
}

bool MainWindow::winEvent(MSG * message, long * result)
{
    if (message->message == m_IDTaskbarButtonCreated) {

        initTaskbarButton();

    }

    return false;
}

MainWindow::~MainWindow()
{
    if (m_taskbarInterface) {
        m_taskbarInterface->Release();
    }

    delete ui;
}
