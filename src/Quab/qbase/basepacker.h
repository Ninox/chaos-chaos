#ifndef BASEPACKER_H
#define BASEPACKER_H

// base operation of the quab's package

typedef char qb_byte;
typedef short qbs_fileid;
typedef int PACKER_RETURN;
typedef int MULTI_PACKER_RETURN;
typedef void* qb_packptr;

/** \brief ��һ��quab����Դ�ļ�
 *
 * \param  file �ļ���·��
 * \return ����һ��ָ���ļ��ṹ��ָ��
 *
 */
qb_packptr qbase_packer_open(wchar_t* file);

/** \brief ��ȡһ���ļ��Ĵ�С
 *
 * \param f �ļ��ṹ��ָ��
 * \return  �����ļ��Ĵ�С
 *
 */
int qbase_packer_size(qb_packptr f);

/** \brief ��ȡ�ļ���У����
 *
 * \param f         �ļ��ṹ��ָ��
 * \param filepath  ��ʾѹ��������ĳ���ļ���crcУ���룬��·��Ϊ��ȫ·���������ļ���
 * \param pid       ��ʾָ��Pid�Ĵ�����ļ���
 * \param filename  ��Pid��ָ�����ļ���������ļ���
 * \return �����ļ���CRCУ����
 *
 */
short qbase_packer_crc(qb_packptr f);
short qbase_packer_crc(qb_packptr f, wchar_t* filepath);
short qbase_packer_crc(qb_packptr f, qbs_fileid pid, wchar_t* filename);

/** \brief ��ȡ�ļ���Դ����Ŀ¼�ṹ
 *
 * \param f ָ���ļ���Դ���ṹ��ָ��
 * \return ����һ�����ڱ�ʾ���е��ļ����б�
 *
 */
void qbase_packer_treelist(qb_packptr f);

/** \brief ��ȡ��Դ����ĳ�ļ��е������ļ��б�
 *
 * \param f     ָ���ļ���Դ���ṹ��ָ��
 * \param pid   ��ʾĿ¼id�������Ҫ��ȡ��Ŀ¼��pid=0
 * \param dir   ��Դ��������ļ���·���������Ҫ��ȡ��Ŀ¼������Ϊdir=NULL �� dir = ""
 * \return ������Դ������ָ���ļ��е��ļ��б�
 *
 */
void qbase_packer_show(qb_packptr f, wchar_t* dir);
void qbase_packer_show(qb_packptr f, qbs_fileid pid);

/** \brief ��ָ���ĵ����ļ����д������
 *
 * \param f         ��Դ���ṹָ��
 * \param zip_path  ���֮��λ����Դ����·������Ŀ¼��Pid
 * \param pid       ���֮��λ����Դ����·������Ŀ¼��Pid
 * \param src_file  Ŀ���ļ���·�����߶�������
 * \param bytes     ���Ŀ���Ƕ����������˲�����ʾ������������
 * \param byte_size ���֮��λ����Դ����·������Ŀ¼��Pid
 * \param pwd       ����Ƿ���Ҫ��������ܣ�������ñ��������ʾ���������
 * \return
 *
 */
PACKER_RETURN qbase_packer_pack(qb_packptr f, wchar_t* zip_path, wchar_t* src_file, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_pack(qb_packptr f, qbs_fileid pid, wchar_t* src_file, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_pack(qb_packptr f, wchar_t* zip_path, qb_byte* bytes, size_t byte_size, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_pack(qb_packptr f, qbs_fileid pid, qb_byte* bytes, size_t byte_size, wchar_t* pwd = NULL);

/** \brief ��������Դ�������ĳ���ļ����н�ѹ������
 *
 * \param f         ָ����Դ����ָ��
 * \param zip_path  ���֮��λ����Դ������ȫ·���������ļ���
 * \param dest_file ��ѹ֮������ŵ�·���������ļ���
 * \param bytes     Ŀ���ļ��Ķ�������
 * \param byte_size ������������
 * \param pwd       ��ѹ����Ҫ�����룬�����������������Ϊnull
 * \return ����һ��״ֵ̬��ʾ�Ƿ�ɹ���ѹ��
 *
 */
PACKER_RETURN qbase_packer_unpack(qb_packptr f, wchar_t* zip_path, wchar_t* dest_file, wchar_t* pwd = NULL);
PACKER_RETURN qbase_packer_unpack(qb_packptr f, wchar_t* zip_path, qb_byte* bytes, size_t byte_size, wchar_t* pwd = NULL);

/** \brief ����ļ����������
 *
 * \param f                 ָ����Դ����ָ��
 * \param multi_zip_path    ָ��Ŀ��Ĵ��·�������԰������·�����÷ֺŷָ�
 * \param src_file          ��Ҫ������ļ���·�����ɰ���������÷ֺŷָ���
 * \param bytes             ��Ҫ����Ķ�������ƴ�
 * \param item_count        ��ʾ�����ƴ��ĸ���
 * \param byte_size         һ��size_t�����飬��ʾ���������ƴ��ĳ���
 * \param pwd               ѹ��ʱ���õ�������
 * \return  ����һ��״ֵ̬����ʾ�Ƿ����ɹ�
 *
 */
MULTI_PACKER_RETURN qbase_packer_multi_pack(qb_packptr f, wchar_t* multi_zip_path, wchar_t* src_file, wchar_t* pwd = NULL);
MULTI_PACKER_RETURN qbase_packer_multi_pack(qb_packptr f, wchar_t* multi_zip_path, qb_byte** bytes, size_t item_count, size_t* byte_size, wchar_t* pwd = NULL);

/** \brief ��ѹ��Դ����ָ��·�����ļ����ݵ�Ŀ�꣬
 *
 * \param f                 ָ����Դ����ָ��
 * \param multi_zip_path    ��Դ���������ļ���·�����÷ֺŷָ�
 * \param dest_path         ��ѹ֮��Ŀ��·����ֻ��ָʾ��һ��·��
 * \param bytes             ÿ���ļ���ѹ֮�������ص�ʵ�ʵĶ�����������
 * \param item_count        ���صĶ�����������ĸ���
 * \param byte_size         ÿ��������������ĳ���
 * \param pwd               ��ѹ����
 * \return
 *
 */
MULTI_PACKER_RETURN qbase_packer_multi_unpack(qb_packptr f, wchar_t* multi_zip_path, wchar_t* dest_path, wchar_t* pwd = NULL);
MULTI_PACKER_RETURN qbase_packer_multi_unpack(qb_packptr f, wchar_t* multi_zip_path, qb_byte** bytes, size_t& item_count, size_t* byte_size, wchar_t* pwd = NULL);

/** \brief �ر�һ���򿪵���Դ��
 *
 * \param ָ����ĳ����Դ�����ļ��ṹָ��
 * \return None
 *
 */
void qbase_packer_close(qb_packptr f);


#endif // BASEPACKER.h
