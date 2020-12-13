
#include "Resource.h"
#include "Randomizer.h"

Resource::Resource(RESOURCE_TYPE type) : 
type (type), 
uid	 (Randomizer::GetBoundedRandomUint()) //Generate UID
{

}

Resource::~Resource()
{

}

void Resource::SetUID(uint32 new_UID)
{
	uid = new_UID;
}
void Resource::GenerateNewUID()
{
	uid = Randomizer::GetRandomUint();
}

void Resource::SetAssetsPath(const char* new_path)
{
	assets_path = new_path;
}

void Resource::SetLibraryPath()
{
	std::string directory;
	std::string name = std::to_string(uid);
	std::string extension;

	switch (type)
	{
	case RESOURCE_TYPE::MESH:
		directory = MESHES_PATH;
		extension = CUSTOM_MESH_EXT;
		break;

	case RESOURCE_TYPE::MATERIAL:
		directory = MATERIALS_PATH;
		extension = CUSTOM_MAT_EXT;
		break;

	case RESOURCE_TYPE::TEXTURE:
		directory = TEXTURES_PATH;
		extension = CUSTOM_TEX_EXT;
		break;
	}
	library_path = directory + name + extension;

	directory.clear();
	name.clear();
	extension.clear();
}