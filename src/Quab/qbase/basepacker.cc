#include <stdio.h>
#include "basepacker.h"

// declare the real package file struct
typedef struct {

} qb_sFile;

// the static helper function

// implement the interface of basepacker
qb_packptr qbase_packer_open(wchar_t* file)   {
    return NULL;
}

int qbase_packer_size(qb_packptr f)   {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}


short qbase_packer_crc(qb_packptr f)  {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

short qbase_packer_crc(qb_packptr f, wchar_t* filepath)   {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

short qbase_packer_crc(qb_packptr f, qbs_fileid pid, wchar_t* filename)   {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}


void qbase_packer_treelist(qb_packptr f)  {
    qb_sFile* qfile = (qb_sFile*)f;
}


void qbase_packer_show(qb_packptr f, wchar_t* dir)    {
    qb_sFile* qfile = (qb_sFile*)f;
}

void qbase_packer_show(qb_packptr f, qbs_fileid pid)  {
    qb_sFile* qfile = (qb_sFile*)f;
}


PACKER_RETURN qbase_packer_pack(qb_packptr f, wchar_t* zip_path, wchar_t* src_file, wchar_t* pwd)  {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

PACKER_RETURN qbase_packer_pack(qb_packptr f, qbs_fileid pid, wchar_t* src_file, wchar_t* pwd) {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

PACKER_RETURN qbase_packer_pack(qb_packptr f, wchar_t* zip_path, qb_byte* bytes, size_t byte_size, wchar_t* pwd)   {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

PACKER_RETURN qbase_packer_pack(qb_packptr f, qbs_fileid pid, qb_byte* bytes, size_t byte_size, wchar_t* pwd)  {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}


PACKER_RETURN qbase_packer_unpack(qb_packptr f, wchar_t* zip_path, wchar_t* dest_file, wchar_t* pwd)   {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

PACKER_RETURN qbase_packer_unpack(qb_packptr f, wchar_t* zip_path, qb_byte* bytes, size_t byte_size, wchar_t* pwd) {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}


MULTI_PACKER_RETURN qbase_packer_multi_pack(qb_packptr f, wchar_t* multi_zip_path, wchar_t* src_file, wchar_t* pwd)    {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

MULTI_PACKER_RETURN qbase_packer_multi_pack(qb_packptr f, wchar_t* multi_zip_path, qb_byte** bytes, size_t item_count, size_t* byte_size, wchar_t* pwd)    {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}


MULTI_PACKER_RETURN qbase_packer_multi_unpack(qb_packptr f, wchar_t* multi_zip_path, wchar_t* dest_path, wchar_t* pwd) {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}

MULTI_PACKER_RETURN qbase_packer_multi_unpack(qb_packptr f, wchar_t* multi_zip_path, qb_byte** bytes, size_t& item_count, size_t* byte_size, wchar_t* pwd) {
    qb_sFile* qfile = (qb_sFile*)f;
    return 0;
}


void qbase_packer_close(qb_packptr f)   {
    qb_sFile* qfile = (qb_sFile*)f;
}
