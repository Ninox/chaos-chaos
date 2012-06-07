#include "basepacker.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define PACKER_ERROR	0
#define PACKER_OK	1
#define PACKER_ACSERR	-1

/*   TODO:  this whole module must optimize in later version   */

/*		data structure defination		*/
typedef struct qbase_packIndexer {
    char name[20];
    qbase_byte *data;
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
    int remainSize;
    int pres, dz;
    qbase_byte *data = NULL;
    char fname[20];
    qbase_packIndexer *idx = NULL;
    qbase_typeBlocks *bck = NULL;
    qbase_pck *pck = (qbase_pck*)malloc(sizeof(qbase_pck));

    FILE *f = fopen(path, "rb");
    if(f != NULL)   {
        fread(&pck->prefix[0], sizeof(char), 10, f);
        fread(&pck->version, sizeof(int), 1, f);
        fread(&pck->encrypt_token[0], sizeof(char), 24, f);
        fread(&pck->datasize, sizeof(int), 1, f);
        remainSize = pck->datasize;
        while(remainSize > 0)   {
            idx = (qbase_packIndexer*)malloc(sizeof(qbase_packIndexer));
            bck = (qbase_typeBlocks*)malloc(sizeof(qbase_typeBlocks));
            fread(&pres, sizeof(int), 1, f);
            fread(&fname[0], sizeof(char), 20, f);
            fread(&dz, sizeof(int), 1, f);
            data = (qbase_byte*)malloc(dz);
            fread(data, sizeof(qbase_byte), dz, f);

            idx->data_size = dz;
            idx->data = data;
            strcpy(idx->name, fname);
            bck->current = idx;

            /*  set to index pointer  */
            if(pck->indexer == NULL)    {
                pck->indexer = idx;
                idx->next = NULL;
                idx->prev = NULL;
            }
            else    {
                if(pck->indexer->prev == NULL)  {
                    pck->indexer->next = (pck->indexer->prev = idx);
                    idx->next = (idx->prev = pck->indexer);
                }
                else    {
                    idx->next = pck->indexer;
                    idx->prev = pck->indexer->prev;
                    pck->indexer->prev->next = idx;
                    pck->indexer->prev = idx;
                }
            }
            /*  set to blocks pointer  */
            if(pck->blocks[pres] == NULL)    {
                pck->blocks[pres]  = bck;
                bck->next = NULL;
                bck->prev = NULL;
            }
            else    {
                if(pck->blocks[pres]->prev == NULL)  {
                    pck->blocks[pres]->next = (pck->blocks[pres] ->prev = bck);
                    bck->next = (bck->prev = pck->blocks[pres] );
                }
                else    {
                    bck->next = pck->blocks[pres] ;
                    bck->prev = pck->blocks[pres] ->prev;
                    pck->blocks[pres] ->prev->next = bck;
                    pck->blocks[pres] ->prev = bck;
                }
            }
        }
    }
    return NULL;
}

