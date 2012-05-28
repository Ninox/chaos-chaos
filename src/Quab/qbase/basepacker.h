#ifndef	QBASE_PACKER_H
#define QBASE_PACKER_H
 
enum package_ver	{
	QPACK_VER_1 = 0x01
};
typedef char ZByte;
typedef struct qbase_zip qbase_zip;

int qbase_pack_compress(const char *source, const char *target, const char *pwd, package_ver ver, qbase_zip *ziphnd);
int qbase_pack_decompress(const char *target, const char *dest, const char *pwd, package_ver ver, qbase_zip *ziphnd));
ZByte* qbase_pack_getfile(const char *target, const char *pwd, package_ver ver, qbase_zip *ziphnd))
void qbase_pack_remove(const char *target, const char *pwd, package_ver ver, qbase_zip *ziphnd));
int qbase_pack_move(const char *target, const char *dest, const char *pwd, package_ver ver, qbase_zip *ziphnd));

#endif