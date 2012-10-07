#include "resource/QuabResource.h"
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
	this->_sz = len;
}
QuabStream::QuabStream(const char *path)	{
	FILE *f = fopen(path, "rb");
	char *buffer = NULL;
	assert(f != NULL);
	fseek(f, 0, SEEK_END);
	this->sz = ftell(f);
	fseek(f, 0, SEEK_SET);	
	buffer = (char*)malloc(this->sz);
	fread(buffer, 1, this->sz, f);
	this->_buffer.reset(buffer);
	fclose(f);
}

bool QuabStream::write(const char *path) const	{
	FILE *f = fopen(path, "wb");
	char *buffer = NULL;
	if(f == NULL)
		return false;
	buffer = this->_buffer.get();
	fwrite(buffer, 1, this->sz, f);
	fclose(f);
	return true;
}

bool QuabStream::write(char **buffer, unsigned len) const	{
	char *sbuffer = NULL;
	if(buffer == NULL || this->sz < len)
		return false;
	if(*buffer != NULL)	{
		free(*buffer);
	}
	*buffer = (char*)malloc(len);
	sbuffer = this->_buffer.get();
	memcpy(*buffer, sbuffer, len);
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