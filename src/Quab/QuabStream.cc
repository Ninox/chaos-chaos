#include "resource/QuabResource.h"
#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
using namespace Quab;

/*    class QuabStream's implementation    */
QuabStream::QuabStream():_sz(0)	{
	this->_buffer.reset(NULL);	
}

QuabStream::QuabStream(const char *buffer, unsigned len)	{
	this->read(buffer, len);
}

QuabStream::QuabStream(const char *path)	{
	this->read(path);
}

QuabStream::~QuabStream()
{
	this->_buffer.~shared_array();
}

bool QuabStream::write(const char *path) const	{
	FILE *f = fopen(path, "wb");
	if(f != NULL)	{
		const char *buff = this->_buffer.get();
		if(buff == NULL)	{
			fclose(f);
			return false;
		}
		fwrite(buff, this->_sz, 1, f);
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
	const char *streamBuffer = this->_buffer.get();
	memcpy(*buffer, streamBuffer, len);
	return true;
}

QuabStream QuabStream::copy() const	{
	QuabStream qs;
	qs._buffer = this->_buffer;
	qs._sz = this->_sz;
	return qs;
}

bool QuabStream::read(const char *path)	{
	FILE *f = fopen(path, "rb");
	if(f == NULL)	{
		this->_buffer.reset(NULL);
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
		this->_buffer.reset(buffer);
		this->_sz = len;
		return true;
	}
}

bool QuabStream::read(const char *buffer, unsigned len)	{
	if(buffer == NULL)	{
		this->_buffer.reset(NULL);
		this->_sz = 0;
		return false;
	}
	char *inputBuffer = (char *)malloc(len);
	memcpy(inputBuffer, buffer, len);
	this->_buffer.reset(inputBuffer);
	this->_sz = len;
	return true;
}