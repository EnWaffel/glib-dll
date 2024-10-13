#include "glib/utils/Easing.h"

#include <tweeny.h>

using namespace glib;

tweeny::easing::enumerated __translate_easing(Easing easing)
{
	return (tweeny::easing::enumerated)(int)easing;
}