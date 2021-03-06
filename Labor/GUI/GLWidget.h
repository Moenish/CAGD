#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <Parametric/ParametricCurves3.h>
#include <Parametric/ParametricSurfaces3.h>
#include <Cyclic/CyclicCurves3.h>
#include <Core/GenericCurves3.h>
#include <Core/TriangulatedMeshes3.h>
#include <Core/Materials.h>
#include <Core/Lights.h>
#include <Core/ShaderPrograms.h>
#include <Trigonometric/SecondOrderTrigonometricPatch3.h>
#include <Trigonometric/SecondOrderTrigonometricArc3.h>

namespace cagd
{
    struct ModelProperties
    {
        GLuint          id;
        GLint           material_id;
        DCoordinate3    position;
        GLdouble        angle1[3];
        GLdouble        angle2[3];
        GLdouble        scale[3];
        GLdouble        color[3];

        inline friend std::istream& operator>>(std::istream& inStream, ModelProperties& modelP)
        {
            inStream >> modelP.id;
            inStream >> modelP.material_id;
            inStream >> modelP.position.x() >> modelP.position.y() >> modelP.position.z();
            inStream >> modelP.angle1[0] >> modelP.angle1[1] >> modelP.angle1[2];
            inStream >> modelP.angle2[0] >> modelP.angle2[1] >> modelP.angle2[2];
            inStream >> modelP.scale[0] >> modelP.scale[1] >> modelP.scale[2];
            inStream >> modelP.color[0] >> modelP.color[1] >> modelP.color[2];
            return inStream;
        }

        inline friend std::ostream& operator<<(std::ostream& outStream, ModelProperties& modelP)
        {
            outStream << modelP.id << std::endl;
            outStream << modelP.material_id << std::endl;
            outStream << modelP.position.x() << modelP.position.y() << modelP.position.z() << std::endl;
            outStream << modelP.angle1[0] << modelP.angle1[1] << modelP.angle1[2] << std::endl;
            outStream << modelP.angle2[0] << modelP.angle2[1] << modelP.angle2[2] << std::endl;
            outStream << modelP.scale[0] << modelP.scale[1] << modelP.scale[2] << std::endl;
            outStream << modelP.color[0] << modelP.color[1] << modelP.color[2] << std::endl;
            return outStream;
        }
    };

    class GLWidget: public QOpenGLWidget
    {
        Q_OBJECT
    private:
        // variables defining the projection matrix
        double       _aspect;            // aspect ratio of the rendering window
        double       _fovy;              // field of view in direction y
        double       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix
        double       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;

        // your other declarations
        GLuint      _selected_page = 0;

        QTimer          *_timer0;
        QTimer          *_timer1;
        QTimer          *_timer2;
        QTimer          *_timer3;
        QTimer          *_timer4;
        QTimer          *_timer5;
        QTimer          *_timer6;
        QTimer          *_timer7;
        DCoordinate3    _i_prime[4], _j_prime[4], _k_prime[4];
        GLdouble        _transformation[4][16];
        GLuint          _time_index[4] = {0, 0, 0, 0};
        GLdouble         _angles[4] = {0.0, 0.0, 0.0, 0.0};


        // Parametric curves
            RowMatrix<RowMatrix<ParametricCurve3::Derivative>>  _pc_derivatives;
            GLdouble                                            _pc_derivative_scale = 1.0;
            bool                                                _pc_zeroth_derivative = true;
            bool                                                _pc_first_derivative = false;
            bool                                                _pc_second_derivative = false;

            GLuint                                    _pc_count = 8;
            RowMatrix<ParametricCurve3*>              _pcs;
            RowMatrix<GenericCurve3*>                 _pc_image_of_pcs;
            GLuint                                    _pc_selected_curve_index = 0;
            GLuint                                    _pc_div_point_count = 200;
            GLenum                                    _pc_usage_flag = GL_STATIC_DRAW;

            void _createParametricCurves();
            void _generateParametricImage(GLuint);
            void _updateParametricVBO(GLuint);
            void _destroyAllExistingParametricCurves();
            void _destroyAllExistingParametricCurvesImages();


