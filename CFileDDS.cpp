#include "CFileDDS.h"

CFileDDS::CFileDDS( QString name )
{
	ucPixels = NULL;
	image = NULL;

    QFile file( name );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        return;
    }
	std::clog << "Using texture file:" << name.toLatin1().data() << std::endl;
    QDataStream stream( &file );
    stream.setByteOrder( QDataStream::LittleEndian );
    stream.readRawData( (char*)header.Magic, 4 );
    stream >> header.Size;
    stream >> header.Flags;
    stream >> header.Height;
    stream >> header.Width;
    stream >> header.PitchOrLinearSize;
    stream >> header.Depth;
    stream >> header.MipMapCount;
    stream.readRawData( (char*)header.Reserved1, 11*sizeof(int) );
    stream >> header.ddspf.Size;
    stream >> header.ddspf.Flags;
    stream >> header.ddspf.FourCC;
    stream >> header.ddspf.RGBBitCount;
    stream >> header.ddspf.RBitMask;
    stream >> header.ddspf.GBitMask;
    stream >> header.ddspf.BBitMask;
    stream >> header.ddspf.ABitMask;
    stream >> header.Caps;
    stream >> header.Caps2;
    stream >> header.Caps3;
    stream >> header.Caps4;
    stream >> header.Reserved2;
    switch(header.ddspf.FourCC)
    {
        case FOURCC('D','X','T','1'):     // DXT1
			std::clog << "DDS Loaded as DXT1." << std::endl;
            byteCount = ( header.Height * header.Width ) / 2;
            byteCount = qMax(8, byteCount);
            break;
        case FOURCC('D','X','T','2'):     // DXT2
			std::clog << "DDS Loaded as DXT2." << std::endl;
            byteCount = header.Height * header.Width;
            byteCount = qMax(16, byteCount);
            break;
        case FOURCC('D','X','T','3'):     // DXT3
			std::clog << "DDS Loaded as DXT3." << std::endl;
            byteCount = header.Height * header.Width;
            byteCount = qMax(16, byteCount);
            break;
        case FOURCC('D','X','T','4'):     // DXT4
			std::clog << "DDS Loaded as DXT4." << std::endl;
            byteCount = header.Height * header.Width;
            byteCount = qMax(16, byteCount);
            break;
        case FOURCC('D','X','T','5'):     // DXT5
			std::clog << "DDS Loaded as DXT5." << std::endl;
            byteCount = header.Height * header.Width;
            byteCount = qMax(16, byteCount);
            break;
        case FOURCC('D','X','1','0'):     // DX10
			std::clog << "DDS encoded with DX10." << std::endl;
        default:
            std::cerr << "DDS Failed to load, incompatible encoding.";
            byteCount = 0;
            break;
	}
	pixels = ( char* )malloc( byteCount );
    stream.readRawData( pixels, byteCount );
}

QImage* CFileDDS::uncompress()
{
	if( byteCount && !image )
	{
		ucPixels = ( squish::u8* )malloc( header.Width * header.Height * 16 );
        squish::DecompressImage( ucPixels, header.Width, header.Height, ( const void* )pixels, squish::kDxt5 );
        image = new QImage( ( uchar* ) ucPixels, header.Width, header.Height, QImage::Format_RGBA8888 );
    }

    return image;
}

CFileDDS::~CFileDDS()
{
    delete image;
    free(ucPixels);
    free(pixels);
}
