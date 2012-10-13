#ifndef QUAB_RESOURCE_H
#define QUAB_RESOURCE_H

#include "../QuabDef.h"
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
		char *_buffer;
		unsigned int _sz;
		QuabStream(const QuabStream& qs){}
	public:
		QuabStream();
		QuabStream(const char *buffer, unsigned len);
		QuabStream(const char *path);
		~QuabStream();
		
		inline unsigned int getSize() const {return this->_sz;}
				
		bool write(const char *path) const;
		bool write(char **buffer, unsigned len) const;
		QuabStream copy() const;
		bool read(const char *path);
		bool read(const char *buffer, unsigned len);
		
		inline const char* getStream() const { return this->_buffer; }
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
		
		int pack(int resid, const char *packName, const QuabStream &qs);
		int pack(int resid, const char *packName, const char *filename);
		const QuabStream* unpack(int resid, const char *name);
		void save() const;
		
		bool remove(int resid, const char *name);
		bool update(int resid, const char *name, const QuabStream &s);
		bool rename(int resid, const char *oldName, const char *newName);
	};		
}

#endif
