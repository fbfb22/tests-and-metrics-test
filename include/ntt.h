#pragma once

#define MAX_ADDR_SIZE 40
typedef struct NttStats
{
	long code;
	char addr[MAX_ADDR_SIZE];
	double name_lookup_time;
	double connect_time;
	double start_transfer_time;
	double total_time;
} NttStats;

typedef void * NttHandle;

typedef int(*TestFunc)(const char *, const int, const char **, const int, const int, NttStats * const);

#define MAX_IMPL_LEN 10
typedef struct Funcs
{
	char name[MAX_IMPL_LEN];
	TestFunc testFunc;
} Funcs;

int Ntt_Init();
NttHandle Ntt_GetHandle(const char * impl);
int Ntt_Register(Funcs * fimp);
int Ntt_DoTest(NttHandle ntt, const char * addr, const int getcnt, const char * hdrs[], 
	const int hdrcnt, const int follow, NttStats * const sts);