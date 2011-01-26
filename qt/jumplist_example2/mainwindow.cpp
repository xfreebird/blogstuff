#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "winutils.h"

#include <QDebug>

IShellItem* FilePath2ShellItem(QString path) {
    HMODULE shell;
    IShellItem *shell_item = NULL;
    t_SHCreateItemFromParsingName SHCreateItemFromParsingName = NULL;

    shell = LoadLibrary(L"shell32.dll");

    if (shell) {
        SHCreateItemFromParsingName = reinterpret_cast<t_SHCreateItemFromParsingName>
                                      (GetProcAddress (shell, "SHCreateItemFromParsingName"));

        if (SHCreateItemFromParsingName != NULL) {
            SHCreateItemFromParsingName(path.toStdWString().c_str(), NULL, IID_IShellItem,
                                        reinterpret_cast<void**> (&(shell_item)));
        }

        FreeLibrary (shell);
    }

    return shell_item;
}


void AddCategoryToList(QString title, ICustomDestinationList* destinationList) {
    IObjectArray* object_array;
    IObjectCollection* obj_collection;

    CoCreateInstance(CLSID_EnumerableObjectCollection, NULL,
                                  CLSCTX_INPROC, IID_IObjectCollection, reinterpret_cast<void**> (&(obj_collection)));

    obj_collection->QueryInterface(IID_IObjectArray, reinterpret_cast<void**> (&(object_array)));


    //make sure that the file path is valid and the file extension is registered to this application
    //otherwise it will crash the application
    obj_collection->AddObject(FilePath2ShellItem("D:\\file1.rgm"));
    obj_collection->AddObject(FilePath2ShellItem("D:\\file2.rgm"));

    destinationList->AppendCategory(title.toStdWString().c_str(), object_array);

    object_array->Release();
    obj_collection->Release();
}



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
    AddCategoryToList("Custom entry", destinationList);

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

    SetupJumpList();
}

MainWindow::~MainWindow()
{
    delete ui;
}
