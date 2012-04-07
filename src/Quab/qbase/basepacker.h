#ifndef BASEPACKER_H
#define BASEPACKER_H

// base operation of the quab's package

typedef char qb_byte;
typedef short qbs_fileid;
typedef int PACKER_RETURN;
typedef int MULTI_PACKER_RETURN;
typedef void* qb_packptr;

/** \brief 打开一个quab的资源文件
 *
 * \param  file 文件的路径
 * \return 返回一个指向文件结构的指针
 *
 */
qb_packptr qbase_packer_open(wchar_t* file);

/** \brief 获取一个文件的大小
 *
 * \param f 文件结构的指针
 * \return  返回文件的大小
 *
 */
int qbase_packer_size(qb_packptr f);

/** \brief 获取文件的校验码
 *
 * \param f         文件结构的指针
 * \param filepath  表示压缩包里面某个文件的crc校验码，此路径为完全路径，包含文件名
 * \param pid       表示指定Pid的代表的文件夹
 * \param filename  在Pid所指定的文件夹下面的文件名
 * \return 返回文件的CRC校验码
 *
 */
short qbase_packer_crc(qb_packptr f);
short qbase_packer_crc(qb_packptr f, wchar_t* filepath);
short qbase_packer_crc(qb_packptr f, qbs_fileid pid, wchar_t* filename);

/** \brief 获取文件资源包的目录结构
 *
 * \param f 指向文件资源包结构的指针
 * \return 返回一个用于表示所有的文件夹列表
 *
 */
void qbase_packer_treelist(qb_packptr f);

/** \brief 获取资源包内某文件夹的所有文件列表
 *
 * \param f     指向文件资源包结构的指针
 * \param pid   表示目录id，如果需要获取根目录则pid=0
 * \param dir   资源包里面的文件夹路径，如果需要获取根目录则设置为dir=NULL 或 dir = ""
 * \return 返回资源包里面指定文件夹的文件列表
 *
 */
void qbase_packer_show(qb_packptr f, wchar_t* dir);
void qbase_packer_show(qb_packptr f, qbs_fileid pid);

/** \brief 对指定的单个文件进行打包操作
 *
 * \param f         资源包结构指针
 * \param zip_path  打包之后位于资源包的路径或者目录的Pid
 * \param pid       打包之后位于资源包的路径或者目录的Pid
 * \param src_file  目标文件的路径或者二进制流
 * \param bytes     如果目标是二进制流，此参数表示二进制流长度
 * \param byte_size 打包之后位于资源包的路径或者目录的Pid
 * \param pwd       打包是否需要用密码加密，如果设置本参数则表示用密码加密
 * \return
 *
 */
PACKER_RETURN qbase_packer_pack(qb_packptr f, wchar_t* zip_path, wchar_t* src_file, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_pack(qb_packptr f, qbs_fileid pid, wchar_t* src_file, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_pack(qb_packptr f, wchar_t* zip_path, qb_byte* bytes, size_t byte_size, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_pack(qb_packptr f, qbs_fileid pid, qb_byte* bytes, size_t byte_size, wchar_t* pwd = NULL);

/** \brief 对在于资源包里面的某个文件进行解压缩操作
 *
 * \param f         指向资源包的指针
 * \param zip_path  打包之后位于资源包的完全路径，包含文件名
 * \param dest_file 解压之后所存放的路径，包含文件名
 * \param bytes     目标文件的二进制流
 * \param byte_size 二进制流长度
 * \param pwd       解压所需要的密码，如果不用密码则设置为null
 * \return 返回一个状态值表示是否成功解压包
 *
 */
PACKER_RETURN qbase_packer_unpack(qb_packptr f, wchar_t* zip_path, wchar_t* dest_file, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_unpack(qb_packptr f, wchar_t* zip_path, qb_byte* bytes, size_t byte_size, wchar_t* pwd = NULL);

/** \brief 多个文件打包操作。
 *
 * \param f                 指向资源包的指针
 * \param multi_zip_path    指定目标的打包路径，可以包含多个路径，用分号分隔
 * \param src_file          需要打包的文件的路径，可包含多个，用分号分隔。
 * \param bytes             需要打包的多个二进制串
 * \param item_count        表示二进制串的个数
 * \param byte_size         一个size_t的数组，表示各个二进制串的长度
 * \param pwd               压缩时候用到的密码
 * \return  返回一个状态值，表示是否打包成功
 *
 */
MULTI_PACKER_RETURN qbase_packer_multi_pack(qb_packptr f, wchar_t* multi_zip_path, wchar_t* src_file, wchar_t* pwd = NULL);
MULTI_PACKER_RETURN qbase_packer_multi_pack(qb_packptr f, wchar_t* multi_zip_path, qb_byte** bytes, size_t item_count, size_t* byte_size, wchar_t* pwd = NULL);

/** \brief 解压资源包中指定路径的文件数据到目标，
 *
 * \param f                 指向资源包的指针
 * \param multi_zip_path    资源包里面多个文件的路径，用分号分隔
 * \param dest_path         解压之后目标路径，只能指示到一个路径
 * \param bytes             每个文件解压之后所返回的实际的二进制流数组
 * \param item_count        返回的二进制流数组的个数
 * \param byte_size         每个二进制流数组的长度
 * \param pwd               解压密码
 * \return
 *
 */
MULTI_PACKER_RETURN qbase_packer_multi_unpack(qb_packptr f, wchar_t* multi_zip_path, wchar_t* dest_path, wchar_t* pwd = NULL);
MULTI_PACKER_RETURN qbase_packer_multi_unpack(qb_packptr f, wchar_t* multi_zip_path, qb_byte** bytes, size_t& item_count, size_t* byte_size, wchar_t* pwd = NULL);

/** \brief 关闭一个打开的资源包
 *
 * \param 指向了某个资源包的文件结构指针
 * \return None
 *
 */
void qbase_packer_close(qb_packptr f);


#endif // BASEPACKER.h
