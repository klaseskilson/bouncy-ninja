#include "bouncy_helpers.h"

glm::vec3 pow(glm::vec3 v, int e)
{
    for (int i = 1; i < e; ++i)
    {
        v *= v;
    }
    return v;
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{"
        << vec.x << " " << vec.y << " "<< vec.z
        << "}";

    return out;
}
std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
    out << "{"
        << vec.x << " " << vec.y
        << "}";

    return out;
}
std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat)
{
    out << "["
        << mat[0].x << "\t" << mat[0].y << "\t" << mat[0].z << "\t" << mat[0].w << std::endl
        << mat[1].x << "\t" << mat[1].y << "\t" << mat[1].z << "\t" << mat[1].w << std::endl
        << mat[2].x << "\t" << mat[2].y << "\t" << mat[2].z << "\t" << mat[2].w << std::endl
        << mat[3].x << "\t" << mat[3].y << "\t" << mat[3].z << "\t" << mat[3].w << std::endl
        << "]" << std::endl;

    return out;
}
