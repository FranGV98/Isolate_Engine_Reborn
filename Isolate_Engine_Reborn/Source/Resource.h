#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <vector>
#include <string>

#include "glmath.h"
#include "Globals.h"

enum class RESOURCE_TYPE
{
	NONE,
	MESH,
	MATERIAL,
	TEXTURE,
	UNKNOWN
};

struct ResourceData
{

};

class Resource
{
public:
	Resource(RESOURCE_TYPE type);
	~Resource();

	const char* GetName() const { return name; }
	uint32 GetUID() const { return uid; }
	RESOURCE_TYPE GetType() const { return type; }
	const char* GetAssetsPath() { return assets_path.c_str(); }
	const char* GetLibraryPath() { return library_path.c_str(); }

	void SetUID(uint32 new_UID);
	void GenerateNewUID();

	void SetAssetsPath(const char* new_path);
	void SetLibraryPath();

private:

	RESOURCE_TYPE	type;

	const char* name;
	uint32			uid;

	std::string		assets_path;
	std::string		library_path;
};

#endif // !__RESOURCE_H__