#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "winutils.h"

#include <QDebug>

IShellLink* CreateShellLink(QString title, QString description,
                             QString app_path, QString app_args,
                             QString icon_path, int app_index)  {
    IShellLink* shell_link = NULL;
    IPropertyStore* prop_store = NULL;
    bool is_not_separator = (app_path.length() > 0);

    HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
                                  reinterpret_cast<void**> (&(shell_link)));

    if(SUCCEEDED(hr)) {

        if (is_not_separator) {
            shell_link->SetPath(app_path.toStdWString().c_str());
            shell_link->SetArguments(app_args.toStdWString().c_str());
            shell_link->SetIconLocation(icon_path.toStdWString().c_str(), app_index);
            shell_link->SetDescription(description.toStdWString().c_str());
        }

        hr = shell_link->QueryInterface(IID_IPropertyStore, reinterpret_cast<void**> (&(prop_store)));

        if (SUCCEEDED(hr)) {
            PROPVARIANT pv;

            if (is_not_separator) {
                hr = InitPropVariantFromString(title.toStdWString().c_str(), &pv);

                if (SUCCEEDED(hr)) {
                    hr = prop_store->SetValue(PKEY_Title, pv);
                }
            } else {
                hr = InitPropVariantFromBoolean(TRUE, &pv);

                if (SUCCEEDED(hr)) {
                    hr = prop_store->SetValue(PKEY_AppUserModel_IsDestListSeparator, pv);
                }
            }

            //Save the changes we made to the property store
            prop_store->Commit();
            prop_store->Release();

            PropVariantClear(&pv);
        }
    }

    return shell_link;
}




void AddTasksToList(ICustomDestinationList* destinationList) {
    IObjectArray* object_array;
    IObjectCollection* obj_collection;

    CoCreateInstance(CLSID_EnumerableObjectCollection, NULL,
                                  CLSCTX_INPROC, IID_IObjectCollection, reinterpret_cast<void**> (&(obj_collection)));

    obj_collection->QueryInterface(IID_IObjectArray, reinterpret_cast<void**> (&(object_array)));

    QString icons_source("C:\\windows\\explorer.exe");
    QString app_path = qApp->applicationFilePath();

    obj_collection->AddObject(CreateShellLink("Task 1", "Description Task 1",
                                               app_path, "-auto",
                                               icons_source, 20));

    obj_collection->AddObject(CreateShellLink("", "This is a separator",
                                               "", "",
                                               "", 0));

    obj_collection->AddObject(CreateShellLink("Task 2", "Description Task 2",
                                               app_path, "-auto",
                                               icons_source, 21));

    destinationList->AddUserTasks(object_array);

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
    AddTasksToList(destinationList);

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
