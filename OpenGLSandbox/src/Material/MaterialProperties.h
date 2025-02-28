#pragma once

struct RefractiveIndex
{
    static constexpr float Air = 1.00f;
    static constexpr float Water = 1.33f;
    static constexpr float Ice = 1.309f;
    static constexpr float Glass = 1.52f;
    static constexpr float Diamond = 2.42f;

    static constexpr const char* Indices[] = { "Air", "Water", "Ice", "Glass", "Diamond" };

    static float GetRefractiveIndex(const std::string& key)
    {
        static const std::unordered_map<std::string, float> indexMap = {
            { "Air", Air },
            { "Water", Water },
            { "Ice", Ice },
            { "Glass", Glass },
            { "Diamond", Diamond }
        };

        auto it = indexMap.find(key);
        return (it != indexMap.end()) ? it->second : Air;
    }
};

struct PhongMaterial
{
    unsigned int DiffuseId;
    unsigned int SpecularId;
    unsigned int EmissionId;
    float Shininess;
};