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

typedef qbase_packIndexer* qbase_packIndexerPointer;

struct qbase_pck    {
    char prefix[8];
    int version;
    int datasize;
    char encrypt_token[21];
    qbase_packIndexerPointer indexer[6];
    qbase_byte *databytes;
};


/*		static helper function defination		*/



/*		basepacker's api implementation		*/
qbase_pck* qbase_packer_create(char *path, char *pwd, int ver)  {
    return NULL;
}

qbase_pck* qbase_packer_load(char *path, char *pwd, int ver)    {
    return NULL;
}

qbase_byte* qbase_packer_get(qbase_pck *pck, int pres, char *name,
                             char *pwd, int ver)  {
    return NULL;
}

int qbase_packer_add(qbase_pck *pck, int pres, qbase_byte *bytes,
                     char *name, int ver)  {
    return PACKER_ERROR;
}

int qbase_packer_setpwd(qbase_pck *pck, char *pwd)  {
    return PACKER_ERROR;
}

int qbase_packer_update(qbase_pck *pck, int pres, char *name,
                        qbase_byte *newbytes, char *pwd, int ver) {
    return PACKER_ERROR;
}

int qbase_packer_remove(qbase_pck *pck, int pres,
                        char *name ,char *pwd, int ver)   {
    return PACKER_ERROR;
}
