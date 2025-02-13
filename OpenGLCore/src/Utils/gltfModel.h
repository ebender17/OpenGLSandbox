#pragma once

#include <tiny_gltf.h>

namespace OpenGLCore::Utils {

    class gltfModel
    {
    public:
        static bool LoadModel(tinygltf::Model& model, const char* filename);
    };

}