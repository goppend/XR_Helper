#include "CFileLTX.h"


void CFileLTX::readLTX( QString path )
{
	QFile file( path );

	if( file.open( QIODevice::ReadOnly ) )
	{
        filePath = path;
		QTextStream stream( &file );
		QString string = stream.readLine().trimmed();
		CFileLTXEntry *lastEntry = NULL;

		while( !string.isNull() )
		{
			// Construct line
			CFileLTXLine *line = new CFileLTXLine( CFileLTXLine::line );

			// Set line
			int commentStart = string.indexOf( ';' );
			if( commentStart != -1 )
				line->comment = string.mid( commentStart );
			string.chop( line->comment.length() );
			line->comment = line->comment.trimmed();

			// Handle line
			if( string.startsWith( '#' ) ) // Include
			{
				line->key = string.trimmed();

				line->type = CFileLTXLine::include;
                lines.append( new CFileLTXLineContainer( line, this ) );
			}
			else if( string.startsWith( '[' ) ) // Entry
			{
				QStringList parts = string.split( ':' );

				if( parts.length() >= 1 )
				{
					line->key = parts.at( 0 );
					line->key = line->key.remove( '[' ).remove( ']' ).trimmed();

					if( parts.length() >= 2 )
						line->value = parts.at( 1 ).trimmed();
				}

				line->type = CFileLTXLine::entry;
				lastEntry = new CFileLTXEntry;
				lastEntry->line = *line;
				delete line;
				lines.append( new CFileLTXLineContainer( lastEntry , this ) );
			}
			else if( string.isEmpty() ) // Blank line or comment
			{
				if( lastEntry )
					lastEntry->lines.append( line );
				else
					lines.append( new CFileLTXLineContainer( line , this ) );
			}
			else if( lastEntry ) // Keyvalue
			{
				QStringList parts = string.split( '=' );

				if( parts.length() >= 1 )
				{
					line->key = parts.at( 0 ).trimmed();

					if( parts.length() >= 2 )
						line->value = parts.at( 1 ).trimmed();
				}

				lastEntry->lines.append( line );
			}

			string = stream.readLine().trimmed();
		}
	}
}

void CFileLTX::printLine( int index )
{
	CFileLTXLineContainer *container = lines.value( index , NULL );

	if( container->isStructure )
		std::cout << QString( "%1 , %2 , %3" ).arg( container->entry->line.key ).arg( container->entry->line.value ).arg( container->entry->line.comment ).toLatin1().data() << std::endl;
	else if( !container->isStructure && ( !container->line->key.isEmpty() || !container->line->value.isEmpty() || !container->line->comment.isEmpty() ) )
		std::cout << QString( "%1 , %2 , %3" ).arg( container->line->key ).arg( container->line->value ).arg( container->line->comment ).toLatin1().data() << std::endl;
}

void CFileLTX::writeEntry( QTextStream &stream , CFileLTXEntry *entry )
{
	if( entry )
	{
		int keyWidth = 0;
		for( int index = 0 ; index < entry->lines.size() ; index++ )
			keyWidth = qMax( entry->lines.at( index )->key.length() , keyWidth );

		for( int index = 0 ; index < entry->lines.size() ; index++ )
		{
			CFileLTXLine *line = entry->lines.at( index );

			if( !line->key.isEmpty() && !line->value.isEmpty() )
			{
				stream << QString( "%1" ).arg( line->key );

				// Ghetto
				for( int spaceIndex = line->key.length() ; spaceIndex < keyWidth ; spaceIndex++ )
					stream << ' ';

				stream << QString( " = %1" ).arg( line->value );

				if( !line->comment.isEmpty() )
					stream << QString( " %1\n" ).arg( line->comment );
				else
					stream << QString( "\n" );
			}
			else if( !line->comment.isEmpty() )
				stream << QString( "%1\n" ).arg( line->comment );
			else
				stream << QString( "\n" );
		}
	}
}

void CFileLTX::save( const QString &path )
{
    QFile file( path.isEmpty()?filePath:path );

	CFileLTXLineContainer *container = NULL;

	if( file.open( QIODevice::WriteOnly ) )
	{
		QTextStream stream( &file );

		for( int index = 0 ; index < lines.size() ; index++ )
		{
			container = lines.value( index , NULL );

			if( container->isStructure )
			{
				stream << QString( "[%1]:%2 %3\n" ).arg( container->entry->line.key ).arg( container->entry->line.value ).arg( container->entry->line.comment ).toLatin1().data();

				writeEntry( stream , container->entry );
			}
			else if( container->line->key.isEmpty() ) // Only comment
			{
				stream << QString( "%1\n" ).arg( container->line->comment );
			}
			else
				stream << QString( "%1 %2\n" ).arg( container->line->key ).arg( container->line->comment );
		}
	}
}

QString CFileLTXEntry::value(QString indexString, const QString &wef)
{
	CFileLTXLine* line = NULL;
	for(int index = 0; index<lines.size(); index++)
	{
		line = lines.at(index);
		if(line->key == indexString)
		{
			return line->value;
		}
	}
	return wef;
}

CFileLTXEntry::~CFileLTXEntry()
{
	for( int index = 0 ; index < lines.size() ; index++ )
    {
		delete lines.at( index );
    }
}

CFileLTXLineContainer::~CFileLTXLineContainer()
{
	if( isStructure )
    {
        delete entry;
    }
    else
    {
        delete line;
    }
}

CFileLTX::~CFileLTX()
{
	for( int index = 0 ; index < lines.size() ; index++ )
    {
		delete lines.at( index );
    }
}
