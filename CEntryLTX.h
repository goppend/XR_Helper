#ifndef CENTRYLTX_H
#define CENTRYLTX_H

#include <QDir>
#include <QFile>
#include <QHash>
#include <QStringList>
#include "CFileLTX.h"
#include "VLogger.h"

class CEntryLTX
{
public:
    void loadLTX( QString );
    void loadDir( QVector<CFileLTX* >&, QString, bool clear = true);
    CEntryLTX();
	~CEntryLTX();
};

#endif // CENTRYLTX_H
