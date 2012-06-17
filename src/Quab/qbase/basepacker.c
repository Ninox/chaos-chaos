#include "basepacker.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <zlib.h>

#define MAX_FILE_COUNT 65536

typedef struct qbase_datainfo	{
    char fname[20];
	unsigned short hash_id;
	size_t old_sz;
	qbase_pdata data;
	unsigned char resId;
} qbase_datainfo;

typedef struct qbase_resblock	{
	qbase_datainfo *current;
	struct qbase_resblock *next;
} qbase_resblock;

struct qbase_pck	{
	char pwd[20];
	char prefix[10];
	qbase_securityfn encrypt;
	qbase_securityfn decrypt;
	unsigned short version;
	size_t datasize;
	size_t count;
	qbase_resblock *blocks[5];
	qbase_datainfo *hash_info[MAX_FILE_COUNT];
};

/*****		helper functions		*****/
static qbase_pck *
pck_create(char *path) {
    qbase_pck *pck = NULL;
    int i;
    FILE *f = fopen(path, "r");
    if(f != NULL)   {
        fclose(f);
        return qbase_packer_load(path);
    }
    pck = (qbase_pck*)malloc(sizeof(qbase_pck));
    pck->datasize = 0;
    pck->count = 0;
    pck->version = PCK_CUR_VER;
    pck->encrypt = NULL;
    pck->decrypt = NULL;
    memset(pck->blocks, 0, 5);
    memset(pck->hash_info, 0, MAX_FILE_COUNT);

    memset(pck->pwd, 0, 20);
    strcpy((char*)pck->prefix, "QUAB_PACK");

    for(i = 0; i < 5; i++)  {
        pck->blocks[i] = NULL;
    }
    return pck;
}

static qbase_pck *
pck_load(char *path)    {
    int read_sz = 0;
    int resid, osz, csz, hashId;
    char fname[20];
    char *buffer  = NULL;
    qbase_datainfo *info = NULL;
    qbase_resblock *bck = NULL;
    qbase_pck *pck = (qbase_pck*)malloc(sizeof(qbase_pck));
    FILE *f = fopen(path, "rb");

    /*      set packer      */
    pck->datasize = 0;
    pck->count = 0;
    pck->decrypt = NULL;
    pck->encrypt = NULL;

    /*      if file exists, get it into binary      */
    if(f != NULL)   {
        fread(pck->prefix, sizeof(char), 10, f);
        fread(&pck->version, sizeof(int), 1, f);
        fread(pck->pwd, sizeof(char), 20, f);
        fread(&pck->count, sizeof(int), 1, f);
        fread(&pck->datasize, sizeof(int), 1, f);

        if(pck->datasize <= 0)  {
            free(pck);
            return NULL;
        }
        while(read_sz < pck->datasize)  {
            fread(&resid, sizeof(char), 1, f);
            fread(fname, sizeof(char), 20, f);
            fread(&hashId, sizeof(unsigned short), 1, f);
            fread(&osz, sizeof(int), 1, f);
            fread(&csz, sizeof(int), 1, f);
            buffer = (char*)malloc(csz);
            fread(buffer, sizeof(char), csz,f);

            info = (qbase_datainfo*)malloc(sizeof(qbase_datainfo));
            info->hash_id = hashId;
            strcpy(info->fname, fname);
            info->old_sz = osz;
            info->resId = resid;
            info->data.pdata = buffer;
            info->data.sz = csz;

            /*      create block data       */
            bck = (qbase_resblock*)malloc(sizeof(qbase_resblock));
            bck->current = info;
            bck->next = NULL;
            if(pck->blocks[resid] == NULL)  {
                pck->blocks[resid] = bck;
            }
            else    {
                bck->next = pck->blocks[resid];
                pck->blocks[resid] = bck;
            }
            /*      create hash data        */
            pck->hash_info[info->hash_id] = info;
            /*      increase the read_sz    */
            read_sz += info->data.sz;
        }
        return pck;
    }
    else    {
        qbase_packer_free(pck);
        return NULL;
    }
}

static int
pck_writefile(qbase_pck *pck, const char *path)   {
    int i;
    qbase_resblock *bck = NULL;
    FILE *f = fopen(path, "wb");
    if(f != NULL)   {
        if(pck != NULL) {
            /*     write fixed fields    */
            fwrite(pck->prefix, sizeof(char), 10, f);
            fwrite(&pck->version, sizeof(int), 1, f);
            fwrite(pck->pwd, sizeof(char), 20, f);
            fwrite(&pck->count, sizeof(int), 1, f);
            fwrite(&pck->datasize, sizeof(int), 1, f);

            /*      write data field    */
            for(i = 0; i < 5; i++)  {
                bck = pck->blocks[i];
                while(bck != NULL)  {
                    fwrite(&bck->current->resId, sizeof(int), 1, f);
                    fwrite(bck->current->fname, sizeof(char), 20, f);
                    fwrite(&bck->current->hash_id, sizeof(int), 1, f);
                    fwrite(&bck->current->old_sz, sizeof(int), 1, f);
                    fwrite(&bck->current->data.sz, sizeof(int), 1, f);
                    fwrite(bck->current->data.pdata, sizeof(char), bck->current->data.sz, f);

                    bck = bck->next;
                }
            }
            fclose(f);
            return PACKER_FN_OK;
        }
        else    {
            fclose(f);
            return PACKER_FN_ERROR;
        }
    }
    return PACKER_FN_NOTEXSIT;
}

