/*
Author: <Yaru Niu> 
Class: ECE6122 
Last Date Modified: <2019-11-22>
Description: Solution to Homework 6: the header file
*/

#include <vector>

// Define struct vec3 to store data from obj file
struct vec3
{
    float x;
    float y;
    float z;
};

// Define struct vec2 to store data from obj file
struct vec2
{
    float x;
    float y;
};

// Claim function to load obj file
bool loadOBJ(
        const char * path,
        std::vector <vec3> & out_vertices,
        std::vector <vec2> & out_uvs,
        std::vector <vec3> & out_normals
);