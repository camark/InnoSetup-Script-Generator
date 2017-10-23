#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_actionQuit_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

	void on_find_exe_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *tableModel;



    void SetupTabview()
    {
         tableModel = new QStandardItemModel();

         //设置行
         tableModel->setColumnCount(2);
         tableModel->setHeaderData(0,Qt::Horizontal,tr("File"));
         tableModel->setHeaderData(1,Qt::Horizontal,tr("Is Register?"));

         //tableModel->sort(0,Qt::DescendingOrder);
         tableModel->setSortRole(1);

         //设置每行大小


    }

    char* stoc(QString str){
        return str.toLatin1().data();
    }

    QString getFileName(const QString& filePath){
        QFileInfo fi(filePath);
        return fi.fileName();
    }

    QString getFilePath(const QString& fileName){
        QFileInfo fi(fileName);
        return fi.absolutePath();
    }


    QString getFileXiangduiPath(const QString& fileName,const QString& basePath){

        int iPos = fileName.indexOf(basePath);
        if(iPos==-1)
            return QString("");

        int Pos = basePath.length();

        if(Pos>0)
            return fileName.mid(Pos+1);
        else
            return QString("");
    }

    QString getCurDay(){
        QDateTime now;
        now = QDateTime::currentDateTime();

        QString str = now.toString("yyyyMMdd"); //设置显示格式

        return str;
    }
};

#endif // MAINWINDOW_H
