#include <stdio.h>
#include "getopt.h"
#include "ntt.h"
#define MAX_HEADERS 10

int main(int argc, char ** argv)
{
	char *nvalc = NULL;
	char * headers[MAX_HEADERS];
	char * host = "www.google.com";
	opterr = 0;
	int c;
	int headerCount = 0;
	while ((c = getopt(argc, argv, "H:n:s:")) != -1)
		switch (c)
		{
			case 's':
				host = optarg;
			case 'H':
				if (headerCount >= MAX_HEADERS)
				{
					fprintf(stderr, "Too many headers. Max is %d\n", MAX_HEADERS);
					return 0;
				}
				headers[headerCount] = optarg;
				headerCount++;
				break;
			case 'n':
				nvalc = optarg;
				break;
			case '?':
				if (optopt == 'H' || optopt == 'n')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
				return 1;
			default:
				fprintf(stderr, "Error.\n");
				return 0;
		}

	int nval = nvalc == NULL ? 1 : atoi(nvalc);
	int ret = Ntt_Init();
	if (ret == 0)
	{
		fprintf(stderr, "Failed initialization of 'library'.\n");
		return 0;
	}

	const NttHandle nh = Ntt_GetHandle("CURL");
	if(nh == NULL)
	{
		fprintf(stderr, "No CURL implementations found.\n");
		return 0;
	}
	const char * addr = host;
	NttStats sts;
	int v = Ntt_DoTest(nh, addr, nval, headers, headerCount, 1, &sts);
	if (v == 0)
	{
		printf("SKTEST;%s;%d;%2.2f;%2.2f;%2.2f;%2.2f\n",
			sts.addr, sts.code, sts.name_lookup_time,
			sts.connect_time, sts.start_transfer_time, sts.total_time);
	}
	else
	{
		fprintf(stderr, "Unexpected error\n");
		return 0;
	}

	return 1;
}

