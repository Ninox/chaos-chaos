#ifndef QBASE_PACKER_H
#define QBASE_PACKER_H

#include "basedef.h"
#include <stddef.h>

#define PACKER_FN_OK					0
#define PACKER_FN_ERROR			1
#define PACKER_FN_DENY				2
#define PACKER_FN_NOTEXSIT	3

enum pck_version	{
	PCK_CUR_VER 		= 0x0100,
	PCK_VER_0100		= 0x0100
};
enum pck_resource	{
    RES_TOTAL_COUNT =5,
	RES_ROOT		=0,
	RES_IMG			=1,
	RES_MODEL	    =2,
	RES_SCRIPT	    =3,
	RES_DATA		=4
};

typedef struct qbase_resinfo{
	uchar **filename;
	size_t count;
} qbase_resinfo;

typedef struct qbase_pdata	{
	uchar *pdata;
	size_t sz;
} qbase_pdata;

typedef struct qbase_pck qbase_pck;

/*   use src to encryt or decrypt the dest   */
typedef void (*qbase_securityfn)(uchar *dest, int size, const uchar *src);

/*		packer create or load API		*/
qbase_pck* qbase_packer_create(uchar *path);
qbase_pck* qbase_packer_load(uchar *path);
void qbase_packer_save(qbase_pck *pck, const uchar *path);
void qbase_packer_free(qbase_pck *pck);

/*		packer security API		*/
int qbase_packer_setsercurity(qbase_pck *pck, qbase_securityfn encryptfn, qbase_securityfn decryptfn);
int qbase_packer_setpwd(qbase_pck *pck, uchar *npwd, const uchar *opwd);

/*		packer file lists API*/
qbase_resinfo* qbase_packer_show(qbase_pck *pck, int resid);

/*		pack common CRUD operations		*/
qbase_pdata* qbase_packer_get(qbase_pck *pck, int resid, uchar *fname, uchar *pwd);
int qbase_packer_add(qbase_pck *pck, int resid, uchar *fname, qbase_pdata *data);
int qbase_packer_remove(qbase_pck *pck, int resid, uchar *fname);
int qbase_packer_update(qbase_pck *pck, int resid, uchar *fname, qbase_pdata *data, uchar *pwd);

#endif
