#include "ircUserDB.h"

#include "posix_ircUserDB.h"

ircUserDB* ircUserDB::create()
{
	return new posix_ircUserDB();
}
