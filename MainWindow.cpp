#include "MainWindow.h"

MainWindow::MainWindow(QDir path, QWidget *parent) : QMainWindow(parent)
{
	ltx = NULL;
    vlog = new VLogger(NULL);
    pathToGamedata = path;
    std::clog << "gamedata path set: " << pathToGamedata.absolutePath().toLatin1().data() << std::endl;

    ui_icon_equipment = new CFileDDS(QString(pathToGamedata.absolutePath() + "/gamedata/textures/ui/ui_icon_equipment.dds"));
    allItems = ui_icon_equipment->uncompress();

    QMenuBar* mainMenuBar = this->menuBar();
    QMenu* fileMenu = new QMenu("File", mainMenuBar);
    QAction* pathAct = new QAction("New gamedata path", fileMenu);
    QAction* reloadAct = new QAction("Reload content", fileMenu);
    QAction* exitAct = new QAction("Exit", fileMenu);
    mainMenuBar->addMenu(fileMenu);
    fileMenu->addAction(pathAct);
    fileMenu->addAction(reloadAct);
    fileMenu->addAction(exitAct);
    connect(pathAct  , SIGNAL(triggered()), this, SLOT(gamedataSelect(void)));
    connect(reloadAct, SIGNAL(triggered()), this, SLOT(reloadLTX(void)));
    connect(exitAct  , SIGNAL(triggered()), this, SLOT(exit(void)));

    label = new QLabel;
    gWidget = new QGraphicsWidget;
	imageScroll = new QScrollArea;
	imageScroll->setMinimumSize(300 + imageScroll->verticalScrollBar()->width(), 150 + imageScroll->horizontalScrollBar()->height());
	imageScroll->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	imageScroll->setAlignment(Qt::AlignCenter);
    imageScroll->setWidget( label );

    setWindowIcon(QIcon(":/icons/icons/icon.png"));

	keyTable = new QTableWidget;
    connect( keyTable , SIGNAL( cellChanged( int, int ) ), this, SLOT( keyCellChanged( int, int ) ) );

    box = new attBox(QStringList());
    connect(box, SIGNAL(scopeStateChanged(bool))   , this, SLOT(scopeStateChanged(bool)));
    connect(box, SIGNAL(silencerStateChanged(bool)), this, SLOT(silencerStateChanged(bool)));
    connect(box, SIGNAL(launcherStateChanged(bool)), this, SLOT(launcherStateChanged(bool)));

    LtxTree = new QTreeWidget();
    LtxTree->setColumnCount(1);
    LtxTree->setHeaderLabel("Name");
    connect( LtxTree, SIGNAL( itemClicked( QTreeWidgetItem*, int ) ), this, SLOT( ltxTreeItemChanged( QTreeWidgetItem*, int ) ) );
    reloadLTX();
    itemInfo.x = 5;
    itemInfo.y = 2;
    itemInfo.w = 1;
    itemInfo.h = 2;

	subLayout = new QGridLayout;
	QWidget* subWidget = new QWidget( NULL );
    subLayout->addWidget(keyTable   , 0, 0, 2, 1);
    subLayout->addWidget(imageScroll, 0, 1, 1, 1);
    subLayout->addWidget(box        , 1, 1, 1, 1);
	subWidget->setLayout(subLayout);
    update_pix();

	mainSplitter = new QSplitter(Qt::Vertical);
	mainSplitter->addWidget(subWidget);
	mainSplitter->addWidget(vlog);

	QHBoxLayout* mainLayout = new QHBoxLayout;
	QWidget* mainWidget = new QWidget;
	LtxTree->setMaximumWidth(200);
	mainLayout->addWidget(LtxTree);
	mainLayout->addWidget(mainSplitter);
	mainWidget->setLayout(mainLayout);

	this->setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
    label->deleteLater();
}

