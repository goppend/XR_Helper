#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QCheckBox>
#include <QDataStream>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QGraphicsWidget>
#include <QHeaderView>
#include <QImage>
#include <QIntValidator>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPixmap>
#include <QScrollArea>
#include <QSettings>
#include <QSignalMapper>
#include <QSplitter>
#include <QString>
#include <QTableWidget>
#include <QTreeWidget>
#include "CEntryLTX.h"
#include "CFileDDS.h"
#include <math.h>
#include "VArrowLineEdit.h"
#include "VLogger.h"

class attBox : public QWidget
{
    Q_OBJECT

public:
    QHBoxLayout* layout;
    QCheckBox* boxes[3];
    attBox(QStringList text, QWidget *parent = 0);

signals:
    void scopeStateChanged(bool enabled);
    void silencerStateChanged(bool enabled);
    void launcherStateChanged(bool enabled);

protected slots:
    void checkBoxStateChanged(int);

private:
    QSignalMapper* signalMapper;
};

class QLTXTreeItem : public QTreeWidgetItem
{
public:
	CFileLTXLineContainer *container;
	QLTXTreeItem(QTreeWidget* parent, QStringList stringlist);
	~QLTXTreeItem();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    bool scopeEnabled;
    bool silEnabled;
    bool launchEnabled;
    QDir pathToGamedata;
    QGraphicsWidget* gWidget;
	QGridLayout* subLayout;
    QHBoxLayout* lAttach;
    QImage* allItems;
    QImage itemImage, scopeImage, silImage, launchImage;
    QLabel* label;
    QMap<QString, QLTXTreeItem*>  nameMap;
    QPixmap pixmap;
    QPainter* painter;
    QScrollArea* keyScroll, *imageScroll;
    QSplitter* mainSplitter;
	QTableWidget* keyTable;
	QTableWidgetItem* keyItem, *valueItem;
    QVector<CFileLTX* > ltxVector;
    struct wefInfo
    {
        QString name;
        int x, y, w, h;

        struct attInfo
        {
            QString name;
            int x,y,w,h,attx,atty;
            bool status;
        };
        attInfo scopeInfo, silInfo, launchInfo;
    };
    wefInfo itemInfo;

public:
    CFileDDS* ui_icon_equipment;
    CEntryLTX* ltx;
    QTreeWidget* LtxTree;
    VLogger* vlog;
    attBox* box;
    void update_pix(int x, int y, int w, int h);
    void update_pix();
	QList<QString>* loadEntry(QLTXTreeItem* ltxItem);
    MainWindow(QDir path, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void gamedataSelect(void);
    void reloadLTX(void);
    void exit(void);
    void scopeStateChanged(bool enabled);
    void silencerStateChanged(bool enabled);
    void launcherStateChanged(bool enabled);
    void keyCellChanged(int row, int column);
	void ltxTreeItemChanged( QTreeWidgetItem* item, int column);
};

#endif // MAINWINDOW_H
