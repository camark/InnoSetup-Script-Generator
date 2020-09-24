#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <qfile.h>
#include <QIODevice>
#include <QDir>
#include <QMessageBox>
#include <QtPlugin>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetupTabview();

    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnWidth(0,450);
    ui->tableView->setColumnWidth(1,180);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->verticalHeader()->hide();

    ui->le_Version->setText(getCurDay());

    setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tableModel;
}

void MainWindow::on_pushButton_2_clicked()
{
        QFileDialog::Options options;
        QString openFilesPath;
        QString selectedFilter;
        QStringList files = QFileDialog::getOpenFileNames(
                                    this, tr("QFileDialog::getOpenFileNames()"),
                                    openFilesPath,
                                    tr("All Files (*);;Text Files (*.txt)"),
                                    &selectedFilter,
                                    options);
        if (files.count()) {
             for(QStringList::Iterator it=files.begin();it!=files.end();it++)
             {
                 QString strFileName = *it;
                 QStandardItem* item1 = new QStandardItem(strFileName);
                 QStandardItem* item2 = new QStandardItem(tr("0"));
                 QList<QStandardItem*> item;
                 item << item1 << item2;
                 tableModel->appendRow(item);
             }
        }

}

void MainWindow::on_pushButton_clicked()
{
    int index = ui->tableView->currentIndex().row();
    tableModel->removeRow(index);
}

