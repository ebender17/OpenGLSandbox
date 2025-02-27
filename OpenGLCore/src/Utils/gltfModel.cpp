#include "glpch.h"
#include "gltfModel.h"

#include "Core/Core.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i)) // cast int offset into char* for OpenGL buffer funcs, such as glVertexAttribPointer

namespace OpenGLCore::Utils {

    bool gltfModel::LoadModel(tinygltf::Model& model, const char* filename)
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        if (!warn.empty())
            LOG_WARN("WARN: {0}", warn);

        if (!err.empty())
            LOG_ERROR("WARN: {0}", err);

        if (!res)
            LOG_WARN("Failed to load glTF: {0}", filename);
        else
            LOG_WARN("Loaded glTF: {0}", filename);

        return res;
    }

    std::pair<GLuint, std::map<int, GLuint>> gltfModel::BindModel(tinygltf::Model& model)
    {
        std::map<int, GLuint> vbos;
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        const tinygltf::Scene& scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i)
        {
            if (scene.nodes[i] < 0 || scene.nodes[i] >= model.nodes.size()) {
                LOG_ERROR("Invalid node index: {0}", std::to_string(scene.nodes[i]));
                continue;
            }
            BindModelNodes(vbos, model, model.nodes[scene.nodes[i]]);
        }

        glBindVertexArray(0);
        // Index buffers (GL_ELEMENT_ARRAY_BUFFER) are needed for rendering, so we don't delete them.
        // Vertex buffers are deleted to free GPU memory since we don't need them after setting up VAOs.
        for (auto it = vbos.begin(); it != vbos.cend();)
        {
            const tinygltf::BufferView& bufferView = model.bufferViews[it->first];
            if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER)
            {
                glDeleteBuffers(1, &vbos[it->first]);
                vbos.erase(it++);
            }
            else
            {
                ++it;
            }
        }

        return { vao, vbos };
    }

    void gltfModel::DrawModel(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos, tinygltf::Model& model)
    {
        glBindVertexArray(vaoAndEbos.first);

        const tinygltf::Scene& scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i)
        {
            DrawModelNodes(vaoAndEbos, model, model.nodes[scene.nodes[i]]);
        }

        glBindVertexArray(0);
    }

    void gltfModel::PrintModelData(tinygltf::Model& model)
    {
        for (auto& mesh : model.meshes)
        {
            LOG_INFO("mesh: {0}", mesh.name);
            for (auto& primitive : mesh.primitives)
            {
                const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

                LOG_INFO("indexaccessor: count {0}, type {1}", indexAccessor.count, indexAccessor.componentType);

                tinygltf::Material& mat = model.materials[primitive.material];
                for (auto& mats : mat.values)
                {
                    std::cout << "mat : " << mats.first.c_str() << std::endl;
                }

                for (auto& image : model.images)
                {
                    LOG_INFO("image name: {0}, size: {1}, width: {2}, height: {3}", image.uri, image.image.size(), image.width, image.height);
                }

                LOG_INFO("indices: {0}, mode: {1}", primitive.indices, primitive.mode);

                for (auto& attrib : primitive.attributes)
                {
                    LOG_INFO("attribute: {0}", attrib.first);
                }
            }
        }
    }

    void gltfModel::BindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Node& node)
    {
        if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
        {
            BindMesh(vbos, model, model.meshes[node.mesh]);
        }

        for (size_t i = 0; i < node.children.size(); i++)
        {
            if (node.children[i] < 0 || node.children[i] >= model.nodes.size()) {
                LOG_ERROR("Invalid node children index: {0}", std::to_string(node.children[i]));
                continue;
            }
            BindModelNodes(vbos, model, model.nodes[node.children[i]]);
        }
    }

    void gltfModel::BindMesh(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh)
    {
        for (size_t i = 0; i < model.bufferViews.size(); ++i)
        {
            const tinygltf::BufferView& bufferView = model.bufferViews[i];

            if (bufferView.target == 0)
            {
                LOG_WARN("bufferView.target is zero");
                continue;
            }

            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

            GLuint vbo;
            glGenBuffers(1, &vbo);
            vbos[i] = vbo;
            glBindBuffer(bufferView.target, vbo);
            glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
        }

        for (size_t i = 0; i < mesh.primitives.size(); ++i)
        {
            const tinygltf::Primitive& primitive = mesh.primitives[i];
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

            for (auto& attrib : primitive.attributes)
            {
                const tinygltf::Accessor& accessor = model.accessors[attrib.second];

                int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

                int size = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR)
                {
                    size = accessor.type;
                }

                int vaa = -1;
                if (attrib.first == "POSITION")        vaa = 0;
                if (attrib.first == "NORMAL")          vaa = 1;
                if (attrib.first == "TEXCOORD_0")      vaa = 2;
                if (attrib.first == "TEXCOORD_1")      vaa = 3;
                if (attrib.first == "COLOR_0")         vaa = 4;
                if (attrib.first == "TANGENT")         vaa = 5;
                if (attrib.first == "JOINTS_0")        vaa = 6;
                if (attrib.first == "WEIGHTS_0")       vaa = 7;

                if (vaa > -1)
                {
                    glEnableVertexAttribArray(vaa);
                    glVertexAttribPointer(vaa, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, BUFFER_OFFSET(accessor.byteOffset));
                }
                else
                {
                    LOG_ERROR("vertex attrib array missing: {0}", attrib.first);
                    int nrAttributes;
                    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
                    LOG_INFO("max vertex attribs: {0}", std::to_string(nrAttributes));
                }
            }

            if (model.textures.size() > 0)
            {
                // TODO : add support for loading more than one texture
                tinygltf::Texture& tex = model.textures[0];

                if (tex.source > -1)
                {
                    GLuint texid;
                    glGenTextures(1, &texid);

                    tinygltf::Image& image = model.images[tex.source];

                    glBindTexture(GL_TEXTURE_2D, texid);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // image data is tightly packed
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                    GLenum format = GL_RGBA;
                    switch (image.component)
                    {
                    case 1:
                        format = GL_RED;
                        break;
                    case 2:
                        format = GL_RG;
                        break;
                    case 3:
                        format = GL_RGB;
                        break;
                    case 4:
                        break;
                    default:
                        LOG_ERROR("Image component format not supported: number of components {0}", std::to_string(image.component));
                    }

                    GLenum type = GL_UNSIGNED_BYTE;
                    GLenum internalFormat = GL_RGBA8;
                    switch (image.bits)
                    {
                    case 8:
                        internalFormat = (format == GL_RED) ? GL_R8 :
                            (format == GL_RG) ? GL_RG8 :
                            (format == GL_RGB) ? GL_RGB8 : GL_RGBA8;
                        break;
                    case 16:
                        type = GL_UNSIGNED_SHORT;
                        internalFormat = (format == GL_RED) ? GL_R16 :
                            (format == GL_RG) ? GL_RG16 :
                            (format == GL_RGB) ? GL_RGB16 : GL_RGBA16;
                        break;
                    case 32:
                        type = GL_FLOAT; // HDR textures
                        internalFormat = (format == GL_RED) ? GL_R32F :
                            (format == GL_RG) ? GL_RG32F :
                            (format == GL_RGB) ? GL_RGB32F : GL_RGBA32F;
                        break;
                    default:
                        LOG_ERROR("Error: Unsupported image bit depth ({0} bits)", image.bits);
                        return;
                    }

                    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.width, image.height, 0, format, type, &image.image.at(0));
                }
            }
        }
    }

    void gltfModel::DrawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos, tinygltf::Model& model, tinygltf::Node& node)
    {
        if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
        {
            DrawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
        }
        for (size_t i = 0; i < node.children.size(); i++)
        {
            DrawModelNodes(vaoAndEbos, model, model.nodes[node.children[i]]);
        }
    }

    void gltfModel::DrawMesh(const std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh)
    {
        for (size_t i = 0; i < mesh.primitives.size(); ++i)
        {
            tinygltf::Primitive& primitive = mesh.primitives[i];
            tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

            // TODO : add support for non-indexed geometry

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));
            glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, BUFFER_OFFSET(indexAccessor.byteOffset));
        }
    }

}