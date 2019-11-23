/*
Author: <Yaru Niu> 
Class: ECE6122 
Last Date Modified: <2019-11-22>
Description: Solution to Homework 6: the function to load obj file for the enhanced mode
*/

// Reference: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

#include "objloader.h"
#include <stdio.h>
#include <string.h>
#include <string>

bool loadOBJ(const char * path, std::vector <vec3> & out_vertices, std::vector <vec2> & out_uvs, std::vector <vec3> & out_normals)
{
    // Temporary variables to store the contents of the obj file
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< vec3 > temp_vertices;
    std::vector< vec2 > temp_uvs;
    std::vector< vec3 > temp_normals;

    // Open the file
    FILE * file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Impossible to open the file !\n");
        return false;
    }

    // Read the file till the end
    while(1)
    {
        char lineHeader[128];
        // Read the first word of the line
        int res = fscanf(file, "%s", lineHeader);

        // If end Of File, then quit the loop.
        if (res == EOF)
            break; 
        // Read and store different types of data
        // Vertex
        if ( strcmp( lineHeader, "v" ) == 0 )
        {
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        // Texture coordinate of the vertex
        else if ( strcmp( lineHeader, "vt" ) == 0 )
        {
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }
        // Normal of the vertex
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
            vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }
        // Face
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],
                                 &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    // Process the vertex data 
    for ( unsigned int i=0; i<vertexIndices.size(); i++ )
    {
        unsigned int vertexIndex = vertexIndices[i];
        vec3 vertex = temp_vertices[ vertexIndex-1 ];
        out_vertices.push_back(vertex);
    }

    // Process the normal data
    for ( unsigned int i=0; i<normalIndices.size(); i++ )
    {
        unsigned int normalIndex = normalIndices[i];
        vec3 normal = temp_normals[ normalIndex-1 ];
        out_normals.push_back(normal);
    }

}


