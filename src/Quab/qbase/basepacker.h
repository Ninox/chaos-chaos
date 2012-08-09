#ifndef QBASE_PACKER_H
#define QBASE_PACKER_H

#include "basedef.h"
#include <stddef.h>

#define PACKER_FN_OK		0
#define PACKER_FN_ERROR		1
#define PACKER_FN_DENY		2
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
	char **filename;
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

/** \brief create a qbase packer object
 *
 * \param save path, char*
 * \return a pointer to the packer object
 *
 */
qbase_pck* qbase_packer_create(const char *path);

/** \brief get qbase packer object in file
 *
 * \param file path char*
 * \return a qbase pakcer object
 *
 */
qbase_pck* qbase_packer_load(const char *path);

/** \brief save the packer object's modifies
 *
 * \param qbase packer object, qbase_pck*
 * \param save file's path, const char*
 * \return void
 *
 */
void qbase_packer_save(qbase_pck *pck, const char *path);

/** \brief release a qbase packer object
 *
 * \param qbase packer object qbase_pck*
 * \return void
 *
 */
void qbase_packer_free(qbase_pck *pck);

/*		packer security API		*/

/** \brief set the encrypt and decrypt function in packer object
 *
 * \param qbase packer object, qbase_pck*
 * \param encrypted function, qbase_securityfn
 * \param decrypted function, qbase_securityfn
 * \return operation status code
 *
 */
int qbase_packer_setsercurity(qbase_pck *pck, qbase_securityfn encryptfn, qbase_securityfn decryptfn);

/** \brief use password to encrypt the qbase packer object. you must use setsercurity before using this function
 *
 * \param qbase packer object, qbase_pck*
 * \param new password, uchar*
 * \param old password. if it's the first time to set password, you can ignore this argument. const uchar*
 * \return operation status code
 *
 */
int qbase_packer_setpwd(qbase_pck *pck, uchar *npwd, const uchar *opwd);

/*		packer file lists API*/

/** \brief show data list of the qbase packer object
 *
 * \param qbase packer object, qbase_pck*
 * \param the resources' typeid of this object, int
 * \return the resource type infomations
 *
 */
qbase_resinfo* qbase_packer_show(qbase_pck *pck, int resid);

/*		pack common CRUD operations		*/

/** \brief get a file buffer stream in a packer object
 *
 * \param qbase packer object, qbase_pck*
 * \param the resource type you want to get, int
 * \param the variant name, char*
 * \param if the object is not encrypted, set this argument to null, or set it as a password to decrypt uchar*
 * \return the packer data
 *
 */
qbase_pdata* qbase_packer_get(qbase_pck *pck, int resid, const char *fname, const uchar *pwd);

/** \brief add a file in qbase packer object
 *
 * \param qbase packer object, qbase_pck*
 * \param the data's resource id, int
 * \param data name, char*
 * \param data will be added into the object, qbase_pdata*
 * \return the data in object i need to get
 *
 */
int qbase_packer_add(qbase_pck *pck, int resid, const char *fname, qbase_pdata *data);

/** \brief remove the target in packer object
 *
 * \param qbase packer object, qbase_pck*
 * \param target's resource type id, int
 * \param target's name char*
 * \param if the object is not encrypted, set this argument to null, or set it as a password to decrypt uchar*
 * \return operation status code
 *
 */
int qbase_packer_remove(qbase_pck *pck, int resid, const char *fname, const uchar *pwd);

/** \brief update or replace the file
 *
 * \param qbase packer object, qbase_pck*
 * \param target's resource type id, int
 * \param target's name char*
 * \param new data of the target, qbase_pdata*
 * \param if the object is not encrypted, set this argument to null, or set it as a password to decrypt uchar*
 * \return 1 = true, 0 = false
 *
 */
int qbase_packer_update(qbase_pck *pck, int resid, const char *fname, qbase_pdata *data, const uchar *pwd);

/** \brief rename a file in qbase packer object
 *
 * \param qbase packer object, qbase_pck*
 * \param the target's resource id. int
 * \param the original name of the variant, char*
 * \param newname of the variant, char*
 * \param if the object is not encrypted, set this argument to null, or set it as a password to decrypt uchar**
 * \return operation status code
 *
 */
int qbase_packer_rename(qbase_pck *pck, int resid, const char *fname, const char *newname, const uchar *pwd);

#endif
