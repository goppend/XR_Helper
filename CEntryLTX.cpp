#include "CEntryLTX.h"

CEntryLTX::CEntryLTX()
{

}

CEntryLTX::~CEntryLTX()
{

}

void CEntryLTX::loadDir(QVector<CFileLTX *>& ltxVector, QString dirPath, bool clear)
{
    if(clear)
    {
        for(int index = 0; index < ltxVector.size(); index++)
        {
			delete ltxVector.at(index);
        }
        ltxVector.clear();
    }
	std::clog << "Loading path " << dirPath.toLatin1().data() << std::endl;
    QDir searchPath(dirPath);
    if(!searchPath.exists())
    {
		std::cerr << "Cannot find directory " << dirPath.toLatin1().data() << std::endl;
		return;
    }
    searchPath.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	searchPath.setSorting(QDir::DirsLast | QDir::Name | QDir::IgnoreCase);
    QFileInfoList searchInfo = searchPath.entryInfoList();
    if( searchInfo.length() <= 0 )
    {
		std::clog << "No ltx files found in directory " << dirPath.toLatin1().data() << std::endl;
		return;
    }
    for(int index = 0; index < searchInfo.length(); index++)
    {
        QString filePath = searchInfo[index].absoluteFilePath().toLatin1().data();
		if(searchInfo[index].isDir())
        {            
            loadDir( ltxVector, filePath, false);        //another directory, recurse this function
        }
        else
		{
            CFileLTX* ltx = new CFileLTX;
            ltx->readLTX(filePath);
            QString fileName = filePath.right(filePath.length()-filePath.lastIndexOf("/")-1);
			ltxVector.append(ltx);
			std::clog << fileName.toLatin1().data() << " added " << ltxVector.last()->lines.size() << " entries." << std::endl;
        }
    }
}
