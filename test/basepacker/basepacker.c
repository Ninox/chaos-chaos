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
typedef struct qbase_packIndexer {
    char name[20];
    int data_offset;
    int data_size;
    struct qbase_packIndexer *prev;
    struct qbase_packIndexer *next;
} qbase_packIndexer;

typedef struct qbase_typeBlocks {
    qbase_packIndexer *current;
    struct qbase_typeBlocks *prev;
    struct qbase_typeBlocks *next;
} qbase_typeBlocks;

struct qbase_pck    {
    char prefix[10];
    short version;
    int datasize;
    qbase_packIndexer *indexer;
    qbase_byte *databytes;
    char encrypt_token[24];
    qbase_typeBlocks* blocks[6];
};


/*		static helper function defination		*/
/*   pck operations   */
static qbase_pck *
pck_create(char *path, char *pwd, int ver) {
    qbase_pck *pck = NULL;
    int i;
    FILE *f = fopen(path, "r");
    if(f == 0)  {
        fclose(f);
        /*  if file existed, load it */
        return qbase_packer_load(path, ver);
    }
    pck = (qbase_pck*)malloc(sizeof(qbase_pck));
    pck->version            = PACKVER_100;
    pck->datasize           = 0;
    pck->indexer            = NULL;
    pck->databytes          = NULL;
    memset(&pck->encrypt_token[0], 0, 10);
    strcpy(pck->prefix,"QUAB_PACK");
    for(i = 0; i < 6; i++)  {
        pck->blocks[i]->current = NULL;
        pck->blocks[i]->next    = NULL;
        pck->blocks[i]->prev    = NULL;
    }
    return pck;
}

static qbase_pck *
pck_loadfile(const char *path) {
    return NULL;
}

static void
pck_writefile(qbase_pck *pck, const char *path,
              qbase_byte *appendBytes, int sz) {
}

static void
pck_free(qbase_pck *pck)    {
    int i;
    qbase_packIndexer *pIdx = NULL, *pIdex_next = NULL;
    qbase_typeBlocks *ptB = NULL, *ptB_next = NULL;;
    pck->version = -1;
    pck->datasize = 0;
    memset(pck->prefix, 0, 10);
    memset(pck->encrypt_token, 0, 24);
    if(pck->databytes != NULL)
        free(pck->databytes);
    if(pck->indexer != NULL)    {
        pIdx = pck->indexer;
        do
        {
            pIdex_next = pIdx->next;
            free(pIdx);
            pIdx = pIdex_next;
        }while(pIdx != NULL);
    }
    for(i = 0; i < 6; i++)  {
        if(pck->blocks[i] != NULL)  {
            ptB = pck->blocks[i];
            do
            {
                ptB_next = ptB->next;
                free(ptB);
                ptB = ptB_next;
            }while(ptB != NULL);
        }
    }
    free(pck);
}

/*   pwd operations */
static void
pwd_create(char *oldPwd, char *pwd)  {

}

static int
pwd_check(qbase_pck *pck, char *pwd)    {
    return PACKER_ERROR;
}

static qbase_byte *
pwd_translate(qbase_byte *data, int *sz, char *pwd) {
    return NULL;
}

/*		basepacker's api implementation		*/
qbase_pck *
qbase_packer_create(char *path, char *pwd, int ver)  {
    qbase_pck *pck = pck_create(path, pwd, ver);
    if(pck != NULL)
        pck_writefile(pck, path, NULL, 0);
    return pck;
}

qbase_pck *
qbase_packer_load(char *path, int ver)    {
    switch(ver)
    {
    case PACKVER_100:
        return pck_loadfile(path);
    default:
        return NULL;
    }
}

void
qbase_packer_close(qbase_pck *pck) {
    if(pck != NULL)
        pck_free(pck);
}

qbase_byte*
qbase_packer_get(qbase_pck *pck, int *sz, int pres,
                             char *name, char *pwd, int ver)  {
    qbase_typeBlocks *pBlocks = NULL;
    qbase_byte *data = NULL;
    if(pck == NULL) {
        return NULL;
    }
    if(pck->blocks[pres] == NULL)   {
        return NULL;
    }
    pBlocks = pck->blocks[pres];
    /*   get the bytes index   */
    if(pwd_check(pck, pwd) == 0)
        return NULL;
    while(pBlocks != NULL)  {
        if(pBlocks->current == NULL)    {
            pBlocks = pBlocks->next;
            continue;
        }
        /*  search by key name  */
        if(strcmp(pBlocks->current->name, name) == 0)   {
            *sz = pBlocks->current->data_size;
            data = pck->databytes + pBlocks->current->data_offset;
            data = pwd_translate(data, sz, pwd);
        }
        pBlocks = pBlocks->next;
    }
    return data;
}

int
qbase_packer_add(qbase_pck *pck, int pres,
                     qbase_byte *bytes, int sz, char *name)  {
    qbase_packIndexer *idx = (qbase_packIndexer*)malloc(sizeof(qbase_packIndexer));
    qbase_typeBlocks *bck = (qbase_typeBlocks*)malloc(sizeof(qbase_typeBlocks));
    qbase_packIndexer *pIndexer = pck->indexer;
    qbase_typeBlocks *pBlock =pck->blocks[pres];
    qbase_byte *new_data = NULL;
    if(pck == NULL || bytes == NULL || pres < 0 || name == NULL)    {
        free(idx);
        free(bck);
        return PACKER_ERROR;
    }
    idx->data_offset    = pck->datasize;
    idx->data_size      = sz;
    idx->next           = NULL;
    strcpy(idx->name, name);
    /* get tails of indexer */
    while(pIndexer != NULL && pIndexer->next != NULL) {
        pIndexer = pIndexer->next;
    }
    while(pBlock != NULL && pBlock->next != NULL)   {
        pBlock = pBlock->next;
    }
    idx->prev = pIndexer;
    pIndexer->next = idx;
    pBlock->next = bck;
    bck->current = idx;
    bck->prev = pBlock;
    /*  create a new data here  */
    new_data = (qbase_byte*)malloc(pck->datasize + sz);
    memcpy(new_data, pck->databytes, pck->datasize);
    memcpy(new_data+pck->datasize, bytes, sz);
    free(pck->databytes);
    pck->databytes = new_data;
    pck->datasize += sz;

    return PACKER_OK;
}

int
qbase_packer_setpwd(qbase_pck *pck, char *oldPwd, char *pwd)  {
    return PACKER_ERROR;
}

int
qbase_packer_update(qbase_pck *pck, int pres, char *name,
                        qbase_byte *newbytes, int sz, char *pwd, int ver) {
    return PACKER_ERROR;
}

int
qbase_packer_remove(qbase_pck *pck, int pres,
                        char *name ,char *pwd, int ver)   {
    return PACKER_ERROR;
}
