#include "resource/QuabResource.h"
#include "qbase/qbase.h"
#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
using namespace Quab;

/*    class QuabStream's implementation    */
QuabStream::~QuabStream()	{
	this->_buffer.~shared_array();
}
QuabStream::QuabStream()	{
	this->_buffer.reset(NULL);
	this->sz = 0;
}
QuabStream::QuabStream(const char *buffer, unsigned len)	{
	char *newBuffer = (char*)malloc(len);
	memcpy(newBuffer, buffer, len);
	this->_buffer.reset(newBuffer);
	this->sz = len;
}
QuabStream::QuabStream(const char *path)	{
	FILE *f = fopen(path, "rb");
	assert(f != NULL);
	fseek(f, 0, SEEK_END);
	this->sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	this->_buffer.reset(new char[this->sz]);
	fread(this->_buffer.get(), 1, this->sz, f);
	fclose(f);
}

bool QuabStream::write(const char *path) const	{
	FILE *f = fopen(path, "wb");
	if(f == NULL)
		return false;
	fwrite(this->_buffer.get(), 1, this->sz, f);
	fclose(f);
	return true;
}

bool QuabStream::write(char **buffer, unsigned len) const	{
	if(buffer == NULL || this->sz < len)
		return false;
	if(*buffer != NULL)	{
		free(*buffer);
	}
	*buffer = (char*)malloc(len);
	memcpy(*buffer, this->_buffer.get(), len);
	return true;
}

QuabStream QuabStream::copy() const	{
	QuabStream newStream;
	newStream.sz = this->sz;
	newStream._buffer = this->_buffer;
	return newStream;
}

bool QuabStream::read(const char *path)	{
	char *newBuffer = NULL;
	FILE *f = fopen(path, "rb");
	unsigned size = 0;
	if(f == NULL)
		return false;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	newBuffer = (char*)malloc(size);
	fread(newBuffer, sizeof(char), size, f);
	fclose(f);
	this->_buffer.reset(newBuffer);
	this->sz = size;
	return true;
}

bool QuabStream::read(const char *buffer, unsigned len)	{
	char *readBuffer = NULL;
	if(buffer == NULL || len < 0)
		return false;	
	readBuffer = (char*)malloc(len);
	memcpy(readBuffer, buffer, len);
	this->sz = len;	
	this->_buffer.reset(readBuffer);
	return true;
}

const char* QuabStream::getStream() const	{
	return this->_buffer.get();
}

/*    class QuabResource's implementation    */
QuabResource::QuabResource()	{
	this->_pck = NULL;
	this->_size = 0;
	this->_path = NULL;
}
QuabResource::QuabResource(const char *package)	{
	FILE *f = fopen(package, "rb");
	assert(f != NULL);
	this->_path = package;
	fseek(f, 0, SEEK_END);
	this->_size = ftell(f);
	fclose(f);
	this->_pck = qbase_packer_load(package);
	assert(this->_pck != NULL);
}
QuabResource::~QuabResource()	{
	this->_size = 0;
	if(this->_pck != NULL)
		qbase_packer_free(this->_pck);	
}

QuabResource* QuabResource::create(const char *path)	{	
	qbase_pck *pck = NULL;
	FILE *f = fopen(path, "rb");
	if(f == NULL)
		pck = qbase_packer_create(path);
	else	{
		fclose(f);
		pck = qbase_packer_load(path);
	}
	if(pck == NULL)
		return NULL;
	QuabResource *res = new QuabResource();
	res->_pck = pck;
	res->_size = 0;
	res->_path = path;
	qbase_packer_save(pck, path);
	return res;
}
		
QuabFileList* QuabResource::list()	{
	QuabFileList *head = NULL, *tail = NULL;
	for(int i = 0; i < RES_TOTAL_COUNT; i++)	{
		if(i == 0)	{
			head = this->list(i);
			tail = head;						
		}	else	{
			tail->next = this->list(i);
		}
		while(tail->next != NULL)
			tail = tail->next;
	}
	return head;
}
QuabFileList* QuabResource::list(int resid)	{
	int cnt = 0;
	qbase_resinfo *infos = qbase_packer_show(this->_pck,resid, &cnt);
	QuabFileList *fList = NULL, *node = NULL;
	
	if(infos == NULL || cnt == 0)
		return NULL;
	
	for(int i = 0; i < cnt; i++)	{
		node = (QuabFileList*)malloc(sizeof(QuabFileList));
		node->hashid = infos[i].hashid;
		node->resid = resid;
		node->name = infos[i].filename;
		node->originSize = infos[i].osz;
		node->zippedSize = infos[i].sz;
		node->next = NULL;
		if(i == 0)
			fList = node;
		else
			fList->next = node;
	}
	return fList;
}
bool QuabResource::destoryList(QuabFileList *qList)	{
	QuabFileList *qnext = NULL;
	if(qList != NULL)
	{
		do
		{
			qnext = qList->next;
			free(qList);
			qList = qnext;
		}while(qList != NULL);
	}
	return true;
}

int QuabResource::pack(int resid, const QuabStream &qs)	{
	qbase_pdata data;
	data.pdata = (uchar*)const_cast<char*>(qs.getStream());
	data.sz = qs.getSize();	
	return qbase_packer_add(this->_pck, resid, qs.getName(), &data);
}
int QuabResource::pack(int resid, const char *filename)	{
	FILE *f = fopen(filename, "rb");	
	if(f == NULL)
		return 0;
	else fclose(f);
	QuabStream qs(filename);
	return this->pack(resid, qs);
}
const QuabStream* QuabResource::unpack(int resid, const char *name)	{
	QuabStream *qs = NULL;
	qbase_pdata *data = qbase_packer_get(this->_pck, resid, name, NULL);
	if(data == NULL)	
		return NULL;
	qs = new QuabStream((char*)data->pdata, data->sz);
	return qs;
}
void QuabResource::save() const	{
	qbase_packer_save(this->_pck, this->_path);
}

bool QuabResource::remove(int resid, const char *name)	{
	return qbase_packer_remove(this->_pck, resid, name, NULL) == 1;	
}
bool QuabResource::update(int resid, const char *name, const QuabStream &s)	{
	qbase_pdata data;
	data.pdata = (uchar*)const_cast<char*>(s.getStream());
	data.sz = s.getSize();
	return qbase_packer_update(this->_pck, resid, name, &data, NULL) == 1;
}
bool QuabResource::rename(int resid, const char *oldName, const char *newName)	{
	return qbase_packer_rename(this->_pck, resid, oldName, newName, NULL);
}