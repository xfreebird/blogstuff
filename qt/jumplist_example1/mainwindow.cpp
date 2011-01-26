#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "winutils.h"

#include <QDebug>

void SetupJumpList() {
    UINT max_count = 0;
    IObjectArray* objectArray;
    ICustomDestinationList* destinationList;

    //create the custom jump list object
    CoCreateInstance(CLSID_DestinationList, NULL, CLSCTX_INPROC_SERVER, IID_ICustomDestinationList,
                                              reinterpret_cast<void**> (&(destinationList)));

    //initialize list
    destinationList->BeginList(&max_count, IID_IObjectArray, reinterpret_cast<void**> (&(objectArray)));
    destinationList->AppendKnownCategory(KDC_FREQUENT);
    destinationList->AppendKnownCategory(KDC_RECENT);

    //commit list
    destinationList->CommitList();
    objectArray->Release();
    destinationList->Release();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (qApp->arguments().size() > 1) {
        SHAddToRecentDocs(0x00000003, qApp->arguments().at(1).toStdWString().c_str());
    }

    SetupJumpList();
}

MainWindow::~MainWindow()
{
    delete ui;
}
