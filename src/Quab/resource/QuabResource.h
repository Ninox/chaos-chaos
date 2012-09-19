#ifndef QUAB_RESOURCE_H
#define QUAB_RESOURCE_H

struct qbase_pck;

namespace Quab
{
	class QuabStream
	{
	private:
		// use smart pointer?
		const char * _buffer;
	public:
		QuabStream();
		QuabStream(const char *buffer, unsigned len);
		QuabStream(const char *path);
		~QuabStream();
		
		bool isWrite;
		unsigned int sz;
		
		bool write(const char *path);
		bool write(char **buffer, unsigned len);
		bool read(const char *path);
		bool read(const char *buffer, unsigned len);
		
		const char* getStream();
		void instead(const char *buff, unsigned len);
	};
	
	class QuabResource
	{
	private:
		qbase_pck *_pck;
	public:
		QuabResource(const char *package);
		~QuabResource();
		
		void list()	;
		void list(int resid);
		
		int pack(int resid, const QuabStream &qs);
		int pack(int resid, const char *filename);
		const QuabStream* unpack(int resid, const char *name);
		
		bool remove(int resid, const char *name);
		bool update(int resid, const char *name, const QuabStream &s);
		bool rename(int resid, const char *oldName, const char *newName);
	};		
}

#endif
