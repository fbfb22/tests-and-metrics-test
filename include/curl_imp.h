#pragma once

#include "ntt.h"

int curlTest(const char * addr, const int getcnt, const char * hdrs[], const int hdrcnt,
	const int follow, NttStats * const sts);
