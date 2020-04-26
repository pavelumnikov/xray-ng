// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/string.h"

//-----------------------------------------------------------------------------------------------------------

#ifdef XR_TODO_COMMENT
#   error do not define XR_TODO_COMMENT macro
#endif // #ifdef XR_TODO_COMMENT

#define XR_TODO_COMMENT(message_to_show) \
    __pragma(message(__FILE__ "(" XR_MAKE_STRING(__LINE__) "): @todo: " message_to_show))
	
//-----------------------------------------------------------------------------------------------------------