        // 3D race
            // Generic curves
                std::vector<bool>                       _cc_points;
                std::vector<bool>                       _cc_zeroth_derivative;
                std::vector<bool>                       _cc_first_derivative;
                std::vector<bool>                       _cc_second_derivative;
                std::vector<int>                        _cc_speed;
                std::vector<GLdouble>                   _cc_speed2;
                GLuint                                  _mod = 2;
                std::vector<GLuint>                     _div;
                std::vector<GLdouble>                   _e;
                RowMatrix<GenericCurve3*>               _img_ccs;
                GLuint                                  _selected_cyclic_curve_index = 0;
                GLuint                                  _selected_cylcic_curve_control_point_index = 0;
            // Approximating cyclic curves
                GLuint                                  _cc_count = 3;
                RowMatrix<GLuint>                       _n;
                RowMatrix<CyclicCurve3*>                _ccs;
            // Interpolating cyclic curves
                GLuint                                  _icc_count = 2;
                RowMatrix<ColumnMatrix<DCoordinate3>>   _iccs;
                RowMatrix<GenericCurve3*>               _img_iccs;
            DirectionalLight                        *_dirLightRace = nullptr;
            Material                                _race_object_materials[6]{MatFBBrass, MatFBSilver, MatFBGold,
                                                                              MatFBEmerald, MatFBPearl, MatFBTurquoise};
            RowMatrix<TriangulatedMesh3>            _race_static_models;
            RowMatrix<ModelProperties>              _race_static_scene;
            RowMatrix<TriangulatedMesh3>            _race_moving_models;
            RowMatrix<ModelProperties>              _race_moving_scene;
            GLuint                                  _static_model_count = 0;
            GLuint                                  _static_object_count = 0;
            GLuint                                  _moving_model_count = 0;
            GLuint                                  _moving_object_count = 0;
            std::vector<std::string>                _static_model_paths{
                                                                 "../Models/Building blocks/Buildings/building_01.off",
                                                                 "../Models/Building blocks/Buildings/building_02.off",
                                                                 "../Models/Building blocks/Buildings/building_03.off",
                                                                 "../Models/Building blocks/Benches/bench_01.off",
                                                                 "../Models/Building blocks/Benches/bench_02.off",
                                                                 "../Models/Building blocks/Ornaments/ornament_01.off",
                                                                 "../Models/Building blocks/Ornaments/ornament_02.off",
                                                                 "../Models/Building blocks/Ornaments/ornament_03.off",
                                                                 "../Models/Building blocks/Primitives/arc.off",
                                                                 "../Models/Building blocks/Primitives/cone.off",
                                                                 "../Models/Building blocks/Primitives/cube.off",
                                                                 "../Models/Building blocks/Statues/dragon.off",
                                                                 "../Models/Building blocks/Statues/Lucy.off",
                                                                 "../Models/Building blocks/Street lamps/street_lamp_01.off",
                                                                 "../Models/Building blocks/Trees/tree_01.off"};
            std::vector<std::string>                _moving_model_paths{"../Models/Boats/boat_01.off",
                                                                        "../Models/Boats/boat_02.off",
                                                                        "../Models/Flying objects/Airplanes/airplane_01.off",
                                                                        "../Models/Flying objects/Airplanes/airplane_02.off",
                                                                        "../Models/Characters/elephant.off",
                                                                        "../Models/Characters/gangster.off",
                                                                        "../Models/Characters/mouse.off",
                                                                        "../Models/Characters/Spot.off"};

            void _createCyclicCurve(GLuint);
            void _createAllCyclicCurves();
            void _generateCyclicCurveImage(GLuint);
            void _updateCyclicCurveVBO(GLuint);
            void _updateCyclicCurveImageVBO(GLuint);
            void _renderCyclicCurves();
            void _destroyCyclicCurve(GLuint);
            void _destroyCyclicCurveImage(GLuint);
            void _destroyAllExistingCyclicCurves();
            void _destroyAllExistingCyclicCurvesImages();

            void _modifyInterpolatingCyclicCurve(GLuint);
            void _createAllInterpolatingCyclicCurves();
            void _generateInterpolatingCyclicCurveImage(GLuint);
            void _updateInterpolatingCyclicCurveImageVBO(GLuint);
            void _renderInterpolatingCyclicCurve(GLuint);
            void _renderAllExistingInterpolatingCyclicCurves();
            void _destroyInterpolatingCyclicCurveImage(GLuint);
            void _destroyAllExistingInterpolatingCyclicCurves();
            void _destroyAllExistingInterpolatingCyclicCurvesImages();

            void _createRaceObjects();
            void _destroyAllExistingObjects();
            bool _getModels();
            bool _getScene();


        // Surfaces;
            QTimer                  *_surfaceTimer;
            DCoordinate3            _ps_i_prime, _ps_j_prime, _ps_k_prime;
            GLdouble                _ps_transformation[16];
            GLdouble                _ps_u = 0, _ps_v = 0;
            GLdouble                _ps_umin, _ps_umax, _ps_vmin, _ps_vmax;
            RowMatrix<GLdouble>     _ps_umins, _ps_umaxs, _ps_vmins, _ps_vmaxs;

