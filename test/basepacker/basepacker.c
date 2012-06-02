#include "basepacker.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define PACKER_ERROR	0
#define PACKER_OK	1
#define PACKER_ACSERR	-1

/*		data structure defination		*/
typedef struct qbase_indexitem	{
	char item_name[24];
	int data_idx;;
} qbase_indexitem;

typedef struct qbase_packIndex	{
	int resid;
	int reslength;
	qbase_indexitem *items;
} qbase_packIndex;

struct qbase_pck	{
	char prefix_name[8];
	char pwd[20];
	int version;
	int datasize;
	qbase_packIndex indexer[6];
	qbase_byte *databytes;
};


/*		static helper function defination		*/
static char *
pwd_create(const char *pwd)	{
	char *npwd = NULL;
	if(pwd == NULL)
		return NULL;
	return npwd;
}

static qbase_pck *
pck_create_ver100(const char *pwd)	{
	qbase_pck *pck = (qbase_pck*)malloc(sizeof(qbase_pck));
	int i;
	char *npwd = NULL;
	strcpy(pck->prefix_name,"QUABZIP");
	npwd = pwd_create(pwd);
	if(npwd != NULL)	{
		strcpy(pck->pwd, npwd);
		free(npwd);
	}
	else pck->pwd[0] = '\0';
	pck->version = PACKVER_100;
	pck->datasize = 0;
	for(i = 0; i < 6; i++)	{
		pck->indexer[i].resid = i;
		pck->indexer[i].reslength = 0;
		pck->indexer[i].items = NULL;
	}
	pck->databytes = NULL;
	return pck;
}

static int
pck_writefile(char *path, qbase_pck *pck)	{
	FILE *f = NULL;
	int i, itemidx;
	if(pck == NULL)
		return PACKER_ERROR;
	if(f != NULL)	{
		/*	write header	*/
		fwrite(&pck->prefix_name, sizeof(char), 8, f);
		fwrite(&pck->pwd, sizeof(char), 20, f);
		fwrite(&pck->version, sizeof(int), 1, f);
		fwrite(&pck->datasize, sizeof(int), 1, f);
		/*	write index	*/
		for(i = 0; i < 6; i++)	{
			fwrite(&pck->indexer[i].resid, sizeof(int), 1, f);
			fwrite(&pck->indexer[i].reslength, sizeof(int), 1, f);
			if(pck->indexer[i].items != NULL && pck->indexer[i].reslength>0)	{
				for(itemidx = 0; itemidx < pck->indexer[i].reslength; itemidx++)	{
					fwrite(&pck->indexer[i].items[i].item_name, sizeof(char), 24, f);
					fwrite(&pck->indexer[i].items[i].data_idx, sizeof(int), 1, f);
				}
			}
		}
		if(pck->datasize > 0 && pck->databytes != NULL)	{
			fwrite(pck->databytes, sizeof(qbase_byte), pck->datasize,f);
		}
		fclose(f);
		return PACKER_OK;

	}
	else return PACKER_ACSERR;
}

static void
pck_free(qbase_pck *pck)	{
	int i;
	free(pck->databytes);
	for(i = 0; i < 6; i++)	{
		if(pck->indexer[i].items != NULL)
			free(pck->indexer[i].items);
		pck->indexer[i].resid = 0;
		pck->indexer[i].reslength = 0;
		pck->indexer[i].items = NULL;
	}
	pck->version = 0;
	pck->datasize = 0;
	free(pck);
}


static void
res_addidx(int res, const char *name, int dataidx)	{
	return;
}

static void
res_removeidx(int res, const char *name)	{
	return ;
}

static void
res_updateidx(int res, const char *oldname, const char *newname, int newidx)	{
	return;
}

/*		basepacker's api implementation		*/
qbase_pck*
qbase_packer_create(char *path, char *pwd, int ver)	{
	qbase_pck *pck = NULL;
	switch(ver)	{
	case PACKVER_100:
		pck = pck_create_ver100(pwd);
		break;
	default:
		pck = NULL;
		break;
	}
	if(pck != NULL)	{
		// TODO: write to file
		if(pck_writefile(path, pck) != PACKER_OK)	{
			pck_free(pck);
			pck = NULL;
		}
	}
	return pck;
}

qbase_byte*
qbase_packer_get(qbase_pck *pck, int pres, char *name, char *pwd, int ver)	{
	if(pck == NULL)
		return NULL;
	return NULL;
}

int
qbase_packer_add(qbase_pck *pck, int pres, qbase_byte *bytes, char *name, char *pwd, int ver)	{
	return PACKER_ERROR;
}

int
qbase_packer_setpwd(qbase_pck *pck, char *pwd)	{
	return PACKER_ERROR;
}
int
qbase_packer_update(qbase_pck *pck, int pres, char *name, qbase_byte *newbytes, char *pwd, int ver)	{
	return PACKER_ERROR;
}
int
qbase_packer_remove(qbase_pck *pck, int pres, char *name ,char *pwd, int ver)	{
	return PACKER_ERROR;
}
