#include "glpch.h"
#include "gltfModel.h"

#include "Core/Core.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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

        // TODO : need the ref here?
        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i)
        {
            // TODO : test what this assert does & replace with OPENGLCORE_ASSERT
            assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
            BindModelNodes(vbos, model, model.nodes[scene.nodes[i]]);
        }

        glBindVertexArray(0);
        // cleanup vbos but do not delete index buffers yet
        // TODO : get explanation here
        for (auto it = vbos.begin(); it != vbos.cend();)
        {
            tinygltf::BufferView bufferView = model.bufferViews[it->first];
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

    void gltfModel::BindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Node& node)
    {
        if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
        {
            BindMesh(vbos, model, model.meshes[node.mesh]);
        }

        for (size_t i = 0; i < node.children.size(); i++)
        {
            assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
            BindModelNodes(vbos, model, model.nodes[node.children[i]]);
        }
    }

    void gltfModel::BindMesh(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh)
    {
        for (size_t i = 0; i < model.bufferViews.size(); ++i)
        {
            const tinygltf::BufferView& bufferView = model.bufferViews[i];

            // TODO : impl drawarrays
            if (bufferView.target == 0)
            {
                LOG_WARN("WARN: bufferView.target is zero");
                continue;  // Unsupported bufferView.
                /*
                  From spec2.0 readme:
                  https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
                           ... drawArrays function should be used with a count equal to
                  the count            property of any of the accessors referenced by the
                  attributes            property            (they are all equal for a given
                  primitive).
                */
            }

            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            // TODO : can we use a LOG_TRACE here?
            std::cout << "bufferview.target " << bufferView.target << std::endl;

            GLuint vbo;
            glGenBuffers(1, &vbo);
            vbos[i] = vbo;
            glBindBuffer(bufferView.target, vbo);

            // TODO : can we use a LOG_TRACE here?
            std::cout << "buffer.data.size = " << buffer.data.size()
                << ", bufferview.byteOffset = " << bufferView.byteOffset
                << std::endl;

            glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
        }

        for (size_t i = 0; mesh.primitives.size(); ++i)
        {
            tinygltf::Primitive primitive = mesh.primitives[i];
            tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

            for (auto& attrib : primitive.attributes)
            {
                tinygltf::Accessor accessor = model.accessors[attrib.second];

                int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

                int size = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR)
                {
                    size = accessor.type;
                }

                int vaa = -1;
                if (attrib.first.compare("POSITION") == 0) vaa = 0;
                if (attrib.first.compare("NORMAL") == 0) vaa = 1;
                if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
                if (vaa > -1)
                {
                    glEnableVertexAttribArray(vaa);
                    glVertexAttribPointer(vaa, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, BUFFER_OFFSET(accessor.byteOffset));
                }
                else
                {
                    LOG_ERROR("vertex attrib array missing: {0}", attrib.first);
                }
            }

            if (model.textures.size() > 0)
            {
                tinygltf::Texture& tex = model.textures[0];

                if (tex.source > -1) {

                    GLuint texid;
                    glGenTextures(1, &texid);

                    tinygltf::Image& image = model.images[tex.source];

                    glBindTexture(GL_TEXTURE_2D, texid);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                    GLenum format = GL_RGBA;

                    if (image.component == 1)
                    {
                        format = GL_RED;
                    }
                    else if (image.component == 2)
                    {
                        format = GL_RG;
                    }
                    else if (image.component == 3)
                    {
                        format = GL_RGB;
                    }
                    else
                    {
                        // ??? TODO
                    }

                    GLenum type = GL_UNSIGNED_BYTE;
                    if (image.bits == 8)
                    {
                        // ok
                    }
                    else if (image.bits == 16)
                    {
                        type = GL_UNSIGNED_SHORT;
                    }
                    else
                    {
                        // ??? TODO
                    }

                    // TODO : is GL_RGBA okay here?
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                        format, type, &image.image.at(0));
                }
            }
        }
    }

}