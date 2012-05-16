#ifndef	QBASE_LOADER_H
#define QBASE_LOADER_H

void qbase_loader_init(const char *name);
void qbase_loader_free(const char *name);
void* qbase_loader_getf(const char *name, const char *fname);
void qbase_loader_destory();

#endif