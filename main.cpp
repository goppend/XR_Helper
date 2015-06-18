#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir pathToGamedata;
    while(!pathToGamedata.exists("gamedata"))
    {
        QString path =  QFileDialog::getExistingDirectory(NULL, "Open Directory", QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(path.isEmpty())
            return -1;
        pathToGamedata = QDir(path);
    }

    MainWindow w(pathToGamedata,NULL);
    w.show();
	return a.exec();
}
