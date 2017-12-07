#include "ntt.h"
#include <assert.h>
#include <stdbool.h>

static void test_0()
{
	int i = Ntt_Init();
	assert(i != 0 && "test_0");
}


static void tets_1()
{
	NttHandle nh = Ntt_GetHandle("BLA");
	assert(nh == NULL && "test_1");
}

static void test_2()
{
	NttHandle nh = Ntt_GetHandle("CURL");
	assert(nh != NULL && "test_2");
}

int main()
{
	test_0();
	test_1();
	test_2();
	return 0;
}