static void
pck_freeobj(qbase_pck *pck) {
    int i;
    pck->decrypt = NULL;
    pck->encrypt = NULL;
    pck->datasize = 0;
    pck->count = 0;
    memset(pck->pwd, 0, 20);
    memset(pck->prefix, 0, 10);

    qbase_resblock *pq = NULL;

    for(i = 0; i < 5; i++)  {
        pq = pck->blocks[i];
        while(pq != NULL)   {
            pck->blocks[i] = pq->next;
            free(pq);
            pq = pck->blocks[i];
        }
    }
    for(i = 0; i < MAX_FILE_COUNT; i++) {
        if(pck->hash_info[i] != NULL)   {
            free(pck->hash_info[i]->data.pdata);
            free(pck->hash_info[i]);
            pck->hash_info[i] = NULL;
        }
    }
    free(pck);
}

static int
pck_checkpwd(qbase_pck *pck, const char *pwd)   {
    return PACKER_FN_ERROR;
}

/******		API implementation		******/

/*		packer create or load API		*/
qbase_pck*
qbase_packer_create(char *path)	{
    return pck_create(path);
}

qbase_pck*
qbase_packer_load(char *path)	{
	return pck_load(path);
}

void
qbase_packer_save(qbase_pck *pck, const char *path)    {
    if(pck != NULL)
        pck_writefile(pck, path);
}

void
qbase_packer_free(qbase_pck *pck)	{
    if(pck != NULL) {
        pck_freeobj(pck);
    }
}

unsigned short
hash_gethashid(qbase_pck *pck, unsigned char resId, const char *name)    {
    return 1;
}

/*		packer security API		*/
int
qbase_packer_setsercurity(qbase_pck *pck, qbase_securityfn encryptfn, qbase_securityfn decryptfn)	{
    if(pck->pwd[0] == '\0') {
        pck->encrypt = encryptfn;
        pck->decrypt = decryptfn;
        return PACKER_FN_OK;
    }
	else return PACKER_FN_DENY;
}

int
qbase_packer_setpwd(qbase_pck *pck, char *npwd, const char *opwd)	{
    int len;
    char *encrypt_pwd = NULL;
    if(pck->encrypt == NULL || pck->decrypt == NULL)    {
        return PACKER_FN_NOTEXSIT;
    }
    if(opwd == NULL && pck->pwd[0] == '\0') {
        len = strlen(npwd)+1;
        encrypt_pwd = (char*)malloc(len);
        pck->encrypt(encrypt_pwd, len, npwd);
        strcpy((char*)pck->pwd, encrypt_pwd);
        free(encrypt_pwd);
        return PACKER_FN_OK;
    }
    else    {
        if(pck->pwd[0] != '\0' && opwd != NULL) {
            len = strlen(opwd)+1;
            encrypt_pwd = (char*)malloc(len);
            pck->encrypt(encrypt_pwd, len, opwd);
            if(strcmp(encrypt_pwd, pck->pwd) == 0)  {
                free(encrypt_pwd);
                len = strlen(npwd)+1;
                encrypt_pwd = (char*)malloc(len);
                pck->encrypt(encrypt_pwd, len, npwd);
                memset(pck->pwd, 0, 20);
                strcpy((char*)pck->pwd, encrypt_pwd);
                free(encrypt_pwd);
                return PACKER_FN_OK;
            }
            free(encrypt_pwd);
            return PACKER_FN_DENY;
        }
        else return PACKER_FN_DENY;
    }
	return PACKER_FN_ERROR;
}

/*		packer file lists API*/
qbase_resinfo*
qbase_packer_show(qbase_pck *pck, int resid)	{
	return NULL;
}

/*		pack common CRUD operations		*/
qbase_pdata*
qbase_packer_get(qbase_pck *pck, int resid, char *fname, char *pwd)	{
    unsigned short hashID;
    int len;
    qbase_pdata *ret = NULL;
    qbase_datainfo *data = NULL;
    char *buffer = NULL;

    if(pck == NULL)
        return NULL;
    if(pck_checkpwd(pck, pwd) != PACKER_FN_OK)
        return NULL;

    hashID = hash_gethashid(pck, resid, fname);
    if(hashID != -1)    {
        data = pck->hash_info[hashID];
        buffer = (char*)malloc(data->old_sz);
        memset(buffer, 0, data->old_sz);
        len = data->old_sz;
        uncompress((Bytef*)buffer, (uLongf*)&len, (Bytef*)data->data.pdata, data->old_sz);
        ret = (qbase_pdata*)malloc(sizeof(qbase_pdata));
        ret->pdata = buffer;
        ret->sz = len;
        return ret;
    }
    else    {
        return NULL;
    }
}

int
qbase_packer_add(qbase_pck *pck, int resid, char *fname, qbase_pdata *data)	{
	return PACKER_FN_ERROR;
}

int
qbase_packer_remove(qbase_pck *pck, int resid, char *fname)	{
	return PACKER_FN_ERROR;
}

int
qbase_packer_update(qbase_pck *pck, int resid, char *fname, qbase_pdata *data, char *pwd)	{
	return PACKER_FN_ERROR;
}
