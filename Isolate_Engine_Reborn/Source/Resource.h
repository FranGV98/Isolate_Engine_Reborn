#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <vector>
#include <string>

#include "glmath.h"
#include "Globals.h"

enum class RESOURCE_TYPE
{
	UNKNOWN,
	MESH,
	MATERIAL,
	TEXTURE
};

class Resource
{
public:
	Resource(RESOURCE_TYPE type);
	~Resource();

	const char* GetName() const { return name; }
	uint32 GetUID() const { return uid; }

	void GenerateNewUID();

private:

	RESOURCE_TYPE	type;

	const char*		name;
	uint32			uid;

	std::string		path;
};

#endif // !__RESOURCE_H__