static void
pck_writefile(qbase_pck *pck, const char *path) {
    int i;
    qbase_typeBlocks *pb = NULL;
    FILE *f = fopen(path, "wb");
    if(f != NULL)   {
        if(pck != NULL) {
            fwrite(&pck->prefix[0], sizeof(char), 10, f);
            fwrite(&pck->version, sizeof(int), 1, f);
            fwrite(&pck->encrypt_token[0], sizeof(char), 24, f);
            fwrite(&pck->datasize, sizeof(int), 1, f);
            for(i = 0; i < 6; i++)  {
                pb = pck->blocks[i];
                if(pb == NULL) continue;
                do
                {
                    fwrite(&i, sizeof(int), 1, f);
                    fwrite(&pb->current->name, sizeof(char), 20, f);
                    fwrite(&pb->current->data_size, sizeof(int), 1, f);
                    fwrite(pb->current->data, sizeof(qbase_byte), pb->current->data_size, f);
                    pb = pb->next;
                }while(pb != NULL && pb != pck->blocks[i]);
            }
        }
        fclose(f);
    }
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
    if(pck->indexer != NULL)    {
        pIdx = pck->indexer;
        do
        {
            pIdex_next = pIdx->next;
            if(pIdx->data != NULL)  {
                free(pIdx->data);
            }
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
pwd_set(char *src, int sz, char *pwd)  {
    return;
}

static int
pwd_check(qbase_pck *pck, char *pwd)    {
    return PACKER_OK;
}

static void
pwd_encrypted(qbase_pck *pck, qbase_byte *data, int sz) {
    return;
}

static qbase_byte *
pwd_translate(qbase_byte *data, int sz, char *pwd) {
    return NULL;
}

/*		basepacker's api implementation		*/
qbase_pck *
qbase_packer_create(char *path, char *pwd, int ver)  {
    qbase_pck *pck = pck_create(path, pwd, ver);
    if(pck != NULL)
        pck_writefile(pck, path);
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
qbase_packer_save(qbase_pck *pck, char *path)  {
    pck_writefile(pck, path);
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
    if(pBlocks != NULL) {
        do  {
            if(pBlocks->current == NULL)    {
                pBlocks = pBlocks->next;
                continue;
            }
            /*  search by key name  */
            if(strcmp(pBlocks->current->name, name) == 0)   {
                *sz = pBlocks->current->data_size;
                data = pBlocks->current->data;
                data = pwd_translate(data, *sz, pwd);
                /*   uncompress   */

            }
            pBlocks = pBlocks->next;
        }while(pBlocks != NULL && pBlocks != pck->blocks[pres]);
    }
    return data;
}

int
qbase_packer_add(qbase_pck *pck, int pres,
                 qbase_byte *bytes, int sz, char *name)  {
    int new_sz;
    qbase_packIndexer *idx = (qbase_packIndexer*)malloc(sizeof(qbase_packIndexer));
    qbase_typeBlocks *bck = (qbase_typeBlocks*)malloc(sizeof(qbase_typeBlocks));
    qbase_byte *cmp_data = NULL;
    if(pck == NULL || bytes == NULL || pres < 0 || name == NULL)    {
        free(idx);
        free(bck);
        return PACKER_ERROR;
    }
    idx->data           = NULL;
    strcpy(idx->name, name);
    bck->current = idx;
    /* get tails of indexer */
    if(pck->indexer == NULL)    {
         pck->indexer = idx;
         idx->next = NULL;
         idx->prev = NULL;
    }
    else    {
        if(pck->indexer->prev == NULL)  {
            pck->indexer->prev = (pck->indexer->next = idx);
            idx->prev = (idx->next = pck->indexer);
        }
        else    {
            pck->indexer->prev->next = idx;
            idx->prev = pck->indexer->prev;
            idx->next = pck->indexer;
            pck->indexer->prev = idx;
        }
    }
    if(pck->blocks[pres] == NULL)  {
        pck->blocks[pres] = bck;
        bck->current = idx;
        bck->next = NULL;
        bck->prev = NULL;
    }
    else    {
        if(pck->blocks[pres]->prev == NULL)    {
            pck->blocks[pres]->prev = pck->blocks[pres]->next = bck;
            bck->prev = bck->next = pck->blocks[pres];
        }
        else    {
            pck->blocks[pres]->prev->next = bck;
            bck->prev = pck->blocks[pres]->prev;
            pck->blocks[pres]->prev = bck;
            bck->next = pck->blocks[pres];
        }
    }
    /*  compress the data   */
    new_sz = sz;
    compress((Bytef*)cmp_data, (uLongf*)&new_sz, (Bytef*)bytes, (uLongf)sz);
    pwd_encrypted(pck, cmp_data, new_sz);
    idx->data = cmp_data;
    idx->data_size = new_sz;

    return PACKER_OK;
}

int
qbase_packer_setpwd(qbase_pck *pck, char *oldPwd, char *pwd)  {
    qbase_byte *decryptData = NULL;
    qbase_packIndexer *idx = NULL;
    if(pwd_check(pck, oldPwd) == 1) {
        /*  reset the pwd in bytes  */
        idx = pck->indexer;
        if(idx == NULL)
            return PACKER_ACSERR;
        if(idx != NULL) {
            do  {
                decryptData = pwd_translate(idx->data, idx->data_size, oldPwd);
                if(decryptData != NULL) {
                    pwd_set(decryptData, idx->data_size, pwd);
                }
                idx = idx->next;
            }while(idx != pck->indexer && idx != NULL);
        }
        /*  reset pwd   */
        pwd_set(&pck->encrypt_token[0], 20, pwd);
        return PACKER_OK;
    }
    return PACKER_ERROR;
}

int
qbase_packer_update(qbase_pck *pck, int pres, char *name,
                    qbase_byte *newbytes, int sz, char *pwd) {
    int cmp_sz;
    qbase_typeBlocks *pb = pck->blocks[pres];
    qbase_byte *cmp_data = NULL;
    if(pwd_check(pck, pwd) == 1)    {
        if(pb == NULL)  {
            return PACKER_ERROR;
        }
        do
        {
            if(strcmp(pb->current->name, name) == 0)    {
                free(pb->current->data);
                cmp_data = (qbase_byte*)malloc(sz);
                compress((Bytef*)cmp_data, (uLongf*)&cmp_sz, (Bytef*)newbytes, (uLong)sz);
                pwd_encrypted(pck, cmp_data, cmp_sz);
                pb->current->data = cmp_data;
                pb->current->data_size = cmp_sz;
                break;
            }
            pb = pb->next;
        }while(pb != NULL && pb != pck->blocks[pres]);
    }
    return PACKER_ERROR;
}

int
qbase_packer_remove(qbase_pck *pck, int pres,
                    char *name ,char *pwd)   {
    qbase_typeBlocks *bck = NULL;
    if(pwd_check(pck, pwd) == 1)    {
        bck = pck->blocks[pres];
        if(bck == NULL)
            return PACKER_ACSERR;
        do
        {
            if(strcmp(bck->current->name, name) == 0)   {
                /* free data    */
                free(bck->current->data);
                /*  free index*/
                if(bck->current->prev != bck->current->next)    {
                    bck->current->prev->next = bck->current->next;
                    bck->current->next->prev = bck->current->prev;
                }
                else    {
                    bck->current->next->next = NULL;
                    bck->current->next->prev = NULL;
                }
                free(bck->current);
                /*   free blcok   */
                if(bck->prev != bck->next)    {
                    bck->prev->next = bck->next;
                    bck->next->prev = bck->prev;
                }
                else    {
                    bck->next->next = NULL;
                    bck->next->prev = NULL;
                }
                free(bck);
                break;
            }
            bck = bck->next;
        }while(bck != NULL && bck != pck->blocks[pres]);
    }
    return PACKER_ERROR;
}
