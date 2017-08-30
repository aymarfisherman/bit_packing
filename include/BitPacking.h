#pragma once

#ifdef MSVC_IMPLICIT_LINK
#ifndef _DEBUG
#pragma comment(lib, "BitPacking.lib")
#else
#pragma comment(lib, "BitPacking_d.lib")
#endif
#endif

#include "BitSet/BitSet.h"
#include "StringPacking/StringPacking.h"