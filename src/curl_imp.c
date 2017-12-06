#include <curl/curl.h>

#include "curl_imp.h"
#include "utils.h"

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	return size * nmemb;
}

int curlTest(const char * addr, const int getcnt, const char * hdrs[], const int hdrcnt, 
	const int follow, NttStats * const sts)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *chunk = NULL;

	if (addr == NULL || strlen(addr) < 1) return -1;
	if (hdrcnt > 0)
	{
		for (int i = 0; i < hdrcnt; i++)
		{
			if (strlen(hdrs[i]) < 1)
			{
				return -1;
			}
		}
	}
	if (sts == NULL)
	{
		return -1;
	}

	curl = curl_easy_init();

	if (!curl)
	{
		return -1;
	}

	res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	if (CURLE_OK != res) 
	{
		goto cleanup1;
	}

	res = curl_easy_setopt(curl, CURLOPT_URL, addr);
	if (CURLE_OK != res)
	{
		goto cleanup1;
	}

	if (follow)
	{	
		res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow);
		if (CURLE_OK != res)
		{
			goto cleanup1;
		}
	}

	if (hdrcnt > 0)
	{
		for (int h = 0; h < hdrcnt; h++)
		{
			chunk = curl_slist_append(chunk, hdrs[h]);
		}
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
		if (CURLE_OK != res)
		{
			goto cleanup1;
		}
	}

	sts->connect_time = 0.0;
	sts->name_lookup_time = 0.0;
	sts->start_transfer_time = 0.0;
	sts->total_time = 0.0;

	NttStats * stsarr = (NttStats *) malloc(sizeof(NttStats) * getcnt);
	if (stsarr == NULL) goto cleanup1;
	//printf("sizeof sts %d\n", sizeof(NttStats));
	for (int cnt = 0; cnt < getcnt; cnt++)
	{
		NttStats * thissts = stsarr + cnt;
		//printf("stsarr %x\n", thissts);
		res = curl_easy_perform(curl);

		if (CURLE_OK == res) 
		{
			res = curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &(thissts->name_lookup_time));
		}
		if (CURLE_OK == res) 
		{
			res = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &(thissts->connect_time));
		}
		if (CURLE_OK == res) 
		{
			res = curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &(thissts->start_transfer_time));
		}
		if (CURLE_OK == res) 
		{
			res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &(thissts->total_time));
		}

		if (CURLE_OK == res && cnt == getcnt - 1)
		{
			char * addr;
			res = curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &addr);
			if (CURLE_OK == res)
			{
				if (addr && strlen(addr) < MAX_ADDR_SIZE)
				{
					strncpy_s(sts->addr, MAX_ADDR_SIZE, addr, MAX_ADDR_SIZE - 1);
					sts->addr[MAX_ADDR_SIZE - 1] = '\0';
				}
			}
			if (CURLE_OK == res) 
			{
				res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(sts->code));
			}

			if (CURLE_OK == res) 
			{	
				double * vals = (double *)malloc(sizeof(double) * getcnt);
				if (vals == NULL) goto cleanup2;
				for (int j = 0; j < getcnt; j++)
				{
					*(vals + j) = (stsarr + j)->connect_time;
				}
				sts->connect_time = median(getcnt, vals);

				for (int j = 0; j < getcnt; j++)
				{
					*(vals + j) = (stsarr + j)->name_lookup_time;
				}
				sts->name_lookup_time = median(getcnt, vals);
				
				for (int j = 0; j < getcnt; j++)
				{
					*(vals + j) = (stsarr + j)->start_transfer_time;
				}
				sts->start_transfer_time = median(getcnt, vals);
				
				for (int j = 0; j < getcnt; j++)
				{
					*(vals + j) = (stsarr + j)->total_time;
				}
				sts->total_time = median(getcnt, vals);
				free(vals);
			}
			if (CURLE_OK != res)
			{
				break;
			}

		}
	}


	/* always cleanup */
cleanup2:
	free(stsarr);
cleanup1:
	curl_easy_cleanup(curl);
	if (hdrcnt > 0 && chunk != NULL)
	{
		curl_slist_free_all(chunk);
	}

	return (res == CURLE_OK) ? 0 : -1;
}
