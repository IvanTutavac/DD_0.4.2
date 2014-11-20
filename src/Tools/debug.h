#pragma once

#include "../Data/Defines.h"

#if defined(_MSC_VER)

#if defined(_DEBUG) && defined(DD_TRACK_ALLOCATIONS)
#include <stdlib.h>
#include <crtdbg.h>
# define DD_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
# define DD_NEW new
#endif

#if !defined(DD_DELETE)
#define DD_DELETE(x) if(x) delete x; x=nullptr;
#endif

#if !defined(DD_DELETE_ARRAY)
#define DD_DELETE_ARRAY(x) if(x) delete [] x; x=nullptr; 
#endif

#else 
# define DD_NEW new
#define DD_DELETE(x) if(x) delete x; x=nullptr;
#define DD_DELETE_ARRAY(x) if(x) delete [] x; x=nullptr; 

#endif
