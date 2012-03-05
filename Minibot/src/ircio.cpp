#include "ircio.h"

#include "posix_ircio.h"

ircio* ircio::create()
{
	posix_ircio* io = new posix_ircio;
	return (ircio*) io;
}
