#ifndef	QBASE_PACKER_H
#define QBASE_PACKER_H

/*		version define		*/
enum qbase_pversion {
	PACKVER_100 = 0x100
};

/*		resource path define	*/
enum qbase_presource	{
	RES_ROOT = 0,
	RES_SCRIPT = 1,
	RES_IMG = 2,
	RES_MODEL = 3,
	RES_DATA = 4,
	RES_RESOURCE = 5
};

/*		data structure declaration		*/
typedef struct qbase_pck qbase_pck;
typedef char qbase_byte;

/*		API declaration		*/
qbase_pck* qbase_packer_create(char *path, char *pwd, int ver);
qbase_pck* qbase_packer_load(char *path, int ver);
void qbase_packer_close(qbase_pck *pck);
qbase_byte* qbase_packer_get(qbase_pck *pck, int *sz, int pres, char *name, char *pwd, int ver);
int qbase_packer_add(qbase_pck *pck, int pres, qbase_byte *bytes, int sz, char *name);
int qbase_packer_setpwd(qbase_pck *pck, char *oldPwd, char *pwd);
int qbase_packer_update(qbase_pck *pck, int pres, char *name, qbase_byte *newbytes, int sz, char *pwd, int ver);
int qbase_packer_remove(qbase_pck *pck, int pres, char *name ,char *pwd, int ver);

#endif
