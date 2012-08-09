#include "basepacker.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <zlib.h>

#define MAX_FILE_COUNT 65536

typedef struct qbase_datainfo	{
    char fname[20];
	ushort hash_id;
	size_t old_sz;
	qbase_pdata data;
	uchar resId;
} qbase_datainfo;

typedef struct qbase_resblock	{
	qbase_datainfo *current;
	struct qbase_resblock *next;
} qbase_resblock;

struct qbase_pck	{
	uchar pwd[20];
	char prefix[10];
	qbase_securityfn encrypt;
	qbase_securityfn decrypt;
	ushort version;
	size_t datasize;
	size_t count;
	qbase_resblock *blocks[5];
	qbase_datainfo *hash_info[MAX_FILE_COUNT+1];
};

/*****		helper functions		*****/
static int
pwdcmp(const uchar *s1, const uchar *s2, size_t sz)   {
    int i;
    for(i = 0; i < sz; i++) {
        if(s1[i] != s2[i])
            return 1;
    }
    return 0;
}


static qbase_pck *
pck_create(const char *path) {
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

    memset(pck->pwd, 0, 20);
    strcpy((char*)pck->prefix, "QUAB_PACK");

    for(i = 0; i < MAX_FILE_COUNT+1; i++)  {
        if(i < 5)
            pck->blocks[i] = NULL;
        pck->hash_info[i] = NULL;
    }
    return pck;
}

