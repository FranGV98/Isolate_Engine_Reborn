
#include "Assimp.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "R_Mesh.h"

#include "GameObject.h"
#include "I_Meshes.h"

#include "Transform.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")


uint64 Importer::Meshes::Save(const R_Mesh* mesh, const char* path, char** buffer)
{
	uint64 result = 0;

	//Create the header
	uint header[HEADER_SIZE] =
	{
		mesh->vertices.size(),
		mesh->normals.size(),
		mesh->tex_coords.size(),
		mesh->indices.size(),
		mesh->VBO, 																				
		mesh->NBO, 																				
		mesh->TBO, 																				
		mesh->IBO
	};
	uint size = sizeof(header) + sizeof(uint);
	size += header[0];
	size += header[1];
	size += header[2] * sizeof(float);
	size += header[3] * sizeof(uint);

	if (size == 0)
	{
		LOG("[WARNING] Mesh had no data to Save!");
		return 0;
	}

	*buffer = new char[size];
	char* file_cursor = *buffer; //Indicates the position where we are writing

	//Storing the information

	//HEADER
	uint bytes = sizeof(header);
	memcpy(file_cursor, header, bytes);
	file_cursor += bytes;

	//VERTICES
	bytes = mesh->vertices.size() * sizeof(float);
	memcpy(file_cursor, &mesh->vertices[0], bytes);
	file_cursor += bytes;

	//NORMALS
	bytes = mesh->normals.size() * sizeof(float);
	memcpy(file_cursor, &mesh->normals[0], bytes);
	file_cursor += bytes;

	//TEXTURE COORDINATES
	bytes = mesh->tex_coords.size() * sizeof(float);
	memcpy(file_cursor, &mesh->tex_coords[0], bytes);
	file_cursor += bytes;

	//INDICES
	bytes = mesh->indices.size() * sizeof(float);
	memcpy(file_cursor, &mesh->indices[0], bytes);
	file_cursor += bytes;

	//Saving the file
	std::string file_path = path;
	std::string file_name = App->file_system->GetNameFromPath(file_path);

	std::string new_path = MESHES_PATH + file_name + CUSTOM_MESH_EXT;

	result = App->file_system->Save(new_path.c_str(), *buffer, size);

	LOG("[IMPORTER] Saved %s in %s", new_path, MESHES_PATH);

	return result;
}

bool Importer::Meshes::Load(const char* buffer, R_Mesh* mesh)
{
	bool ret = true;

	if (buffer == nullptr || mesh == nullptr)
	{
		LOG("[ERROR] Could not load mesh from Library, mesh or buffer was nullptr");
	}

	char* file_cursor = (char*)buffer;

	//Reading the information

	//HEADER
	uint header[HEADER_SIZE];
	uint bytes = sizeof(header);
	memcpy(header, file_cursor, bytes);
	file_cursor += bytes;

	//VERTICES
	mesh->vertices.resize(header[0]);
	bytes = header[0] * sizeof(float);
	memcpy(&mesh->vertices[0], file_cursor, bytes);
	file_cursor += bytes;

	//NORMALS
	mesh->normals.resize(header[1]);
	bytes = header[1] * sizeof(float);
	memcpy(file_cursor, file_cursor, bytes);
	file_cursor += bytes;

	//TEXTURE COORDINATES
	mesh->tex_coords.resize(header[2]);
	bytes = header[2] * sizeof(float);
	memcpy(file_cursor, file_cursor, bytes);
	file_cursor += bytes;

	//INDICES
	mesh->indices.resize(header[3]);
	bytes = header[3] * sizeof(float);
	memcpy(file_cursor, file_cursor, bytes);
	file_cursor += bytes;

	mesh->VBO = header[4];
	mesh->NBO = header[5];
	mesh->TBO = header[6];
	mesh->IBO = header[7];

	return ret;
}

void Importer::Meshes::Import(const char* path, std::vector<R_Mesh*>& meshes)
{
	char* buffer = nullptr;
	uint file_size = App->file_system->Load(path, &buffer);

	if (file_size > 0)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG("[ERROR] Error loading scene %s", aiGetErrorString());

			return;
		}

		Utilities::ProcessNode(scene, scene->mRootNode, meshes);
	}
}

void Importer::Meshes::Utilities::ProcessNode(const aiScene* scene, aiNode* node, std::vector<R_Mesh*>& meshes)
{
	for (uint i = 0; i < node->mNumMeshes; ++i)														
	{
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];											
		
		if (ai_mesh != nullptr && ai_mesh->HasFaces())												
		{
			R_Mesh* r_mesh = new R_Mesh();													
			r_mesh->og_trans = new Transform();														

			Importer::Meshes::Utilities::GenerateMesh(scene, ai_mesh, r_mesh);						
			Importer::Meshes::Utilities::GenerateTransform(scene, node, r_mesh->og_trans);
			if (r_mesh != nullptr)																	
			{
				meshes.push_back(r_mesh);															
			}
			else
			{
				delete r_mesh;
				r_mesh = nullptr;

				LOG("[ERROR] Could not generate a mesh during Import: R_Mesh* was nullptr!")
			}
		}
		else
		{
			LOG("[ERROR] Could not generate a mesh during Import: aiMesh* was nullptr and/or did not have any faces!")
		}
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		Importer::Meshes::Utilities::ProcessNode(scene, node->mChildren[i], meshes);
	}
}

