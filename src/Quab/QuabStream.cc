#include "resource/QuabResource.h"
#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
using namespace Quab;

/*    class QuabStream's implementation    */
QuabStream::QuabStream():_sz(0)	{
	this->_buffer = NULL;	
}

QuabStream::QuabStream(const char *buffer, unsigned len)	{
	this->read(buffer, len);
}

QuabStream::QuabStream(const char *path)	{
	this->read(path);
}

QuabStream::~QuabStream()
{
	if(this->_buffer != NULL)
		free(this->_buffer);
	this->_buffer = NULL;
}

bool QuabStream::write(const char *path) const	{
	FILE *f = fopen(path, "wb");
	if(f != NULL)	{
		if(this->_buffer == NULL)	{
			fclose(f);
			return false;
		}
		fwrite(this->_buffer, this->_sz, 1, f);
		fclose(f);
		return true;
	} else {
		return false;
	}
}

bool QuabStream::write(char **buffer, unsigned len) const	{
	if(buffer == NULL)
		return false;
	if(*buffer != NULL)
		free(*buffer);
	*buffer = (char *)malloc(len);
	const char *streamBuffer = this->_buffer;
	memcpy(*buffer, streamBuffer, len);
	return true;
}

QuabStream QuabStream::copy() const	{
	QuabStream qs;
	char *srcBuff = this->_buffer;
	char *copyBuff = (char*)malloc(this->_sz);
	memcpy(copyBuff, srcBuff, this->_sz);
	qs._buffer = copyBuff;
	qs._sz = this->_sz;
	return qs;
}

bool QuabStream::read(const char *path)	{
	FILE *f = fopen(path, "rb");
	if(f == NULL)	{
		if(this->_buffer != NULL)
			free(this->_buffer);
		this->_buffer = NULL;
		this->_sz = 0;
		return true;
	} else	{
		unsigned len = 0;
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
		char *buffer = (char*)malloc(len);
		fread(buffer, len, 1, f);
		fclose(f);
		if(this->_buffer != NULL)
			free(this->_buffer);
		this->_buffer = buffer;
		this->_sz = len;
		return true;
	}
}

bool QuabStream::read(const char *buffer, unsigned len)	{
	if(buffer == NULL || len == 0)	{
		if(this->_buffer != NULL)
			free(this->_buffer);
		this->_buffer = NULL;
		this->_sz = 0;
		return false;
	}
	if(this->_buffer != NULL)
		free(this->_buffer);
    this->_buffer = const_cast<char*>(buffer);
	this->_sz = len;
	return true;
}
