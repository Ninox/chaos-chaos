#include <Quab/qbase/qbase.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <xaloy.h>
using namespace xaloy;

/*********************************************/

void encrypt_function(uchar *dest, int sz, const uchar *src)   {
    int i = 0;
    int srcLen = 0;
    if(src == NULL) {
        for(i = 0; i < sz; i++) {
            dest[i] ^=0xd1;
        }
    }
    else    {
        srcLen = strlen((char*)src);
        for(i = 0; i < sz; i++) {
            dest[i] += 1;
        }
    }
}
void decrypt_function(uchar *dest, int sz, const uchar *src)   {
    int i = 0;
    int srcLen = 0;
    if(src == NULL) {
        for(i = 0; i < sz; i++) {
            dest[i] ^=0xd1;
        }
    }
    else    {
        srcLen = strlen((char*)src);
        for(i = 0; i < sz; i++) {
            dest[i] -= 1;
        }
    }
}

/*********************************************/
static const char *oldFiles[] = {"lh.jpg","gmon.out","test.dat", "noexists.txt"};
static const char *newFiles[] = {"lh-new.jpg","gmon-new.out","test-new.dat", "noexists-new.txt"};
static const char *rpath[] = {"lh-r.jpg", "gmon-r.out", "test-r.dat","noexists-r.txt"};
static const char *ppath[] = {"lh-rp.jpg", "gmon-rp.out", "test-rp.dat","noexists-rp.txt"};

XALOY_TEST_MODULE(compressTest)
{
	FILE *f;
	qbase_pdata data;
	qbase_pck *pck = NULL;
	uchar *buffer = NULL;
	int len;	
	
	pck = qbase_packer_create("test.pck");
	XALOY_ASSERT_NULL(XL_NOTNULL, pck);
	
	for(int i = 0; i < 4; i++)	{
		f = fopen(oldFiles[i], "rb");
		if(f == NULL) continue;
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (uchar*)malloc(len);
		fread(buffer, sizeof(char), len, f);
		fclose(f);
		data.pdata = buffer;
		data.sz = len;		
		XALOY_EXPECT(XL_EQUAL, qbase_packer_add(pck, RES_DATA, oldFiles[i], &data), PACKER_FN_OK);
		qbase_packer_save(pck, "test.pck");
		free(buffer);
	}
	qbase_packer_free(pck);
}

XALOY_TEST_MODULE(uncompressTest)
{
    qbase_pck *pck = NULL;
    qbase_pdata *pdata = NULL;
    FILE *f = NULL;

    pck = qbase_packer_load("test.pck");
	XALOY_ASSERT_NULL(XL_NOTNULL, pck);
	
	for(int i = 0; i < 4; i++)	{
		pdata = qbase_packer_get(pck, RES_DATA, oldFiles[i], NULL);
		if(i < 3)	{
			XALOY_ASSERT_NULL(XL_NOTNULL, pdata);
			XALOY_EXPECT_NULL(XL_NOTNULL,pdata->pdata);
			XALOY_EXPECT(XL_GREATER, pdata->sz, 0);
		}
		else	{
			XALOY_ASSERT_NULL(XL_ISNULL, pdata);
			continue;
		}
		if(pdata == NULL)
			continue;
		f = fopen(rpath[i], "wb");
		if(f != NULL)   {
			fwrite(pdata->pdata, sizeof(char), pdata->sz, f);
			fclose(f);			
			free(pdata->pdata);
			free(pdata);
		}
	}
	qbase_packer_free(pck);
}

XALOY_TEST_MODULE(setpwdTest)
{
    qbase_pck *pck = NULL;
    FILE *f = NULL;
    int len;
    qbase_pdata *dt1 = NULL, *dt2 = NULL;
    uchar *buffer = NULL;
    pck = qbase_packer_create("test_pwd.pck");
	
	// check pck is NOT null
	XALOY_ASSERT_NULL(XL_NOTNULL, pck);
	
	// check the setsercurity and setpwd function is ok
	XALOY_ASSERT(XL_EQUAL, qbase_packer_setsercurity(pck, encrypt_function, decrypt_function), PACKER_FN_OK);
	XALOY_ASSERT(XL_EQUAL, qbase_packer_setpwd(pck, (uchar*)"123321", NULL), PACKER_FN_OK);
	
	for(int i = 0; i < 4; i++)	{
		f = fopen(oldFiles[i], "rb");
		if(f == NULL) continue;
		
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (uchar*)malloc(len);
		fread(buffer, sizeof(uchar), len, f);
		dt1 = (qbase_pdata*)malloc(sizeof(qbase_pdata));
		dt1->pdata = buffer;
		dt1->sz = len;
		fclose(f);
		XALOY_EXPECT(XL_EQUAL,qbase_packer_add(pck, RES_DATA, oldFiles[i], dt1), PACKER_FN_OK);

		free(buffer);
		free(dt1);

		f = fopen(ppath[i], "wb");
		dt2 = qbase_packer_get(pck, RES_DATA, oldFiles[i], NULL);
		XALOY_ASSERT_NULL(XL_ISNULL, dt2);
		dt2 = qbase_packer_get(pck, RES_DATA, oldFiles[i], (uchar*)"asd");
		XALOY_ASSERT_NULL(XL_ISNULL, dt2);
		dt2 = qbase_packer_get(pck, RES_DATA, oldFiles[i], (uchar*)"123321");
		XALOY_ASSERT_NULL(XL_NOTNULL, dt2);
		XALOY_EXPECT(XL_GREATER, dt2->sz, 0);
		XALOY_EXPECT_NULL(XL_NOTNULL, dt2->pdata);
		
		fwrite(dt2->pdata, sizeof(char), dt2->sz, f);
		fclose(f);    
		free(dt2->pdata);
		free(dt2);
	}
	qbase_packer_free(pck);
}