void MainWindow::update_pix()
{
    itemImage = allItems->copy(itemInfo.x*50, itemInfo.y*50, itemInfo.w*50, itemInfo.h*50);
    QImage itemPainter = itemImage;
    painter = new QPainter( &itemPainter );
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->drawImage(0, 0, itemImage );
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    if(scopeEnabled)
    {
        //painter->begin( &itemImage );
        scopeImage = allItems->copy(itemInfo.scopeInfo.x*50, itemInfo.scopeInfo.y*50, itemInfo.scopeInfo.w*50, itemInfo.scopeInfo.h*50);
        painter->drawImage(itemInfo.scopeInfo.attx, itemInfo.scopeInfo.atty, scopeImage);
        //painter->end();
    }
    if(silEnabled)
    {
        //painter->begin( &itemImage );
        silImage = allItems->copy(itemInfo.silInfo.x*50, itemInfo.silInfo.y*50, itemInfo.silInfo.w*50, itemInfo.silInfo.h*50);
        painter->drawImage(itemInfo.silInfo.attx, itemInfo.silInfo.atty, silImage);
        //painter->end();
    }
    if(launchEnabled)
    {
        //painter->begin( &itemImage );
        launchImage = allItems->copy(itemInfo.launchInfo.x*50, itemInfo.launchInfo.y*50, itemInfo.launchInfo.w*50, itemInfo.launchInfo.h*50);
        painter->drawImage(itemInfo.launchInfo.attx, itemInfo.launchInfo.atty, launchImage);
        //painter->end();
    }
    painter->end();
    delete painter;
    pixmap = QPixmap::fromImage(itemPainter);
    label->setPixmap( pixmap );
    label->setFixedSize(itemImage.width(), itemImage.height());
}

void MainWindow::ltxTreeItemChanged(QTreeWidgetItem *item, int column)
{
    if(column == -1)
        return;
    keyTable->blockSignals(true);
	QLTXTreeItem *ltxItem = (QLTXTreeItem*) item;
	CFileLTXEntry *entry = ltxItem->container->entry;

	QList<QString>* keyList = loadEntry(ltxItem);

	if(keyTable->rowCount() + keyTable->columnCount() > 0)
		keyTable->clear();

	keyTable->setRowCount(keyList->size()/2);
	keyTable->setColumnCount(2);
	for(int index = 0; index < keyList->size()/2; index++)
	{
        keyItem = new QTableWidgetItem(keyList->at(index*2));
		valueItem = new QTableWidgetItem(keyList->at(index*2+1));
        keyItem->setFlags(keyItem->flags() & ~Qt::ItemIsEditable);
		keyTable->setItem(index, 0, keyItem);
		keyTable->setItem(index, 1, valueItem);
	}
	keyTable->horizontalHeader()->setStretchLastSection(true);
	keyTable->horizontalHeader()->hide();
    keyTable->verticalHeader()->hide();
    subLayout->addWidget(keyTable, 0, 0, 2, 1);

    scopeEnabled = silEnabled = launchEnabled = false;
    itemInfo.x = entry->value( "inv_grid_x", "-1" ).toInt();
    itemInfo.y = entry->value( "inv_grid_y", "-1" ).toInt();
    itemInfo.h = entry->value( "inv_grid_height", "1" ).toInt();
    itemInfo.w = entry->value( "inv_grid_width", "1" ).toInt();
    int scopeStatus = entry->value("scope_status", "0").toInt();
    int silStatus = entry->value("silencer_status", "0").toInt();
    int launchStatus = entry->value("grenade_launcher_status", "0").toInt();
    if(scopeStatus == 2)
    {
        std::clog << "Loading scope for " << entry->lines.at(0)->value.toLatin1().data() << std::endl;
        itemInfo.scopeInfo.name = entry->value("scope_name", "");
        itemInfo.scopeInfo.attx = entry->value("scope_x", "0").toInt();
        itemInfo.scopeInfo.atty = entry->value("scope_y", "0").toInt();
        if(itemInfo.scopeInfo.name.isEmpty() || !(nameMap.value(itemInfo.scopeInfo.name)))
            std::cerr << "scope " << itemInfo.scopeInfo.name.toLatin1().data() << " not found." << std::endl;
        else
        {
            itemInfo.scopeInfo.x = nameMap.value(itemInfo.scopeInfo.name)->container->entry->value("inv_grid_x", "-1").toInt();
            itemInfo.scopeInfo.y = nameMap.value(itemInfo.scopeInfo.name)->container->entry->value("inv_grid_y", "-1").toInt();
            itemInfo.scopeInfo.w = nameMap.value(itemInfo.scopeInfo.name)->container->entry->value("inv_grid_width", "-1").toInt();
            itemInfo.scopeInfo.h = nameMap.value(itemInfo.scopeInfo.name)->container->entry->value("inv_grid_height", "-1").toInt();
        }
    }
    if(silStatus == 2)
    {
        std::clog << "Loading silencer for " << entry->lines.at(0)->value.toLatin1().data() << std::endl;
        itemInfo.silInfo.name = entry->value("silencer_name", "");
        itemInfo.silInfo.attx = entry->value("silencer_x", "0").toInt();
        itemInfo.silInfo.atty = entry->value("silencer_y", "0").toInt();
        if(itemInfo.silInfo.name.isEmpty() || !(nameMap.value(itemInfo.silInfo.name)))
            std::cerr << "silencer " << itemInfo.silInfo.name.toLatin1().data() << " not found." << std::endl;
        else
        {
            itemInfo.silInfo.x = nameMap.value(itemInfo.silInfo.name)->container->entry->value("inv_grid_x", "-1").toInt();
            itemInfo.silInfo.y = nameMap.value(itemInfo.silInfo.name)->container->entry->value("inv_grid_y", "-1").toInt();
            itemInfo.silInfo.w = nameMap.value(itemInfo.silInfo.name)->container->entry->value("inv_grid_width", "1").toInt();
            itemInfo.silInfo.h = nameMap.value(itemInfo.silInfo.name)->container->entry->value("inv_grid_height", "1").toInt();
        }
    }
    if(launchStatus == 2)
    {
        std::clog << "Loading launcher for " << entry->lines.at(0)->value.toLatin1().data() << std::endl;
        itemInfo.launchInfo.name = entry->value("grenade_launcher_name", "");
        itemInfo.launchInfo.attx = entry->value("grenade_launcher_x", "0").toInt();
        itemInfo.launchInfo.atty = entry->value("grenade_launcher_y", "0").toInt();
        if(itemInfo.launchInfo.name.isEmpty() || !(nameMap.value(itemInfo.launchInfo.name)))
            std::cerr << "launcher " << itemInfo.launchInfo.name.toLatin1().data() << " not found." << std::endl;
        else
        {
            itemInfo.launchInfo.x = nameMap.value(itemInfo.launchInfo.name)->container->entry->value("inv_grid_x", "-1").toInt();
            itemInfo.launchInfo.y = nameMap.value(itemInfo.launchInfo.name)->container->entry->value("inv_grid_y", "-1").toInt();
            itemInfo.launchInfo.w = nameMap.value(itemInfo.launchInfo.name)->container->entry->value("inv_grid_width", "-1").toInt();
            itemInfo.launchInfo.h = nameMap.value(itemInfo.launchInfo.name)->container->entry->value("inv_grid_height", "-1").toInt();
        }
    }
    box->boxes[0]->setDisabled( scopeStatus != 2 );
    box->boxes[0]->setChecked( scopeStatus == 1 );
    box->boxes[1]->setDisabled( silStatus != 2 );
    box->boxes[1]->setChecked( silStatus == 1 );
    box->boxes[2]->setDisabled( launchStatus != 2 );
    box->boxes[2]->setChecked( launchStatus == 1 );
    scopeEnabled = silEnabled = launchEnabled = false;
    keyTable->blockSignals(false);
    update_pix();

	delete keyList;
}

