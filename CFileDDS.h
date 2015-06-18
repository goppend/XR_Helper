#ifndef CFILEDDS_H
#define CFILEDDS_H

#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include <VLogger.h>
#include "squish.h"

#define FOURCC( one , two , three , four ) ( (four) << 24 | (three) << 16 | (two) << 8 | (one) )

class CFileDDS
{
protected:
	QImage* image;
	squish::u8* ucPixels;
	char* pixels;
    int byteCount;
    struct DDS_PIXELFORMAT
    {
      qint32  Size;
      qint32  Flags;
      qint32  FourCC;
      qint32  RGBBitCount;
      qint32  RBitMask;
      qint32  GBitMask;
      qint32  BBitMask;
      qint32  ABitMask;
    };
    struct DDS_HEADER
    {
      qint8  Magic[4];
      qint32 Size;
      qint32 Flags;
      qint32 Height;
      qint32 Width;
      qint32 PitchOrLinearSize;
      qint32 Depth;
      qint32 MipMapCount;
      qint32 Reserved1[11];
      DDS_PIXELFORMAT ddspf;
      qint32 Caps;
      qint32 Caps2;
      qint32 Caps3;
      qint32 Caps4;
      qint32 Reserved2;
    };

public:
    DDS_HEADER header;
    QImage* uncompress();
    CFileDDS( QString );
    ~CFileDDS();
};

#endif // CFILEDDS_H
