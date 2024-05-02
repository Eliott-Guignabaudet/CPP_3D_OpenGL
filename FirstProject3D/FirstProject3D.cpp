#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "triangulate/polypartition.h"
#include "Manager.h"

int main(int argc, char* argv[])
{
    Manager* manager = new Manager();
    manager->Run();
    glfwTerminate();
    

    // std::vector<glm::vec3> polygon3D;
    // polygon3D.push_back(glm::vec3(-1.3489, 27.3833, 0));
    // polygon3D.push_back(glm::vec3(-1.3501, 27.4038, 0));
    // polygon3D.push_back(glm::vec3(-1.3435, 28.0254, 0));
    // polygon3D.push_back(glm::vec3(-1.3459, 28.004, 0));
    //
    // std::vector<glm::vec2> polygon2D;
    // for (int i = 0; i < polygon3D.size(); ++i)
    // {
    //     glm::vec2 point2D(polygon3D[i].x,polygon3D[i].y);
    //     polygon2D.push_back(point2D);
    // }
    //
    // TPPLPoly tpplPolygon;
    // tpplPolygon.Init(polygon2D.size());
    // for (int i = 0; i < polygon2D.size(); ++i)
    // {
    //     tpplPolygon[i].x = polygon2D[i].x;
    //     tpplPolygon[i].y = polygon2D[i].y;
    //     tpplPolygon[i].id = i;
    // }
    // tpplPolygon.SetOrientation(TPPL_ORIENTATION_CCW);
    // TPPLPolyList result;
    // TPPLPartition partition;
    //
    // partition.Triangulate_EC(&tpplPolygon, &result);
    // std::vector<glm::vec3> triangulatePoints;
    // for (auto poly2D : result)
    // {
    //     for (int i = 0; i < poly2D.GetNumPoints(); ++i)
    //     {
    //         triangulatePoints.push_back(polygon3D[poly2D.GetPoint(i).id]);
    //     }
    // }
    // for (int i = 0; i < triangulatePoints.size(); i++)
    // {
    //     std::cout << "(" << triangulatePoints[i].x << ", " << triangulatePoints[i].y << ", " << triangulatePoints[i].z << ")" << std::endl;
    // }
    //
    // std::cout << result.size() << std::endl;
    return 0;
}
