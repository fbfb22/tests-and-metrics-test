#include <string.h>

#include "ntt.h"
#include "curl_imp.h"

#define MAX_FIMPL 3
static Funcs implementations[MAX_FIMPL] = { {"",NULL},{ "",NULL },{ "",NULL } };
int implCount = 0;

int Ntt_Init()
{
	Funcs curlFuncs = { "CURL", curlTest };
	int ret = Ntt_Register(&curlFuncs);
	return ret;
}

int Ntt_Register(Funcs * fimp)
{
	for (int j = 0; j < MAX_FIMPL; j++)
	{
		if (strncmp(implementations[j].name, fimp->name, MAX_IMPL_LEN) == 0)
		{
			implementations[j].testFunc = fimp->testFunc;
			return 1;
		}
	}
	if (implCount < MAX_FIMPL)
	{
		int n = strlen(fimp->name);
		if (n > MAX_IMPL_LEN - 1)
		{
			return 0;
		}
		strncpy_s(implementations[implCount].name, MAX_IMPL_LEN, fimp->name, n);
		implementations[implCount].testFunc = fimp->testFunc;
		implCount++;
		return 1;
	}
	return 0;
}

NttHandle Ntt_GetHandle(const char * impl)
{
	for (int j = 0; j < MAX_FIMPL; j++)
	{
		if (strncmp(implementations[j].name, impl, MAX_IMPL_LEN) == 0)
		{
			return &(implementations[j]);
		}
	}
	return NULL;
}

int Ntt_DoTest(NttHandle ntt, const char * addr, const int getcnt, const char * hdrs[], 
	const int hdrcnt, const int follow, NttStats * const sts)
{
	return ((Funcs *)ntt)->testFunc(addr, getcnt, hdrs, hdrcnt, follow, sts);
}