void MainWindow::on_pushButton_4_clicked()
{
    QString exeFile;

    exeFile = QFileDialog::getOpenFileName(this,"Select Exe File");

    if(!exeFile.isEmpty()){
        ui->le_ExeFile->setText(exeFile);

        QFileInfo fi(exeFile);

        QString file_name=fi.fileName();
        QString app_name = file_name.mid(0,file_name.indexOf("."));

        ui->le_AppName->setText(app_name);
        ui->le_InstallDir->setText(app_name);
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString setupSection(tr("[Setup]"));
    QString filesSection(tr("[Files]"));
    QString iconSection(tr("[Icons]"));

    QStringList lines;

    //Setup Section
    lines.append(setupSection);

    QString appName;
    appName.append("AppName=");
    appName.append(ui->le_AppName->text());
    lines.append(appName);
    QString appVersion;
    appVersion.append("AppVersion=");
    appVersion.append(ui->le_Version->text());
    lines.append(appVersion);
    QString defaultInstallDir;
    //defaultInstallDir.append("DefaultDirName={pf}\\emz\\");
    defaultInstallDir.append("DefaultDirName={userappdata}\\emz\\");
    defaultInstallDir.append(ui->le_InstallDir->text());
    lines.append(defaultInstallDir);
    lines.append(tr("DisableProgramGroupPage=yes"));
    QString exeFileName;
    exeFileName.append("UninstallDisplayIcon={app}"
                       "\\");
    exeFileName.append(getFileName(ui->le_ExeFile->text()));

    lines.append(exeFileName);
    lines.append(tr("OutputDir=setup"));

    //File Section
    lines.append(filesSection);
    int rowCount = tableModel->rowCount();
    QModelIndex index_begin = tableModel->index(0,0);
    QString real_fileName = tableModel->data(index_begin).toString();

    QString basePath = getFilePath(real_fileName);
    bool isSubDir=false;

    for(int i=0;i<rowCount;i++){
        QModelIndex index0 = tableModel->index(i,0);
        QString real_fileName = tableModel->data(index0).toString();

        QString fileName = getFileXiangduiPath(real_fileName, basePath);
        isSubDir = fileName.indexOf("/")>0;

        QModelIndex index1 = tableModel->index(i,1);
        QString isRegister = tableModel->data(index1).toString();

        QString strLine;
        fileName.replace("/","\\");
        if(isRegister=="0"){

            if(!isSubDir)
                strLine = QString("Source: \"%1\"; DestDir: \"{app}\";Flags:ignoreversion").arg(fileName);
            else{
                QString subdir = fileName.mid(0,fileName.lastIndexOf(QDir::separator()));

                //QFileInfo fi(subdir);

                //subdir=fi.absolutePath().mid(subdir.lastIndexOf("/"));
                subdir.replace("/",QDir::separator());

                strLine = QString("Source: \"%1\"; DestDir: \"{app}\\%2\\\";Flags:ignoreversion").arg(fileName,subdir);
            }
        }
        else
        {
            if(!isSubDir)
                 strLine = QString("Source: \"%1\"; DestDir: \"{app}\"; CopyMode: alwaysskipifsameorolder; Flags: regserver").arg(fileName);
            else
            {                
                QString subdir = fileName.mid(0,fileName.lastIndexOf(QDir::separator()));
                subdir.replace("/",QDir::separator());

                strLine = QString("Source: \"%1\"; DestDir: \"{app}\\%2\\\"; CopyMode: alwaysskipifsameorolder; Flags: regserver").arg(fileName,subdir);
            }

        }


        lines.append(strLine);
    }

    //Icon Section
    lines.append(iconSection);
    QString desktopShortCut;
    desktopShortCut = QString("Name: \"{commondesktop}\\%1\"; Filename: \"{app}\\%2\"").arg(ui->le_AppName->text(),getFileName(ui->le_ExeFile->text()));
    lines.append(desktopShortCut);

    QString exeFile;
    exeFile = QFileDialog::getSaveFileName(this,"Select generated File:");
    if(!exeFile.isEmpty()){

        if(!exeFile.endsWith(".iss"))
        {
            exeFile.append(".iss");
        }

        QFile file(exeFile);

        if ( file.open(QIODevice::WriteOnly) ) {
               QTextStream stream( &file );
               for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
                   stream << *it << "\n";
               file.close();

               QMessageBox::information(this,"Congralution","Create inno setup script success!",QMessageBox::Ok);
           }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    int row= ui->tableView->currentIndex().row();
    QAbstractItemModel *model = ui->tableView->model ();
    QModelIndex index = model->index(row,0);//选中行第一列的内容
    QVariant data = model->data(index);

    QString exeFile=data.toString();
    ui->le_ExeFile->setText(exeFile);

    QFileInfo fi(exeFile);

    QString file_name=fi.fileName();
    QString app_name = file_name.mid(0,file_name.indexOf("."));

    ui->le_AppName->setText(app_name);
    ui->le_InstallDir->setText(app_name);
}

void MainWindow::on_find_exe_clicked()
{
	QAbstractItemModel *model = ui->tableView->model();

    bool bFind=false;

	for (int i = 0;i < model->rowCount();i++) {
		QVariant data = model->data(model->index(i, 0));

		QString fileName = data.toString();

		if (fileName.endsWith(".exe")) {
            QString yes_file_name=QString("是否将 %1 设置为主程序Exe文件").arg(fileName);
            //qDebug() << fileName;
            QMessageBox::StandardButton sb=QMessageBox::question(this,"确认",yes_file_name,QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
            if(sb==QMessageBox::Yes){
                ui->le_ExeFile->setText(fileName);
                bFind=true;
                return;
            }
		}
	}

    if(!bFind){
        QMessageBox::information(this,"警告","无法找到或者设置Exe文件",QMessageBox::Yes);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    on_find_exe_clicked();
}

void MainWindow::PopulateDir(QString selDir)
{
    QDir dir(selDir);

    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.*";
    //将过滤后的文件名称存入到files列表中
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    if (files.count()) {
         for(QStringList::Iterator it=files.begin();it!=files.end();it++)
         {
             QString strFileName = *it;

             QFileInfo fi(selDir+"/"+strFileName);

             if(fi.isFile()){
                 QStandardItem* item1 = new QStandardItem(selDir+"/"+strFileName);
                 QStandardItem* item2 = new QStandardItem(tr("0"));
                 QList<QStandardItem*> item;
                 item << item1 << item2;
                 tableModel->appendRow(item);
             }
         }
    }

    QStringList dirs = dir.entryList(nameFilters, QDir::AllDirs|QDir::Readable|QDir::NoDotAndDotDot, QDir::Name);
    if(dirs.count())
    {
        for(QStringList::Iterator it=dirs.begin();it!=dirs.end();it++)
        {
            QString dirname=*it;

            QString target_idr=selDir+"/"+dirname;

            PopulateDir(target_idr);
        }
    }

}
void MainWindow::on_pushButton_7_clicked()
{
    QString selDir = QFileDialog::getExistingDirectory(this,"Select dir");

    if (selDir.isEmpty())
    {
       return;
    }

    PopulateDir(selDir);

}