XALOY_TEST_MODULE(updateTest)
{
    /*   some error in this function cause by the hashid, because new file is replaced the old file   */
    /*   so, the name parameter in update function is a name of the old file we want to replace  */
    qbase_pck *pck = NULL;
    uchar *buffer = NULL;
    FILE *f = NULL;
    qbase_pdata data;
    qbase_pdata *ret;
    size_t len = 0;

	/*	loop for the files	*/
	f = fopen("gmon.out","rb");
	
	XALOY_ASSERT_NULL(XL_NOTNULL, f);
	
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
	XALOY_ASSERT_NULL(XL_NOTNULL, pck);
	XALOY_EXPECT(XL_EQUAL, qbase_packer_update(pck, RES_DATA, "lh.jpg", &data, NULL), PACKER_FN_OK);
	free(data.pdata);
	/*   check old data is null   */
	ret = qbase_packer_get(pck, RES_DATA, "gmon.out", NULL);
	XALOY_EXPECT_NULL(XL_NOTNULL, ret);
	if(ret == NULL) {
		ret = qbase_packer_get(pck, RES_DATA, "lh.jpg", NULL);
		XALOY_EXPECT_NULL(XL_NOTNULL, ret);
		/*   ret must be null, because it was replaced by the new file   */
		XALOY_ASSERT_NULL(XL_NOTNULL, ret);
		f = fopen("gmon-r.out","wb");
		XALOY_EXPECT_NULL(XL_NOTNULL, ret->pdata);
		XALOY_EXPECT(XL_GREATER, ret->sz, 0);
		fwrite(ret->pdata, sizeof(uchar), ret->sz, f);
		fclose(f);
		free(ret->pdata);
		free(ret);
		qbase_packer_free(pck);
	}
	else    {
		free(ret->pdata);
		free(ret);
		qbase_packer_free(pck);
	}
}

XALOY_TEST_MODULE(removeTest)
{
    qbase_pck *pck = qbase_packer_load("test.pck");
    qbase_resinfo *ri = NULL;
    int i, cnt;
	XALOY_ASSERT_NULL(XL_NOTNULL, pck);
	
    /*  show data befor remove  */
    ri = qbase_packer_show(pck, RES_DATA, &cnt);
	XALOY_ASSERT_NULL(XL_NOTNULL, ri);	
    for(i = 0; i < cnt; i++)  {
        printf("%d:%s[%d]\n", ri[i].hashid, ri[i].filename, ri[i].sz);
    }
    free(ri);
    ri = NULL;	

    /*  show data after remove  */
	for(int i = 0; i < 4; i++)	{
		if(i != 3)	{
			XALOY_EXPECT(XL_EQUAL, qbase_packer_remove(pck, RES_DATA, oldFiles[i], NULL), PACKER_FN_OK);
		}
		else	{
			XALOY_EXPECT(XL_NOT_EQUAL, qbase_packer_remove(pck, RES_DATA, oldFiles[i], NULL), PACKER_FN_OK);
		}
		printf("%s\n", oldFiles[i]);
	}
	ri = qbase_packer_show(pck, RES_DATA, &cnt);
	// all data has deleted. so the resinfo must be null
	XALOY_ASSERT_NULL(XL_ISNULL, ri);
	
    qbase_packer_free(pck);
}

XALOY_TEST_MODULE(renameTest)
{
    qbase_pck *pck = qbase_packer_load("test.pck");
    qbase_resinfo *ri = NULL;
    int i, cnt;
	XALOY_ASSERT_NULL(XL_NOTNULL, pck);
    /* show data before rename  */
    ri = qbase_packer_show(pck, RES_DATA, &cnt);
	XALOY_ASSERT_NULL(XL_NOTNULL, ri);
    for(i = 0; i < cnt; i++)  {
		printf("%s\n", ri[i].filename);
	}    
    free(ri);
    ri = NULL;
	
    /*  show data after rename  */
	for(int i = 0; i < 4; i++)	{
		if(i != 3)	{
			XALOY_EXPECT(XL_EQUAL, qbase_packer_rename(pck, RES_DATA, oldFiles[i], newFiles[i], NULL), PACKER_FN_OK);
		}
		else	{
			XALOY_EXPECT(XL_NOT_EQUAL, qbase_packer_rename(pck, RES_DATA, oldFiles[i], newFiles[i], NULL), PACKER_FN_OK);
		}		
	}
    ri = qbase_packer_show(pck, RES_DATA, &cnt);
	XALOY_ASSERT_NULL(XL_NOTNULL, ri);
    for(i = 0; i < cnt; i++)  {
        printf("%s\n", ri[i].filename);
    }
    free(ri);
    ri = NULL;
	
    qbase_packer_free(pck);
}

int main()
{
	return G_XALOY.execute(true);
}
