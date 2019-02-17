#pragma once

#include "D3DUtilities.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "BasicEffect.h"

class MeshLoader {
    public:
        static std::vector<std::unique_ptr<Mesh>> LoadMesh( ID3D11Device* device, const std::string& file, TextureManager* texMan )
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile( file, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices );

            if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
            {
                return {};
            }

            std::string dir = file.substr( 0, file.find_last_of( '/' ) );
            std::vector<std::unique_ptr<Mesh>> out;
            processNode( device, scene->mRootNode, scene, out, dir, texMan );
            return out;
        }

    private:
        static void processNode( ID3D11Device* device, aiNode* node, const aiScene* scene, std::vector<std::unique_ptr<Mesh>>& meshes, const std::string& directory, TextureManager* texMan )
        {
            for ( UINT i = 0; i < node->mNumMeshes; i++ )
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back( processMesh( device, mesh, scene, directory, texMan ) );
            }

            for ( UINT i = 0; i < node->mNumChildren; i++ )
            {
                processNode( device, node->mChildren[i], scene, meshes, directory, texMan );
            }
        }

        static std::unique_ptr<Mesh> processMesh( ID3D11Device* device, aiMesh* mesh, const aiScene* scene, const std::string& directory, TextureManager* texMan )
        {
            std::vector<BasicEffect::Vertex> vertices;
            std::vector<std::uint16_t> indices;

            for ( UINT i = 0; i < mesh->mNumVertices; i++ )
            {
                BasicEffect::Vertex vertex;
                vertex.Pos.x = mesh->mVertices[i].x;
                vertex.Pos.y = mesh->mVertices[i].y;
                vertex.Pos.z = mesh->mVertices[i].z;
                vertex.Norm.x = mesh->mNormals[i].x;
                vertex.Norm.y = mesh->mNormals[i].y;
                vertex.Norm.z = mesh->mNormals[i].z;

                if ( mesh->mTextureCoords[0] )
                {
                    vertex.Tex.x = mesh->mTextureCoords[0][i].x;
                    vertex.Tex.y = mesh->mTextureCoords[0][i].y;
                }

                vertices.push_back( vertex );
            }

            for ( UINT i = 0; i < mesh->mNumFaces; i++ )
            {
                aiFace face = mesh->mFaces[i];

                for ( UINT j = 0; j < face.mNumIndices; j++ )
                {
                    indices.push_back( face.mIndices[j] );
                }
            }

            std::unique_ptr<Mesh> rmesh = std::make_unique<Mesh>( device, vertices.data(), indices.data(), vertices.size(), indices.size() );

            if ( mesh->mMaterialIndex >= 0 )
            {
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

                if ( material->GetTextureCount( aiTextureType_DIFFUSE ) > 0 )
                {
                    aiString str;
                    material->GetTexture( aiTextureType_DIFFUSE, 0, &str );
                    rmesh->SetTexture( texMan->getTexture( D3DUtilities::str_to_wstr( str.C_Str() ) ) );
                }

                aiColor3D colorDiff;
                aiColor3D colorSpec;
                aiColor3D colorAmbi;
                material->Get( AI_MATKEY_COLOR_DIFFUSE, colorDiff );
                material->Get( AI_MATKEY_COLOR_SPECULAR, colorSpec );
                material->Get( AI_MATKEY_COLOR_AMBIENT, colorAmbi );
                Material mat;
                mat.Diffuse = { colorDiff.r, colorDiff.g, colorDiff.b, 1.0f };
                mat.Specular = { colorSpec.r, colorSpec.g, colorSpec.b, 1.0f };
                mat.Ambient = { colorAmbi.r, colorAmbi.g, colorAmbi.b, 1.0f };
                rmesh->SetMaterial( mat );
            }

            return rmesh;
        }
};

