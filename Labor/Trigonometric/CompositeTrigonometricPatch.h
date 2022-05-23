#pragma once
#include "Core/GenericCurves3.h"
#include "Core/Materials.h"
#include "Core/Matrices.h"
#include "Core/ShaderPrograms.h"
#include "Core/TriangulatedMeshes3.h"
#include "SecondOrderTrigonometricPatch3.h"
#include <vector>
#include <GL/glu.h>
#include "Core/DCoordinates3.h"
#include <QOpenGLTexture>

namespace cagd
{
    enum Direction                                           {N, NE, E, SE, S, SW, W, NW};
    class CompositeTrigonometricPatch
    {
    private:
        GLuint                                           _nr_of_patches;
        std::vector<SecondOrderTrigonometricPatch3*>     _patches;
        std::vector<TriangulatedMesh3*>                  _images;
        std::vector<Material*>                           _materials;
        std::vector<QOpenGLTexture*>                     _textures;
        std::vector<ShaderProgram*>                      _shaders;
        std::vector<RowMatrix<GenericCurve3>*>           _u_isoparametric_lines;
        std::vector<RowMatrix<GenericCurve3>*>           _v_isoparametric_lines;
        std::vector<std::vector<GLuint>>                 _neighbour_indexes; // A deletePatch esetén végigjárjuk az oszlopokat
    public:
        GLboolean                                       insertNewPatch();
        GLboolean                                       continuePatch(GLuint patch_index);
        GLboolean                                       joinPatches(GLuint patch_index_1, GLuint patch_index_2, Direction dir_1, Direction dir_2);
        GLboolean                                       mergePatches(GLuint patch_index_1, GLuint patch_index_2, Direction dir_1, Direction dir_2);
        GLboolean                                       deletePatch(GLuint patch_index);
        GLboolean                                       deleteAllPatches();

        ~CompositeTrigonometricPatch();
    };
}
