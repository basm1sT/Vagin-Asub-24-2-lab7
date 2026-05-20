#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/GL.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

using namespace std;

class Model {
public:
    vector<Mesh> meshes;
    string directory;

    Model(string const& path) {
        loadModel(path);
    }

    void Draw(Shader shader, glm::mat4 Rot0, glm::mat4 Rot1, glm::mat4 Rot2)
    {
        glm::vec3 joint0_center = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 joint1_center = glm::vec3(1.2984f, 2.3988f, 0.47974f);
        glm::vec3 joint2_center = glm::vec3(1.2984f, 5.7988f, 0.47974f);

        glm::mat4 toJoint0 = glm::translate(glm::mat4(1.0f), -joint0_center);
        glm::mat4 fromJoint0 = glm::translate(glm::mat4(1.0f), joint0_center);

        glm::mat4 toJoint1 = glm::translate(glm::mat4(1.0f), -joint1_center);
        glm::mat4 fromJoint1 = glm::translate(glm::mat4(1.0f), joint1_center);

        glm::mat4 toJoint2 = glm::translate(glm::mat4(1.0f), -joint2_center);
        glm::mat4 fromJoint2 = glm::translate(glm::mat4(1.0f), joint2_center);

        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            glm::mat4 modelMatrix;

            switch (i) {
            case 0:
                modelMatrix = fromJoint0 * Rot0 * toJoint0;
                break;

            case 1:
                modelMatrix = fromJoint0 * Rot0 * toJoint0 *
                    fromJoint1 * Rot1 * toJoint1;
                break;

            case 2:
                modelMatrix = fromJoint0 * Rot0 * toJoint0 *
                    fromJoint1 * Rot1 * toJoint1 *
                    fromJoint2 * Rot2 * toJoint2;
                break;

            default:
                modelMatrix = glm::mat4(1.0f);
                break;
            }

            shader.setMat4("model", modelMatrix);
            meshes[i].Draw(shader);
        }
    }

private:
    void loadModel(string const& path) {
        Assimp::Importer importer;
        directory = path.substr(0, path.find_last_of('/'));

        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        if (nullptr == scene) {
            fprintf(stderr, "ERROR::ASSIMP:: %s\n", importer.GetErrorString());
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->mNormals) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        return Mesh(vertices, indices);
    }
};

#endif