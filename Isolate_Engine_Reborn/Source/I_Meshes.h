#ifndef __I_MESHES_H__
#define __I_MESHES_H__

#include <vector>

#define HEADER_SIZE 8

struct aiMesh;
struct aiScene;
struct aiNode;

class GameObject;
class R_Mesh;

struct Transform;


namespace Importer
{
	namespace Meshes
	{
		void	Import(const char* path, std::vector<R_Mesh*>& meshes);								// Loads the data in the given file path and stores the resulting meshes in a vector.

		uint64	Save(const R_Mesh* mesh, const char* path, char** buffer);							// Processes R_Mesh data into a buffer ready to save. Will return buffer file's size.

		bool	Load(const char* buffer, R_Mesh* mesh);												// Processes buffer data into a ready-to-use R_Mesh. Will return nullptr on ERROR.

		namespace Utilities
		{
			void ProcessNode(const aiScene* scene, aiNode* node, std::vector<R_Mesh*>& meshes);		// Processes the given aiNode and recursively goes through it's childs.

			void GenerateMesh(const aiScene* ai_scene, const aiMesh* ai_mesh, R_Mesh* r_mesh);		// Stores the data of the given aiMesh* in the given R_Mesh*.
			void GenerateTransform(const aiScene* ai_scene, const aiNode* ai_node, Transform* trans);
			void GetVertices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);						// Gets the data for the meshes' vertex positions.
			void GetNormals(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);						// Gets the data for the meshes' vertex normals.
			void GetTexCoords(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);					// Gets the data for the meshes' vertex tex coords.
			void GetIndices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);						// Gets the data for the meshes' indices.

			void GetTexturePaths(const aiScene* ai_scene, const aiMesh* ai_mesh, R_Mesh* r_mesh);	// Will get the filename of the texture/s associated with the given aiMesh* and store them.
		}
	}
}

#endif // !__I_MESHES_H_