            Material                    _surface_materials[6]{MatFBBrass, MatFBSilver, MatFBGold, MatFBEmerald, MatFBPearl, MatFBTurquoise};
            RowMatrix<GLuint>           _surface_selected_material;
            RowMatrix<GLuint>           _surface_selected_texture;
            RowMatrix<QString>          _surface_texture_paths;
            RowMatrix<QOpenGLTexture*>  _surface_textures;
            TriangulatedMesh3           _surface_rat_model;

            RowMatrix<TriangularMatrix<ParametricSurface3::PartialDerivative>>      _ps_derivatives;
            bool                                                                    _ps_do_texture = true;
            DirectionalLight                                                        *_dirLightSurface = nullptr;

            GLuint                                    _ps_count = 6;
            RowMatrix<ParametricSurface3*>            _pss;
            RowMatrix<TriangulatedMesh3*>             _ps_image_of_pss;
            GLuint                                    _ps_selected_surface_index = 0;
            GLuint                                    _ps_udiv_point_count = 200;
            GLuint                                    _ps_vdiv_point_count = 200;
            GLenum                                    _ps_usage_flag = GL_STATIC_DRAW;

            GLuint                                    _psc_count = 3;
            RowMatrix<RowMatrix<ParametricCurve3::Derivative>>  _psc_derivatives;
            RowMatrix<ParametricCurve3*>              _pscs;
            RowMatrix<GenericCurve3*>                 _ps_image_of_pscs;

            void _createParametricSurfaces();
            void _createParametricSurfaceCurves();
            void _generateParametricSurfaceImage(GLuint);
            void _updateParametricSurfaceVBO(GLuint);
            void _generateParametricSurfaceCurveImage(GLuint);
            void _updateParametricSurfaceCurveVBO(GLuint);
            void _destroyAllExistingParametricSurfaces();
            void _destroyAllExistingParametricSurfacesImages();
            void _destroyAllExistingParametricSurfaceCurves();
            void _destroyAllExistingParametricSurfaceCurvesImages();

            void _getSurfaceTextures();


        // Patch magic
            GLuint                              _patch_udiv_point_count = 30;
            GLuint                              _patch_vdiv_point_count = 30;
            GLuint                              _patch_uip_point_count = 20;
            GLuint                              _patch_vip_point_count = 20;
            GLfloat                             _patch_buip_scale = 0.1f;
            GLfloat                             _patch_bvip_scale = 0.1f;
            GLfloat                             _patch_auip_scale = 0.1f;
            GLfloat                             _patch_avip_scale = 0.1f;
            GLuint                              _patch_umax_order = 2;
            GLuint                              _patch_vmax_order = 2;
            GLenum                              _patch_usage_flag = GL_STATIC_DRAW;
            SecondOrderTrigonometricPatch3      _patch;
            TriangulatedMesh3                   *_patch_before_interpolation, *_patch_after_interpolation;
            RowMatrix<GenericCurve3*>           *_patch_buip, *_patch_auip;
            RowMatrix<GenericCurve3*>           *_patch_bvip, *_patch_avip;
            DirectionalLight                    *_dirLightPatch = nullptr;

            bool                                _patch_do_before = true, _patch_do_after = true;
            bool                                _patch_do_uip_0 = false, _patch_do_vip_0 = false;
            bool                                _patch_do_uip_1 = false, _patch_do_vip_1 = false;
            bool                                _patch_do_uip_2 = false, _patch_do_vip_2 = false;

//            GLfloat                             _patch_data[4][4][3] {{{-2.0, -2.0,  1.0},
//                                                                       {-2.0, -1.0, -2.0},
//                                                                       {-2.0,  1.0, -5.0},
//                                                                       {-2.0,  2.0,  1.0}
//                                                                      },
//                                                                      {{-1.0, -2.0,  0.0},
//                                                                       {-1.0, -1.0, -1.0},
//                                                                       {-1.0,  1.0, -1.0},
//                                                                       {-1.0,  2.0,  0.0}
//                                                                      },
//                                                                      {{ 1.0, -2.0,  0.0},
//                                                                       { 1.0, -1.0,  1.0},
//                                                                       { 1.0,  1.0,  1.0},
//                                                                       { 1.0,  2.0,  0.0}
//                                                                      },
//                                                                      {{ 2.0, -2.0, -1.0},
//                                                                       { 2.0, -1.0,  2.0},
//                                                                       { 2.0,  1.0,  2.0},
//                                                                       { 2.0,  2.0, -1.0}}};


