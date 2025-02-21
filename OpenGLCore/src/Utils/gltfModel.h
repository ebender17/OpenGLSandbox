#pragma once

#include <tiny_gltf.h>

#include <glad/glad.h>

namespace OpenGLCore::Utils {

    class gltfModel
    {
    public:
        static bool LoadModel(tinygltf::Model& model, const char* filename);

        static std::pair<GLuint, std::map<int, GLuint>> BindModel(tinygltf::Model& model);

    private:
        static void BindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Node& node);
        static void BindMesh(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh);
    };

}