void MainWindow::keyCellChanged(int row, int column)
{
    std::cout << QString("%1 = %2").arg(keyTable->item(row, 0)->text(), keyTable->item(row, 1)->text()).toLatin1().data() << std::endl;
    if(keyTable->item(row, 0)->text() == "inv_grid_x")
    {
        itemInfo.x = keyTable->item(row, 1)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "inv_grid_y")
    {
        itemInfo.y = keyTable->item(row, 1)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "inv_grid_height")
    {
        itemInfo.h = keyTable->item(row, 1)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "inv_grid_width")
    {
        itemInfo.w = keyTable->item(row, 1)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "scope_y")
    {
        itemInfo.scopeInfo.atty = keyTable->item(row, column)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "scope_x")
    {
        itemInfo.scopeInfo.attx = keyTable->item(row, column)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "silencer_y")
    {
        itemInfo.silInfo.atty = keyTable->item(row, column)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "silencer_x")
    {
        itemInfo.silInfo.attx = keyTable->item(row, column)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "grenade_launcher_y")
    {
        itemInfo.launchInfo.atty = keyTable->item(row, column)->text().toInt();
    }
    if(keyTable->item(row, 0)->text() == "grenade_launcher_x")
    {
        itemInfo.launchInfo.attx = keyTable->item(row, column)->text().toInt();
    }
    update_pix();
}

QList<QString>* MainWindow::loadEntry(QLTXTreeItem* ltxItem)
{
	CFileLTXEntry *entry = ltxItem->container->entry;
	QList<QString>* list = new QList<QString>;
	if(ltxItem->parent() != NULL)
	{
		list = loadEntry((QLTXTreeItem*) ltxItem->parent());
	}
	for(int index = 0; index < entry->lines.size(); index++)
	{
		QString key = entry->lines.at(index)->key;
		QString value = entry->lines.at(index)->value;
		if( !key.isEmpty() && !value.isEmpty() )
		{
			for(int index2 = 0; index2 < entry->lines.size(); index2++)
			{
				if(entry->lines.at(index2)->value == key)
					(*list)[index2] = entry->lines.at(index2)->value;
            }
			list->append(key);
			list->append(value);
		}
	}
	return list;
}