void Importer::Meshes::Utilities::GenerateMesh(const aiScene* ai_scene, const aiMesh* ai_mesh, R_Mesh* r_mesh)
{
	// Allocating the required memory for each vector
	uint vertices_size = ai_mesh->mNumVertices * 3;			
	r_mesh->vertices.resize(vertices_size);					

	uint normals_size = ai_mesh->mNumVertices * 3;			
	r_mesh->normals.resize(normals_size);

	uint tex_coords_size = ai_mesh->mNumVertices * 2;		
	r_mesh->tex_coords.resize(tex_coords_size);

	uint indices_size = ai_mesh->mNumFaces * 3;				
	r_mesh->indices.resize(indices_size);					

	// Loading the data from the mesh into the corresponding vectors
	Importer::Meshes::Utilities::GetVertices(ai_mesh, r_mesh, vertices_size);								
	Importer::Meshes::Utilities::GetNormals(ai_mesh, r_mesh, normals_size);									
	Importer::Meshes::Utilities::GetTexCoords(ai_mesh, r_mesh, tex_coords_size);							
	Importer::Meshes::Utilities::GetIndices(ai_mesh, r_mesh, indices_size);									

	Importer::Meshes::Utilities::GetTexturePaths(ai_scene, ai_mesh, r_mesh);								

	r_mesh->LoadBuffers();

	if (r_mesh != nullptr)
	{
		r_mesh->aabb.SetNegativeInfinity();
		r_mesh->aabb.Enclose((float3*)&r_mesh->vertices[0], r_mesh->vertices.size() / 3);
	}
}

void Importer::Meshes::Utilities::GenerateTransform(const aiScene* ai_scene, const aiNode* ai_node, Transform* trans)
{
	aiTransform ai_trans;

	ai_node->mTransformation.Decompose(ai_trans.scale, ai_trans.rotation, ai_trans.position);							

	trans->position = { ai_trans.position.x, ai_trans.position.y, ai_trans.position.z };
	trans->rotation = { ai_trans.rotation.x, ai_trans.rotation.y, ai_trans.rotation.z, ai_trans.rotation.w };
	trans->scale = { ai_trans.scale.x, ai_trans.scale.y, ai_trans.scale.z };

	std::string node_name = ai_node->mName.C_Str();																	
	bool found_dummy_node = true;																					
	while (found_dummy_node)
	{
		found_dummy_node = false;

		if (node_name.find("_$AssimpFbx$_") != std::string::npos && ai_node->mNumChildren == 1)						
		{
			ai_node = ai_node->mChildren[0];																		

			ai_node->mTransformation.Decompose(ai_trans.scale, ai_trans.rotation, ai_trans.position);				

			float3	d_position = { ai_trans.position.x, ai_trans.position.y, ai_trans.position.z };						
			Quat	d_rotation = { ai_trans.rotation.x, ai_trans.rotation.y, ai_trans.rotation.z, ai_trans.rotation.w };
			float3	d_scale = { ai_trans.scale.x, ai_trans.scale.y, ai_trans.scale.z };								

																													
			trans->position.Add(d_position);
			trans->rotation.Mul(d_rotation);
			trans->scale.Mul(d_scale);

			node_name = ai_node->mName.C_Str();	
			found_dummy_node = true;
		}
	}
}


void Importer::Meshes::Utilities::GetVertices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasPositions())
	{
		memcpy(&r_mesh->vertices[0], ai_mesh->mVertices, sizeof(float) * size);	

		LOG("[STATUS] Imported %u position vertices!", size)
	}
	else
	{
		LOG("[ERROR] Imported Mesh has no position vertices!");
	}
}

void Importer::Meshes::Utilities::GetNormals(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasNormals())
	{
		memcpy(&r_mesh->normals[0], ai_mesh->mNormals, sizeof(float) * size);

		LOG("[STATUS] Imported %u normals!", size);
	}
	else
	{
		LOG("[ERROR] Imported Mesh has no normals!");
	}
}

void Importer::Meshes::Utilities::GetTexCoords(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasTextureCoords(0))
	{
		for (uint i = 0; i < ai_mesh->mNumVertices; ++i)
		{
			r_mesh->tex_coords[i * 2] = ai_mesh->mTextureCoords[0][i].x;
			r_mesh->tex_coords[i * 2 + 1] = ai_mesh->mTextureCoords[0][i].y;
		}

		LOG("[STATUS] Imported %u texture coordinates!", size)
	}
	else
	{
		LOG("[ERROR] Imported Mesh has no tex coords!");
	}
}

void Importer::Meshes::Utilities::GetIndices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasFaces())																
	{
		uint ind = 0;
		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			aiFace face = ai_mesh->mFaces[i];												

			if (face.mNumIndices == 3)
			{
				for (uint j = 0; j < face.mNumIndices; ++j)
				{
					r_mesh->indices[ind] = face.mIndices[j];
					++ind;
				}
			}
			else
			{
				LOG("[WARNING] Geometry face %u with != 3 indices!", i);
			}
		}

		LOG("[STATUS] Imported %u mesh indices!", size);
	}
}

void Importer::Meshes::Utilities::GetTexturePaths(const aiScene* ai_scene, const aiMesh* ai_mesh, R_Mesh* r_mesh)
{
	if (ai_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
		uint num_textures = material->GetTextureCount(aiTextureType_DIFFUSE);

		for (uint i = 0; i < num_textures; ++i)
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, i, &path);

			std::string file_path = path.C_Str();
			file_path = App->file_system->NormalizePath(file_path.c_str());

			uint file_start = file_path.find_last_of("/");

			if (file_start == file_path.npos)													// If no "/" could be found.
			{
				r_mesh->tex_paths.push_back(file_path.c_str());
			}
			else
			{
				uint file_end = file_path.size();
				file_path = file_path.substr(file_start, file_end);

				r_mesh->tex_paths.push_back(file_path.c_str());
			}

			LOG("[STATUS] Mesh texture file name %u is %s", i, path.C_Str());
		}
	}
}