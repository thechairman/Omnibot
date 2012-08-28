#include "OmniBlocker.h"
#include "posix_OmniBlocker.h"

OmniBlocker* OmniBlocker::create()
{

	return new posix_OmniBlocker();

}
