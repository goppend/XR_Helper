#ifndef CFILELTX_H
#define CFILELTX_H

#include <iostream>
#include <QString>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QVector>

#include "qtil.h"

struct CFileLTXLine
{
	enum linetype
	{
		line,
		include,
		entry
	};

	CFileLTXLine( linetype type = line )
	{
		this->type = type;
	}

	~CFileLTXLine() {}

	linetype type;
	QString key , value , comment;
};

struct CFileLTXEntry
{
	CFileLTXEntry( void ) {}
	QString value(QString indexString, const QString& wef = QString());

    CFileLTXLine line;
	QVector< CFileLTXLine* > lines;
    ~CFileLTXEntry();
};


struct CFileLTX;

struct CFileLTXLineContainer
{
    CFileLTXLineContainer( CFileLTXEntry *entry, CFileLTX *parent = NULL )
	{
		isStructure = true;
		this->entry = entry;
        this->parent = parent;
	}

    CFileLTXLineContainer( CFileLTXLine *line, CFileLTX *parent = NULL )
	{
		isStructure = false;
		this->line = line;
        this->parent = parent;
	}

    bool isStructure;

	union
	{
		CFileLTXEntry *entry;
		CFileLTXLine *line;
	};

    CFileLTX *parent;
    ~CFileLTXLineContainer();
};

struct CFileLTX
{
    QString filePath;
	QVector< CFileLTXLineContainer* > lines;
	void readLTX( QString path );
	void printLine( int index );
	void writeEntry( QTextStream &stream , CFileLTXEntry *entry );
    void save( const QString &path = QString());
    ~CFileLTX();
};

#endif // CFILELTX_H
