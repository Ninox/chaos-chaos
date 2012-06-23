#include "basepacker.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

/*********************************************/

void encrypt_function(char *dest, size_t sz, const char *src)   {
    int i = 0;
    int srcLen = 0;
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
    int srcLen = 0;
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
        fclose(f);
        data.pdata = buffer;
        data.sz = len;
        pck = qbase_packer_create("test.pck");
        qbase_packer_add(pck, RES_DATA, opath, &data);
        qbase_packer_save(pck, "test.pck");
        qbase_packer_free(pck);
        free(buffer);
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
        qbase_packer_free(pck);
        free(pdata->pdata);
        free(pdata);
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
    if(f == NULL)   {
        qbase_packer_free(pck);
        return 0;
    }
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

    free(buffer);
    free(dt1);

    f = fopen("test-release-pwd.dat", "wb");
    dt2 = qbase_packer_get(pck, RES_DATA, "test.dat", (uchar*)"123321");
    fwrite(dt2->pdata, sizeof(char), dt2->sz, f);
    fclose(f);
    qbase_packer_free(pck);
    free(dt2->pdata);
    free(dt2);
	return 1;
}

int test_update()	{
    /*   some error in this function cause by the hashid, because new file is replaced the old file   */
    /*   so, the name parameter in update function is a name of the old file we want to replace  */
    qbase_pck *pck = NULL;
    uchar *buffer = NULL;
    FILE *f = NULL;
    qbase_pdata data;
    qbase_pdata *ret;
    size_t len = 0;

    f = fopen("gmon.out","rb");
    if(f == NULL)   {
        printf("cannot open the new file: gmon.out\n");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (uchar*)malloc(len);
    fread(buffer, sizeof(uchar), len, f);
    fclose(f);

    data.pdata = buffer;
    data.sz = len;

    /*   update and free original data   */
    pck = qbase_packer_load("test.pck");
    qbase_packer_update(pck, RES_DATA, "lh.jpg", &data, NULL);
    free(data.pdata);
    /*   check old data is null   */
    ret = qbase_packer_get(pck, RES_DATA, "gmon.out", NULL);
    if(ret == NULL) {
        ret = qbase_packer_get(pck, RES_DATA, "lh.jpg", NULL);
        /*   ret must be null, because it was replaced by the new file   */
        if(ret == NULL) {
            free(ret->pdata);
            free(ret);
            qbase_packer_free(pck);
            return 0;
        }
        f = fopen("release-gmon.out","wb");
        fwrite(ret->pdata, sizeof(uchar), ret->sz, f);
        fclose(f);
        free(ret->pdata);
        free(ret);
        qbase_packer_free(pck);
        return 1;
    }
    else    {
        free(ret->pdata);
        free(ret);
        qbase_packer_free(pck);
        return 0;
    }
}

int test_remove(){
    qbase_pck *pck = qbase_packer_load("test.pck");
    qbase_resinfo *ri = NULL;
    int i;
    /*  show data befor remove  */
    ri = qbase_packer_show(pck, RES_DATA);
    if(ri != NULL)  {
        for(i = 0; i < ri->count; i++)  {
            printf("%s\n", ri->filename[i]);
        }
        free(ri->filename);
        free(ri);
        ri = NULL;
    }

    /*  show data after remove  */
    qbase_packer_remove(pck, RES_DATA, "lh.jpg", NULL);
    ri = qbase_packer_show(pck, RES_DATA);
    if(ri == NULL)  {
        qbase_packer_free(pck);
        printf("no file!\n");
        return 1;
    }
    free(ri->filename);
    free(ri);
    qbase_packer_free(pck);
	return 0;
}

int test_rename()   {
    qbase_pck *pck = qbase_packer_load("test.pck");
    qbase_resinfo *ri = NULL;
    int i;
    /* show data before rename  */
    ri = qbase_packer_show(pck, RES_DATA);
    if(ri != NULL)  {
        for(i = 0; i < ri->count; i++)  {
            printf("%s\n", ri->filename[i]);
        }
        free(ri->filename);
        free(ri);
        ri = NULL;
    }
    /*  show data after rename  */
    qbase_packer_rename(pck, RES_DATA, "lh.jpg", "lh_newer.jpg", NULL);
    ri = qbase_packer_show(pck, RES_DATA);
    if(ri != NULL)  {
        for(i = 0; i < ri->count; i++)  {
            printf("%s\n", ri->filename[i]);
        }
        free(ri->filename);
        free(ri);
        ri = NULL;
    }
    qbase_packer_free(pck);
    return 1;
}

int main()
{
    int i;
    getchar();
    for(i=0;i<10000;i++)
        test_compress();
    printf("test_compress done, %d\n", i);
//    printf("test compress:%s\n", test_compress()==1? "success": "failure");
    getchar();
    for(i=0;i<10000;i++)
        test_uncompress();
    printf("test_uncompress done\n");
//    printf("test uncompress:%s\n", test_uncompress()==1? "success": "failure");
    getchar();
    for(i=0;i<10000;i++)
        test_setpwd();
    printf("test_setpwd done\n");
//    printf("test setpwd:%s\n", test_setpwd()==1? "success": "failure");
    getchar();
    for(i=0;i<10000;i++)
        test_update();
    printf("test_update done\n");
    getchar();
    for(i=0;i<10000;i++)
        test_rename();
    printf("test_rename done\n");
    getchar();
    for(i=0;i<10000;i++)
        test_remove();
    printf("test_remove done\n");
    getchar();
	return 0;
}
