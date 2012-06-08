#include "basepacker.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

int test_compress()	{
	const char *path = "test.dat";
	FILE *f = fopen(path, "rb");
	qbase_pck *pck = NULL;
	int len;
	qbase_byte *buffer = NULL;
	if(f != NULL)   {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (qbase_byte*)malloc(len);
        fread(buffer, sizeof(qbase_byte), len, f);
        pck = qbase_packer_create("test.pck",  NULL, PACKVER_100);
        qbase_packer_add(pck, RES_DATA, buffer, len, "test.dat");
        qbase_packer_save(pck, "test.pck");
        qbase_packer_close(pck);
        return 1;
	}
	return 0;
}

int test_uncompress()	{
    const char *path = "test.pck";
    const char *target = "test-release.dat";
    qbase_pck *pck = NULL;
    int len;
    qbase_byte *buffer = NULL;
    FILE *f = NULL;

    pck = qbase_packer_load(path, PACKVER_100);
    buffer = qbase_packer_get(pck, &len, 4, "test.dat", NULL, PACKVER_100);
    f = fopen(target, "wb");
    if(f != NULL)   {
        fwrite(buffer, sizeof(qbase_byte), len, f);
        fclose(f);
        return 1;
    }
	else return 0;
}

int test_setpwd()	{
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
    printf("test uncompress:%s\n", test_uncompress()==1? "success": "failure");
    getchar();
	return 0;
}
