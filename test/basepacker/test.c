#include "basepacker.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

/*********************************************/

void encrypt_function(char *dest, size_t sz, const char *src)   {
    int i = 0;
    int srcLen;
    if(src == NULL) {
        for(i = 0; i < sz; i++) {
            dest[i] ^=0xd1;
        }
    }
    else    {
        srcLen = strlen(src);
        for(i = 0; i < sz; i++) {
            dest[i] += 1;
        }
    }
}
void decrypt_function(char *dest, size_t sz, const char *src)   {
    int i = 0;
    int srcLen;
    if(src == NULL) {
        for(i = 0; i < sz; i++) {
            dest[i] ^=0xd1;
        }
    }
    else    {
        srcLen = strlen(src);
        for(i = 0; i < sz; i++) {
            dest[i] -= 1;
        }
    }
}

/*********************************************/
static char *opath = "lh.jpg";
static char *rpath = "lh-release.jpg";

int test_compress()	{
	FILE *f = fopen(opath, "rb");
	qbase_pdata data;
	qbase_pck *pck = NULL;
	int len;
	uchar *buffer = NULL;
	if(f != NULL)   {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (uchar*)malloc(len);
        fread(buffer, sizeof(char), len, f);
        data.pdata = buffer;
        data.sz = len;
        pck = qbase_packer_create("test.pck");
        qbase_packer_add(pck, RES_DATA, opath, &data);
        qbase_packer_save(pck, "test.pck");
        qbase_packer_free(pck);
        return 1;
	}
	return 0;
}

int test_uncompress()	{
    qbase_pck *pck = NULL;
    qbase_pdata *pdata = NULL;
    FILE *f = NULL;

    pck = qbase_packer_load("test.pck");
    pdata = qbase_packer_get(pck, RES_DATA, opath, NULL);
    if(pdata == NULL)
        return 0;
    f = fopen(rpath, "wb");
    if(f != NULL)   {
        fwrite(pdata->pdata, sizeof(char), pdata->sz, f);
        fclose(f);
        return 1;
    }
	else return 0;
}

int test_setpwd()	{
    qbase_pck *pck = NULL;
    FILE *f = NULL;
    int len;
    qbase_pdata *dt1 = NULL, *dt2 = NULL;
    uchar *buffer = NULL;
    pck = qbase_packer_create("test_pwd.pck");
    if(qbase_packer_setsercurity(pck, encrypt_function, decrypt_function) != PACKER_FN_OK)
        return 0;
    if(qbase_packer_setpwd(pck, (uchar*)"123321", NULL) != PACKER_FN_OK)
        return 0;

    f = fopen("test.dat", "rb");
    if(f == NULL) return 0;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (uchar*)malloc(len);
    fread(buffer, sizeof(uchar), len, f);
    dt1 = (qbase_pdata*)malloc(sizeof(qbase_pdata));
    dt1->pdata = buffer;
    dt1->sz = len;
    fclose(f);
    qbase_packer_add(pck, RES_DATA, "test.dat", dt1);

    f = fopen("test-release-pwd.dat", "wb");
    dt2 = qbase_packer_get(pck, RES_DATA, "test.dat", (uchar*)"123321");
    fwrite(dt2->pdata, sizeof(char), dt2->sz, f);
    fclose(f);
	return 1;
}

int test_update()	{
	return 1;
}

int test_remove(){
	return 1;
}

int main()
{
//    printf("test compress:%s\n", test_compress()==1? "success": "failure");
//    printf("test uncompress:%s\n", test_uncompress()==1? "success": "failure");
    printf("test setpwd:%s\n", test_setpwd()==1? "success": "failure");
    getchar();
	return 0;
}
