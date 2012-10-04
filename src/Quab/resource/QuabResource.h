#ifndef QUAB_RESOURCE_H
#define QUAB_RESOURCE_H

#include "../QuabDef.h"
#include <boost/shared_array.hpp>

struct qbase_pck;

namespace Quab
{
	enum ResourceType	{
		RESTYPE_TOTAL_COUNT =5,
		RESTYPE_ROOT		=0,
		RESTYPE_IMG			=1,
		RESTYPE_MODEL	    =2,
		RESTYPE_SCRIPT	    =3,
		RESTYPE_DATA		=4
	};
	
	struct QuabFileList
	{		
		unsigned hashid;
		int resid;
		const char *name;
		unsigned originSize;
		unsigned zippedSize;
		QuabFileList *next;
	};
	
	class QUAB_API QuabStream
	{
	private:
		// use smart pointer?
//		char * _buffer;
		boost::shared_array<char> _buffer;
		unsigned int sz;
		char *_name;
	public:
		QuabStream();
		QuabStream(const char *buffer, unsigned len);
		QuabStream(const char *path);
		~QuabStream();
		
//		bool isWrite;
		
		inline unsigned int getSize() const {return this->sz;}
		inline const char * getName() const {return this->_name;}
				
		bool write(const char *path) const;
		bool write(char **buffer, unsigned len) const;
		QuabStream copy() const;
		bool read(const char *path);
		bool read(const char *buffer, unsigned len);
		
		const char* getStream() const;
	};
	
	class QUAB_API QuabResource
	{
	private:
		qbase_pck *_pck;
		unsigned _size;
		QuabResource();
		const char *_path;
	public:
		QuabResource(const char *package);
		~QuabResource();	
		
		static QuabResource* create(const char *path);
		
		QuabFileList* list();
		QuabFileList* list(int resid);
		static bool destoryList(QuabFileList *qList);
		unsigned int getSize() { return this->_size; }
		
		int pack(int resid, const QuabStream &qs);
		int pack(int resid, const char *filename);
		const QuabStream* unpack(int resid, const char *name);
		void save() const;
		
		bool remove(int resid, const char *name);
		bool update(int resid, const char *name, const QuabStream &s);
		bool rename(int resid, const char *oldName, const char *newName);
	};		
}

#endif
