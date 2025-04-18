#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "chcommon.h"
namespace obj3d {
    struct gl_vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tex_coord;
    };

    struct texture {
        std::string type;
        std::string path;
        u32 id;
    };

    std::vector<texture> textures_loaded;
 
    // NOTE: ::WARNING:: Incomplete do not use thi piece of shite
    class gl_mesh {
    public:
        static const u32 kPositionIndex = 0;
        static const u32 kNormalIndex   = 1;
        static const u32 kTexCoordIndex = 2;

    public:
        std::vector<gl_vertex>     vertices;
        std::vector<u32>        indices;
        std::vector<texture>    textures;

        gl_mesh(std::vector<gl_vertex>& vertices, std::vector<u32>& indices, std::vector<texture>& textures) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            SetupMesh();
        }

        void Draw(Shader& shader) {
            u32 diffuse_nr = 1;
            u32 specular_nr = 1;

            for (u32 i = 0; i < textures.size(); i++) {

                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);

                std::string number;
                std::string name = textures[i].type;
                
                if (name == "texture_diffuse") {
                    number = std::to_string(diffuse_nr++);
                } else if (name == "texture_specular") {
                    number = std::to_string(specular_nr++);
                }

                shader.SetInt(("material." + name + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    private:
        u32 m_vao, m_vbo, m_ebo;
        
        void SetupMesh() {
            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);

            glGenBuffers(1, &m_vbo);
            glGenBuffers(1, &m_ebo);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
            
            glVertexAttribPointer(kPositionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(gl_vertex), (void*)offsetof(gl_vertex, position));
            glVertexAttribPointer(kNormalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(gl_vertex), (void*)offsetof(gl_vertex, normal));
            glVertexAttribPointer(kTexCoordIndex, 2, GL_FLOAT, GL_FALSE, sizeof(gl_vertex), (void*)offsetof(gl_vertex, tex_coord));
            glEnableVertexAttribArray(kPositionIndex);
            glEnableVertexAttribArray(kNormalIndex);
            glEnableVertexAttribArray(kTexCoordIndex);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    };

    class model {
    public:
        model(char* path) {
            LoadModel(path);
        }
      
        void Draw(Shader& shader) {
            for (auto& mesh : meshes) {
                mesh.Draw(shader);
            }
        }

    private:
        std::vector<gl_mesh> meshes;
        std::string directory;

        void LoadModel(const std::string& path) {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));

            ProcessNode(scene->mRootNode, scene);
        }

        
        void ProcessNode(aiNode* node, const aiScene* scene) {
            // Process the node's meshes
            for (u32 i = 0; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(ProcessMesh(mesh, scene));
            }

            // Process the node's children
            for (u32 i = 0; i < node->mNumChildren; i++) {
                ProcessNode(node->mChildren[i], scene);
            }
        }
        
        gl_mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<gl_vertex> vertices;
            std::vector<u32> indices;
            std::vector<texture> textures;

            // Load vertices
            for (u32 i = 0; i < mesh->mNumVertices; i++) {
                gl_vertex vertex;
                vertex.position.x = mesh->mVertices[i].x;
                vertex.position.y = mesh->mVertices[i].y;
                vertex.position.z = mesh->mVertices[i].z;

                vertex.normal.x = mesh->mNormals[i].x;
                vertex.normal.y = mesh->mNormals[i].y;
                vertex.normal.z = mesh->mNormals[i].z;

                vertex.tex_coord.x = mesh->mTextureCoords[0][i].x;
                vertex.tex_coord.y = mesh->mTextureCoords[0][i].y;

                vertices.push_back(vertex);
            }

            // Load indicse
            for (u32 i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for (u32 j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j]);
                }
            }

            if (mesh->mMaterialIndex >= 0) {
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                
                std::vector<texture> diffuse_maps = 
                LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
                
                std::vector<texture> specular_maps = 
                LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
            }

            return gl_mesh{vertices, indices, textures};
        }

        std::vector<texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
        {
            std::vector<texture> textures;
            for (u32 i = 0; i < mat->GetTextureCount(type); i++) {
                aiString str;
                mat->GetTexture(type, i, &str);

                bool skip = false;
                for (u32 j = 0; j < textures_loaded.size(); j++) {
                    if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
                        textures.push_back(textures_loaded[j]);
                        skip = true;
                        break;
                    }
                }
                if (!skip) {
                    texture t;
                    t.id = TextureFromFile(str.C_Str(), directory);
                    t.type = type_name;
                    t.path = str.C_Str();
                    textures.push_back(t);
                }
            }

            return textures;
        }

        bool HasExt(const std::string& str, const std::string& ext) {
            return str.size() > ext.size() && str.compare(str.size() - ext.size(), ext.size(), ext) == 0;
        }

        std::string ToUpper(const std::string& str) {
            std::string nstr = str;
            std::transform(nstr.begin(), nstr.end(), nstr.begin(), [](unsigned char c) { return std::toupper(c); });
            return nstr;
        }

        u32 TextureFromFile(const std::string& path, const std::string& directory) {
            // TODO: check if the current path contains .PNG
            const std::string ext{".png"};

            bool is_png = HasExt(path, ext) || HasExt(path, ToUpper(ext)) ? GL_RGBA : GL_RGB;

            u32 format = is_png ? GL_RGBA : GL_RGB;
            
            // Load the image to use as texture
            u32 id;
            int width, height, nr_channels;
            u8 *data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
            // Generate a texture
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);    
            if (!data)
            {
                std::cerr << "STB::ERROR:: Failed load texture: " << (directory + "\\" + path) << std::endl;;
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);
            
            return 0;
        }
    };

    void Start();

    void Run();
        
} // namespace model

#endif // MODEL_H
