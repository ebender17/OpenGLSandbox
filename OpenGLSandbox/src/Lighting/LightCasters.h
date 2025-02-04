#pragma once

#include <glm/glm.hpp>

struct DirectionalLight
{
    glm::vec3 Direction;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;

    DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient,
        const glm::vec3& diffuse, const glm::vec3& specular) :
        Direction(direction), Ambient(ambient), Diffuse(diffuse), Specular(specular)
    {
    }
};

struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float Constant;
    float Linear;
    float Quadratic;

    PointLight(const glm::vec3& position, const glm::vec3& ambient,
        const glm::vec3& diffuse, const glm::vec3& specular, float constant,
        float linear, float quadratic) :
        Position(position), Ambient(ambient), Diffuse(diffuse), Specular(specular),
        Constant(constant), Linear(linear), Quadratic(quadratic)
    {
    }
};

struct SpotLight
{
    glm::vec3 Position;
    glm::vec3 Direction;

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;

    float Constant;
    float Linear;
    float Quadratic;

    float InnerCutOff;
    float OuterCutOff;

    SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient,
        const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear,
        float quadratic, float innerCutOff, float outerCutOff) :
        Position(position), Direction(direction), Ambient(ambient), Diffuse(diffuse), Specular(specular),
        Constant(constant), Linear(linear), Quadratic(quadratic), InnerCutOff(innerCutOff), OuterCutOff(outerCutOff)
    {
    }
};