            GLfloat                             _patch_data[4][4][3] {{{-2.0, -2.0,  0.0},
                                                                       {-2.0, -1.0,  0.0},
                                                                       {-2.0,  1.0,  0.0},
                                                                       {-2.0,  2.0,  0.0}
                                                                      },
                                                                      {{-1.0, -2.0,  0.0},
                                                                       {-1.0, -1.0,  3.0},
                                                                       {-1.0,  1.0,  3.0},
                                                                       {-1.0,  2.0,  0.0}
                                                                      },
                                                                      {{ 1.0, -2.0,  0.0},
                                                                       { 1.0, -1.0,  3.0},
                                                                       { 1.0,  1.0,  3.0},
                                                                       { 1.0,  2.0,  0.0}
                                                                      },
                                                                      {{ 2.0, -2.0,  0.0},
                                                                       { 2.0, -1.0,  0.0},
                                                                       { 2.0,  1.0,  0.0},
                                                                       { 2.0,  2.0,  0.0}}};

            void _createPatch();

       // Arc
            GLuint                              _arc_div_point_count = 30;
            GLenum                              _arc_usage_flag = GL_STATIC_DRAW;
            SecondOrderTrigonometricArc3        *_arc;
            GenericCurve3                       *_arc_image_of_arc;

            bool                                _arc_do_arc = true;
            bool                                _arc_do_arc_0 = true;
            bool                                _arc_do_arc_1 = false;
            bool                                _arc_do_arc_2 = false;

            void _createArc();

      // Shaders
            RowMatrix<ShaderProgram>    _shaders;
            GLuint                      _shader_index = 0;
            bool                        _shader_do_shader = false;
            GLfloat                     _shader_intensity = 1.0f;
            GLfloat                     _shader_scale       = 1.0f;
            GLfloat                     _shader_smoothing   = 1.0f;
            GLfloat                     _shader_shading     = 1.0f;

            void                _getShaders();

    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

        std::vector<std::string> get_test_names();
        std::vector<std::string> get_surface_test_names();
        GLuint get_pc_count();
        GLuint get_cc_count();
        GLuint get_ps_count();

        // destructor
        ~GLWidget();

    private slots:
        // Race
        void _animate0();
        void _animate1();
        void _animate2();
        void _animate3();

        void _animatePassanger0();
        void _animatePassanger1();
        void _animatePassanger2();
        void _animatePassanger3();

        // Surface
        void _animateSurface();

    public slots:
        // public event handling methods/slots
        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);

        void set_selected_page(int value);

        // Parametric curves
        void pc_set_selected_parametric_curve_index(int value);
        void pc_set_div_point_count(int value);
        void pc_set_derivative_scale(double value);

        void pc_set_zeroth_derivative(bool value);
        void pc_set_first_derivative(bool value);
        void pc_set_second_derivative(bool value);

        // 3D race
        void race_set_selected_cyclic_curve_index(int value);
        void race_set_selected_control_point(int value);
        void race_move_X(double value);
        void race_move_Y(double value);
        void race_move_Z(double value);

        void edit_e(double value);
        void edit_speed(int value);
        void edit_speed2(double value);
        void cc_set_points(bool value);
        void cc_set_zeroth_derivative(bool value);
        void cc_set_first_derivative(bool value);
        void cc_set_second_derivative(bool value);

        // Surfaces
        void ps_set_selected_parametric_surface_index(int value);

        void ps_set_texture_state(bool value);

        void ps_set_texture(int value);
        void ps_set_material(int value);

        // Patch
        void patch_set_before(bool value);
        void patch_set_after(bool value);
        void patch_set_uip_0(bool value);
        void patch_set_uip_1(bool value);
        void patch_set_uip_2(bool value);
        void patch_set_vip_0(bool value);
        void patch_set_vip_1(bool value);
        void patch_set_vip_2(bool value);

        // Arc
        void arc_set_arc(bool value);
        void arc_set_arc_0(bool value);
        void arc_set_arc_1(bool value);
        void arc_set_arc_2(bool value);

        // Shader
        void shader_set(int value);
        void shader_do(bool value);
        void shader_intensity(double value);
        void shader_scale(double value);
        void shader_shading(double value);
        void shader_smoothing(double value);

    signals:
        // Race
        void set_cc_maxLimit(int);
        void set_cc_cp_maxLimit(int);
        void set_cc_cp_values(double, double, double);
        void set_e(double);
        void set_speed(int);
        void set_speed2(double);
        void set_points(bool);
        void set_zeroth_derivative(bool);
        void set_first_derivative(bool);
        void set_second_derivative(bool);

        // Surface
        void surface_set_texture(int);
        void surface_set_material(int);
    };
}