static qbase_pck *
pck_load(const char *path)    {
    int read_sz = 0;
    int i;
    uchar resid;
    int osz, csz;
    ushort hashId;
    char fname[20];
    uchar *buffer  = NULL;
    qbase_datainfo *info = NULL;
    qbase_resblock *bck = NULL;
    qbase_pck *pck = (qbase_pck*)malloc(sizeof(qbase_pck));
    FILE *f = fopen(path, "rb");

    /*      set packer      */
    pck->datasize = 0;
    pck->count = 0;
    pck->decrypt = NULL;
    pck->encrypt = NULL;
    for(i = 0; i < MAX_FILE_COUNT+1; i++) {
        if(i < 5)
            pck->blocks[i] = NULL;
        pck->hash_info[i] = NULL;
    }

    /*      if file exists, get it into binary      */
    if(f != NULL)   {
        fread(pck->prefix, sizeof(char), 10, f);
        fread(&pck->version, sizeof(short), 1, f);
        fread(pck->pwd, sizeof(uchar), 20, f);
        fread(&pck->count, sizeof(int), 1, f);
        fread(&pck->datasize, sizeof(int), 1, f);

        if(pck->datasize <= 0)  {
            free(pck);
            return NULL;
        }
        while(read_sz < pck->datasize)  {
            fread(&resid, sizeof(uchar), 1, f);
            fread(fname, sizeof(char), 20, f);
            fread(&hashId, sizeof(ushort), 1, f);
            fread(&osz, sizeof(int), 1, f);
            fread(&csz, sizeof(int), 1, f);
            buffer = (uchar*)malloc(csz);
            fread(buffer, sizeof(uchar), csz,f);

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
        fclose(f);
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
            fwrite(&pck->version, sizeof(short), 1, f);
            fwrite(pck->pwd, sizeof(uchar), 20, f);
            fwrite(&pck->count, sizeof(int), 1, f);
            fwrite(&pck->datasize, sizeof(int), 1, f);

            /*      write data field    */
            for(i = 0; i < 5; i++)  {
                bck = pck->blocks[i];
                while(bck != NULL)  {
                    fwrite(&bck->current->resId, sizeof(uchar), 1, f);
                    fwrite(bck->current->fname, sizeof(char), 20, f);
                    fwrite(&bck->current->hash_id, sizeof(short), 1, f);
                    fwrite(&bck->current->old_sz, sizeof(int), 1, f);
                    fwrite(&bck->current->data.sz, sizeof(int), 1, f);
                    fwrite(bck->current->data.pdata, sizeof(uchar), bck->current->data.sz, f);

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
    qbase_resblock *pq = NULL;
    pck->decrypt = NULL;
    pck->encrypt = NULL;
    pck->datasize = 0;
    pck->count = 0;
    memset(pck->pwd, 0, 20);
    memset(pck->prefix, 0, 10);

    for(i = 0; i < MAX_FILE_COUNT+1; i++) {
        if(i < 5)   {
            pq = pck->blocks[i];
            while(pq != NULL)   {
                pck->blocks[i] = pq->next;
                free(pq);
                pq =  pck->blocks[i];
            }
        }
        if(pck->hash_info[i] != NULL)   {
            free(pck->hash_info[i]->data.pdata);
            free(pck->hash_info[i]);
            pck->hash_info[i] = NULL;
        }
    }
    free(pck);
}

static int
pck_checkpwd(qbase_pck *pck, const uchar *pwd)   {
    uchar t_pwd[20];
    memset(t_pwd, 0, 20);
    /*   if no encrypt/decrypt in pck, this function will return OK    */
    if(pck->encrypt != NULL && pck->decrypt != NULL)    {
        strcpy((char*)t_pwd, (char*)pwd);
        pck->encrypt(t_pwd, 20, NULL);
        if(pwdcmp(t_pwd, pck->pwd, 20) == 0)
            return PACKER_FN_OK;
        else
            return PACKER_FN_ERROR;
    }
    else return PACKER_FN_OK;
}

static int
pck_checksame(qbase_pck *pck, int resid, const char *name)   {
    qbase_resblock *bck = NULL;
	if(resid >= RES_TOTAL_COUNT)
		return PACKER_FN_ERROR;
    if(pck->blocks == NULL)
        return PACKER_FN_ERROR;
    bck = pck->blocks[resid];
    while(bck != NULL)  {
        if(strcmp(bck->current->fname, name) == 0)
            return PACKER_FN_DENY;
        else bck = bck->next;
    }
    return PACKER_FN_OK;
}

static uchar
hash_calelement(qbase_pck *pck, uchar resId, const char *name,
                uchar *mid_ch, uchar *min_ch, uchar *max_ch) {
    int len = strlen(name), i;
	*mid_ch = 0, *max_ch = 0, *min_ch = 255;   

    *mid_ch = name[len/2];
    for(i = 0; i < len; i++)    {
        if((uchar)name[i] >= *max_ch)
            *max_ch = (uchar)name[i];
        else if((uchar)name[i] <= *min_ch)
            *min_ch = (uchar)name[i];
    }
    return (*mid_ch + *max_ch + *min_ch)/3;
}

static ushort
hash_createid(qbase_pck *pck, uchar resId, const char *name)    {
    uchar mid_ch, max_ch = 0, min_ch = 255, hashCode;
    ushort idx, i;
    hashCode = hash_calelement(pck, resId, name, &mid_ch, &min_ch, &max_ch);
    if(hashCode == 0)
        hashCode = 1;
    if(pck->hash_info[hashCode] == NULL)
        return hashCode;
    else    {
        idx = (int)hashCode;
        /*   start from 1 to MAX_FILE_COUNT   */
        for(i = 1; i <= MAX_FILE_COUNT; i++) {
            if(idx == 0)    {
                idx++;
                i--;
                continue;
            }
            if(pck->hash_info[idx] == NULL)
                return idx;
            else idx = (idx+1)%(MAX_FILE_COUNT+1);
        }
        return 0;
    }
}

static ushort
hash_gethashid(qbase_pck *pck, uchar resId, const char *name)   {
    uchar mid_ch, max_ch = 0, min_ch = 255, hashCode;
    ushort idx, i;
    hashCode = hash_calelement(pck, resId, name, &mid_ch, &min_ch, &max_ch);
    idx = (int)hashCode;
    /*      start from 1 to MAX_FILE_COUNT      */
    for(i = 1; i <= MAX_FILE_COUNT; i++) {
        if(idx == 0)    {
            idx++;
            i--;
            continue;
        }
        if(pck->hash_info[idx] == NULL)
            return 0;
        if(strcmp(pck->hash_info[idx]->fname, name) == 0 && pck->hash_info[idx]->resId == resId)
            return idx;
        else idx = (idx+1)%(MAX_FILE_COUNT+1);
    }
    return 0;
}

/******		API implementation		******/

/*		packer create or load API		*/
qbase_pck*
qbase_packer_create(const char *path)	{
    return pck_create(path);
}

qbase_pck*
qbase_packer_load(const char *path)	{
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

/*		packer security API		*/
int
qbase_packer_setsercurity(qbase_pck *pck, qbase_securityfn encryptfn, qbase_securityfn decryptfn)	{
    if(pck == NULL)
        return PACKER_FN_ERROR;
    if(pck->pwd[0] == '\0') {
        pck->encrypt = encryptfn;
        pck->decrypt = decryptfn;
        return PACKER_FN_OK;
    }
	else return PACKER_FN_DENY;
}

int
qbase_packer_setpwd(qbase_pck *pck, uchar *npwd, const uchar *opwd)	{
    uchar encrypt_pwd[20];
    if(pck == NULL)
        return PACKER_FN_ERROR;
    if(pck->encrypt == NULL || pck->decrypt == NULL)    {
        return PACKER_FN_NOTEXSIT;
    }
    if(opwd == NULL && pck->pwd[0] == '\0') {
        memset(encrypt_pwd, 0, 20);
        strcpy((char*)encrypt_pwd, (char*)npwd);
        pck->encrypt(encrypt_pwd, 20, NULL);
        strcpy((char*)pck->pwd, (char*)encrypt_pwd);
        memset(encrypt_pwd, 0, 20);
        return PACKER_FN_OK;
    }
    else    {
        if(pck->pwd[0] != '\0' && opwd != NULL) {
            memset(encrypt_pwd, 0, 20);
            strcpy((char*)encrypt_pwd, (char*)opwd);
            pck->encrypt(encrypt_pwd, 20, NULL);
            if(pwdcmp(encrypt_pwd, pck->pwd, 20) == 0)  {
                memset(encrypt_pwd, 0, 20);
                strcpy((char*)encrypt_pwd, (char*)npwd);
                pck->encrypt(encrypt_pwd, 20, NULL);
                memset(pck->pwd, 0, 20);
                strcpy((char*)pck->pwd, (char*)encrypt_pwd);
                return PACKER_FN_OK;
            }
            return PACKER_FN_DENY;
        }
        else return PACKER_FN_DENY;
    }
	return PACKER_FN_ERROR;
}

/*		packer file lists API*/
qbase_resinfo*
qbase_packer_show(qbase_pck *pck, int resid)	{
    qbase_resblock *bck = NULL;
    qbase_resinfo *info = NULL;
    int i = 0;
    int counter = 0;
    if(pck == NULL)
        return NULL;
    if(resid >= RES_TOTAL_COUNT)
        return NULL;
    if(pck == NULL)
        return NULL;
    bck = pck->blocks[resid];
    if(bck == NULL)
        return NULL;

    while(bck != NULL)  {
        counter++;
        bck = bck->next;
    }
    info = (qbase_resinfo*)malloc(sizeof(qbase_resinfo));
    info->count = counter;
    info->filename = (char**)malloc(counter);
    /*     scan all file     */
    bck = pck->blocks[resid];
    while(bck != NULL)  {
        info->filename[i] = bck->current->fname;
        i++;
        bck = bck->next;
    }
	return info;
}

/*		pack common CRUD operations		*/
qbase_pdata*
qbase_packer_get(qbase_pck *pck, int resid,
                 const char *fname, const uchar *pwd)	{
    ushort hashID;
    int len;
    qbase_pdata *ret = NULL;
    qbase_datainfo *data = NULL;
    uchar *buffer = NULL, *e_data = NULL;

    if(pck == NULL)
        return NULL;
	if(resid >= RES_TOTAL_COUNT)
		return NULL;
    if(pck_checkpwd(pck, pwd) != PACKER_FN_OK)
        return NULL;

    hashID = hash_gethashid(pck, resid, fname);
    if(hashID != 0)    {
        data = pck->hash_info[hashID];
        buffer = (uchar*)malloc(data->old_sz);
        memset(buffer, 0, data->old_sz);
        len = data->old_sz;
        if(pck->encrypt != NULL && pck->decrypt != NULL && pck->pwd[0] != '\0')  {
            e_data = (uchar*)malloc(data->data.sz);
            memcpy(e_data, data->data.pdata, data->data.sz);
            pck->decrypt(e_data, data->data.sz, pwd);
            uncompress((Bytef*)buffer, (uLongf*)&len, (Bytef*)e_data, data->data.sz);
            free(e_data);
        }
        else uncompress((Bytef*)buffer, (uLongf*)&len, (Bytef*)data->data.pdata, data->old_sz);
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
qbase_packer_add(qbase_pck *pck, int resid,
                 const char *fname, qbase_pdata *data)	{
    ushort hashID =0;
    uchar *buffer = NULL;
    int len;
    uchar pwd[20];
    qbase_datainfo *info = NULL;
    qbase_resblock *bck = NULL;

    if(data == NULL)
        return PACKER_FN_ERROR;
    if(pck == NULL)
        return PACKER_FN_ERROR;
	if(resid >= RES_TOTAL_COUNT)
		return PACKER_FN_ERROR;
    if(pck_checksame(pck, resid, fname) != PACKER_FN_OK)    {
        return PACKER_FN_DENY;
    }

    hashID = hash_createid(pck, resid, fname);
    if(hashID < 0)
        return PACKER_FN_DENY;

    info = (qbase_datainfo*)malloc(sizeof(qbase_datainfo));

    /*   set datainfo   */
    strcpy(info->fname, fname);
    info->hash_id = hashID;
    info->old_sz = data->sz;
    info->resId = resid;

    /*   compress data and set into datainfo    */
    buffer = (uchar*)malloc(data->sz);
    len = data->sz;
    /*   if compress fail   */
    if(compress((Bytef*)buffer, (uLongf*)&len, (Bytef*)data->pdata, (uLong)data->sz) != Z_OK)   {
        free(buffer);
        free(info->fname);
        free(info);
        return PACKER_FN_ERROR;
    }
    info->data.sz = len;
    /*   check status of pck for encrypting the buffer data   */
    if(pck->decrypt != NULL && pck->encrypt != NULL)    {
        memcpy(pwd, pck->pwd, 20);
        pck->decrypt(pwd, 20, NULL);
        pck->encrypt(buffer, info->data.sz, pwd);
    }
    info->data.pdata = buffer;

    /*   set datainfo into hash   */
    pck->hash_info[info->hash_id] = info;
    /*   set into blocks and update item count of pck   */
    bck = (qbase_resblock*)malloc(sizeof(qbase_resblock));
    bck->current = info;
    bck->next = pck->blocks[resid];
    pck->blocks[resid] = bck;

    pck->datasize += info->data.sz;
    pck->count++;

	return PACKER_FN_OK;
}

int
qbase_packer_remove(qbase_pck *pck, int resid,
                    const char *fname, const uchar *pwd)	{
    ushort hashID = 0;
    qbase_resblock *bck = NULL, *next_bck = NULL;

    if(pck == NULL)
        return PACKER_FN_ERROR;
	if(resid >= RES_TOTAL_COUNT)
		return PACKER_FN_ERROR;
    if(pck_checkpwd(pck, pwd) != PACKER_FN_OK)
        return PACKER_FN_DENY;
    hashID = hash_gethashid(pck, resid, fname);
    if(hashID == 0)
        return PACKER_FN_NOTEXSIT;
    /*   remove from blocks   */
    bck = pck->blocks[resid];
    if(bck != NULL) {
        if(strcmp(bck->current->fname, fname) == 0)  {
            pck->blocks[resid] = bck->next;
            pck->datasize -= bck->current->data.sz;
            pck->count --;
            free(bck);
        }
        else    {
            while(bck->next != NULL)    {
                /*  if the next element is not null, get the next pointer   */
                next_bck = bck->next;
                if(strcmp(next_bck->current->fname, fname) == 0)   {
                    pck->datasize -= next_bck->current->data.sz;
                    pck->count --;
                    /*   reset the current pointer's next element(next_bck) to next next elment   */
                    bck->next = next_bck->next;
                    free(next_bck);
                    break;
                }
                else bck = bck->next;
            }
        }
    }
    /*   remove from hash   */
    free(pck->hash_info[hashID]->data.pdata);
    free(pck->hash_info[hashID]);
    pck->hash_info[hashID] = NULL;

	return PACKER_FN_ERROR;
}

int
qbase_packer_update(qbase_pck *pck, int resid,
                    const char *fname, qbase_pdata *data, const uchar *pwd)	{
    int len;
    uchar *buffer = NULL;
    uchar t_pwd[20];
    ushort hashID = 0;

	if(resid >= RES_TOTAL_COUNT)
		return PACKER_FN_ERROR;
    if(pck == NULL)
        return PACKER_FN_ERROR;
    if(data == NULL)
        return PACKER_FN_NOTEXSIT;
    if(data->sz <= 0 || data->pdata == NULL)
        return PACKER_FN_NOTEXSIT;
    if(pck_checkpwd(pck, pwd) != PACKER_FN_OK)  {
        return PACKER_FN_DENY;
    }
    hashID = hash_gethashid(pck, resid, fname);
    if(hashID == 0)
        return PACKER_FN_NOTEXSIT;
    /*   compress data   */
    buffer = (uchar*)malloc(data->sz);
    len = data->sz;
    if(compress((Bytef*)buffer, (uLongf*)&len, (Bytef*)data->pdata, (uLong)data->sz) != Z_OK)    {
        free(buffer);
        return PACKER_FN_ERROR;
    }
    /*   check the packer status for encrypt the data   */
    if(pck->encrypt != NULL && pck->decrypt != NULL)    {
        memcpy(t_pwd, pck->pwd, 20);
        pck->decrypt(t_pwd, 20, NULL);
        pck->encrypt(buffer, len, t_pwd);
    }
    /*   update packer data size   */
    pck->datasize -= pck->hash_info[hashID]->data.sz;
    pck->datasize += len;
    /*   update datainfo   */
    pck->hash_info[hashID]->old_sz = data->sz;
    free(pck->hash_info[hashID]->data.pdata);
    pck->hash_info[hashID]->data.pdata = buffer;
    pck->hash_info[hashID]->data.sz = len;
    memset(pck->hash_info[hashID]->fname, 0, 20);
    strcpy(pck->hash_info[hashID]->fname, fname);
	return PACKER_FN_OK;
}

int
qbase_packer_rename(qbase_pck *pck, int resid,
                    const char *fname, const char *newname, const uchar *pwd) {
    qbase_resblock *bck = NULL;

    if(pck == NULL)
        return PACKER_FN_ERROR;
    if(resid > RES_TOTAL_COUNT)
        return PACKER_FN_ERROR;
    if(pck->blocks[resid] == NULL)
        return PACKER_FN_NOTEXSIT;
    if(pck_checkpwd(pck, pwd) != PACKER_FN_OK)
        return PACKER_FN_DENY;

    bck = pck->blocks[resid];
    while(bck != NULL)  {
        if(strcmp(bck->current->fname, fname) == 0) {
            memset(bck->current->fname, 0, 20);
            strcpy(bck->current->fname, newname);
            return PACKER_FN_OK;
        }
        bck = bck->next;
    }
    return PACKER_FN_NOTEXSIT;
}