QLTXTreeItem::QLTXTreeItem(QTreeWidget* parent, QStringList stringlist) : QTreeWidgetItem(parent, stringlist)
{
    container = NULL;
}

QLTXTreeItem::~QLTXTreeItem()
{
	//delete container;
}

attBox::attBox(QStringList texts, QWidget *parent) : QWidget(parent)
{
    signalMapper = new QSignalMapper(this);
    layout = new QHBoxLayout();
    texts.clear();
    texts.append("Scope");
    texts.append("Silencer");
    texts.append("Launcher");
    for(int i = 0; i < texts.size(); ++i)
    {
        QCheckBox* box = boxes[i] = new QCheckBox(texts[i]);
        box->setDisabled(1);
        connect(box, SIGNAL( clicked() ), signalMapper, SLOT(map()));
        signalMapper->setMapping(box, i);
        layout->addWidget(box, (i+1 >= texts.size()), Qt::AlignLeft);
    }
    this->layout->deleteLater();
    this->setLayout(layout);
    connect(signalMapper, SIGNAL( mapped( int ) ), this, SLOT( checkBoxStateChanged(int) ) );
}

void attBox::checkBoxStateChanged( int state )
{
    if(state == 0)
        emit scopeStateChanged( boxes[0]->isChecked() );
    else if(state == 1)
        emit silencerStateChanged( boxes[1]->isChecked() );
    else if(state == 2)
        emit launcherStateChanged( boxes[2]->isChecked() );
}

void MainWindow::scopeStateChanged(bool enabled)
{
    std::clog << "scope ";
    if(enabled)
        std::clog << "on" << std::endl;
    else
        std::clog << "off" << std::endl;
    scopeEnabled = enabled;
    update_pix();
}

void MainWindow::silencerStateChanged(bool enabled)
{
    std::clog << "silencer ";
    if(enabled)
        std::clog << "on" << std::endl;
    else
        std::clog << "off" << std::endl;
    silEnabled = enabled;
    update_pix();
}

void MainWindow::launcherStateChanged(bool enabled)
{
    std::clog << "launcher ";
    if(enabled)
        std::clog << "on" << std::endl;
    else
        std::clog << "off" << std::endl;
    launchEnabled = enabled;
    update_pix();
}

void MainWindow::gamedataSelect()
{
    do
    {
        pathToGamedata = QDir(QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
    }while(!pathToGamedata.exists("gamedata"));
    reloadLTX();
}

void MainWindow::reloadLTX()
{
    vlog->clear();
    LtxTree->clear();
	delete ltx;
    ltx = new CEntryLTX();
    ltx->loadDir(ltxVector, pathToGamedata.absolutePath() + "/gamedata/config/weapons", true);
    //ltx->loadDir(ltxVector, pathToGamedata.absolutePath() + "/gamedata/config/misc");
    QLTXTreeItem* ltxItem;
    QVector<QLTXTreeItem*> ltxItemVector;
    for(int index=0; index<ltxVector.size(); index++)
    {
        CFileLTX* ltxFile = ltxVector.at(index);
        for(int index2 = 0; index2 < ltxFile->lines.size(); index2++)
        {
            CFileLTXLineContainer* container = ltxFile->lines.at(index2);
            if(container->isStructure)
            {
                QString name = container->line->key;
                ltxItem = new QLTXTreeItem( NULL, QStringList( name ));
                ltxItem->container = container;
                nameMap[ name ] = ltxItem;
                ltxItemVector.append( ltxItem );
            }
        }
    }
    for(int index = 0; index < ltxItemVector.size(); index++)
    {
        QLTXTreeItem* child = ltxItemVector.at(index);
        QLTXTreeItem* parent = nameMap.value(child->container->line->value, NULL);
        if( parent )
        {
            parent->addChild( child );
        }
        else
        {
            LtxTree->addTopLevelItem(child);
        }
    }
    delete ui_icon_equipment;
    ui_icon_equipment = new CFileDDS(QString(pathToGamedata.absolutePath() + "/gamedata/textures/ui/ui_icon_equipment.dds"));
    allItems = ui_icon_equipment->uncompress();
}

void MainWindow::exit()
{
    qApp->exit();
}
