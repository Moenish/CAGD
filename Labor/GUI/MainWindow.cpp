#include "MainWindow.h"

namespace cagd
{
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    {
        setupUi(this);

        /*

          the structure of the main window's central widget

         *---------------------------------------------------*
         |                 central widget                    |
         |                                                   |
         |  *---------------------------*-----------------*  |
         |  |     rendering context     |   scroll area   |  |
         |  |       OpenGL widget       | *-------------* |  |
         |  |                           | | side widget | |  |
         |  |                           | |             | |  |
         |  |                           | |             | |  |
         |  |                           | *-------------* |  |
         |  *---------------------------*-----------------*  |
         |                                                   |
         *---------------------------------------------------*

        */
        _side_widget = new SideWidget(this);

        _scroll_area = new QScrollArea(this);
        _scroll_area->setWidget(_side_widget);
        _scroll_area->setSizePolicy(_side_widget->sizePolicy());
        _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        _gl_widget = new GLWidget(this);

        centralWidget()->setLayout(new QHBoxLayout());
        centralWidget()->layout()->addWidget(_gl_widget);
        centralWidget()->layout()->addWidget(_scroll_area);

        // creating a signal slot mechanism between the rendering context and the side widget
        connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
        connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
        connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));

        connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));

        connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
        connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
        connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));

        connect(_side_widget->page_tabs, SIGNAL(currentChanged(int)), _gl_widget, SLOT(set_selected_page(int)));


        // Race
        connect(_gl_widget, SIGNAL(set_cc_maxLimit(int)), _side_widget, SLOT(set_cc_limit(int)));
        connect(_gl_widget, SIGNAL(set_cc_cp_maxLimit(int)), _side_widget, SLOT(set_cc_controlPoint_limit(int)));
        connect(_gl_widget, SIGNAL(set_cc_cp_values(double, double, double)), _side_widget, SLOT(set_cc_controlPoint_values(double, double, double)));
        connect(_gl_widget, SIGNAL(set_e(double)), _side_widget, SLOT(set_e_value(double)));
        connect(_gl_widget, SIGNAL(set_speed(int)), _side_widget, SLOT(set_speed_value(int)));
        connect(_gl_widget, SIGNAL(set_speed2(double)), _side_widget, SLOT(set_speed2_value(double)));
        connect(_gl_widget, SIGNAL(set_points(bool)), _side_widget, SLOT(set_point(bool)));
        connect(_gl_widget, SIGNAL(set_zeroth_derivative(bool)), _side_widget, SLOT(set_zeroth_derivative(bool)));
        connect(_gl_widget, SIGNAL(set_first_derivative(bool)), _side_widget, SLOT(set_first_derivative(bool)));
        connect(_gl_widget, SIGNAL(set_second_derivative(bool)), _side_widget, SLOT(set_second_derivative(bool)));

        // Surface
        connect(_gl_widget, SIGNAL(surface_set_texture(int)), _side_widget, SLOT(surface_set_texture(int)));
        connect(_gl_widget, SIGNAL(surface_set_material(int)), _side_widget, SLOT(surface_set_material(int)));

        // Parametric Curves
            pc_fillCurveSelector();
            connect(_side_widget->pc_curveSelector, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(pc_set_selected_parametric_curve_index(int)));
            connect(_side_widget->pc_divPointsCount, SIGNAL(valueChanged(int)), _gl_widget, SLOT(pc_set_div_point_count(int)));
            connect(_side_widget->pc_derivativeScale, SIGNAL(valueChanged(double)), _gl_widget, SLOT(pc_set_derivative_scale(double)));
            connect(_side_widget->pc_doZerothDerivative, SIGNAL(clicked(bool)), _gl_widget, SLOT(pc_set_zeroth_derivative(bool)));
            connect(_side_widget->pc_doFirstDerivative, SIGNAL(clicked(bool)), _gl_widget, SLOT(pc_set_first_derivative(bool)));
            connect(_side_widget->pc_doSecondDerivative, SIGNAL(clicked(bool)), _gl_widget, SLOT(pc_set_second_derivative(bool)));
            //  Styling
            _side_widget->pc_doZerothDerivative->setStyleSheet("QCheckBox {color: red; }");
            _side_widget->pc_doFirstDerivative->setStyleSheet("QCheckBox {color: green; }");
            _side_widget->pc_doSecondDerivative->setStyleSheet("QCheckBox {color: blue; }");

        // 3D race
            connect(_side_widget->race_pathSelector, SIGNAL(valueChanged(int)), _gl_widget, SLOT(race_set_selected_cyclic_curve_index(int)));
            connect(_side_widget->race_controlPoint, SIGNAL(valueChanged(int)), _gl_widget, SLOT(race_set_selected_control_point(int)));
            connect(_side_widget->race_moveX, SIGNAL(valueChanged(double)), _gl_widget, SLOT(race_move_X(double)));
            connect(_side_widget->race_moveY, SIGNAL(valueChanged(double)), _gl_widget, SLOT(race_move_Y(double)));
            connect(_side_widget->race_moveZ, SIGNAL(valueChanged(double)), _gl_widget, SLOT(race_move_Z(double)));
            connect(_side_widget->race_edit_e, SIGNAL(valueChanged(double)), _gl_widget, SLOT(edit_e(double)));
            connect(_side_widget->race_speedSpinbox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(edit_speed(int)));
            connect(_side_widget->race_speed2Spinbox, SIGNAL(valueChanged(double)), _gl_widget, SLOT(edit_speed2(double)));
            connect(_side_widget->race_doPoints, SIGNAL(clicked(bool)), _gl_widget, SLOT(cc_set_points(bool)));
            connect(_side_widget->race_doZerothDerivative, SIGNAL(clicked(bool)), _gl_widget, SLOT(cc_set_zeroth_derivative(bool)));
            connect(_side_widget->race_doFirstDerivative, SIGNAL(clicked(bool)), _gl_widget, SLOT(cc_set_first_derivative(bool)));
            connect(_side_widget->race_doSecondDerivative, SIGNAL(clicked(bool)), _gl_widget, SLOT(cc_set_second_derivative(bool)));
            //  Styling
            _side_widget->race_label_moveX->setStyleSheet("QLabel {color: red; }");
            _side_widget->race_label_moveY->setStyleSheet("QLabel {color: green; }");
            _side_widget->race_label_moveZ->setStyleSheet("QLabel {color: blue; }");
            _side_widget->race_doZerothDerivative->setStyleSheet("QLabel {color: red; }");
            _side_widget->race_doFirstDerivative->setStyleSheet("QLabel {color: green; }");
            _side_widget->race_doSecondDerivative->setStyleSheet("QLabel {color: blue; }");

        // Surface
            ps_fillSurfaceSelector();
            connect(_side_widget->ps_surfaceSelector, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(ps_set_selected_parametric_surface_index(int)));
            connect(_side_widget->ps_textureSelector, SIGNAL(valueChanged(int)), _gl_widget, SLOT(ps_set_texture(int)));
            connect(_side_widget->ps_materialSelector, SIGNAL(valueChanged(int)), _gl_widget, SLOT(ps_set_material(int)));
            connect(_side_widget->ps_doTexture, SIGNAL(clicked(bool)), _gl_widget, SLOT(ps_set_texture_state(bool)));

        // Patch
            connect(_side_widget->patch_doBefore, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_before(bool)));
            connect(_side_widget->patch_doAfter, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_after(bool)));
            connect(_side_widget->patch_doUip0, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_uip_0(bool)));
            connect(_side_widget->patch_doUip1, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_uip_1(bool)));
            connect(_side_widget->patch_doUip2, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_uip_2(bool)));
            connect(_side_widget->patch_doVip0, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_vip_0(bool)));
            connect(_side_widget->patch_doVip1, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_vip_1(bool)));
            connect(_side_widget->patch_doVip2, SIGNAL(clicked(bool)), _gl_widget, SLOT(patch_set_vip_2(bool)));

        // Arc
            connect(_side_widget->arc_doArc, SIGNAL(clicked(bool)), _gl_widget, SLOT(arc_set_arc(bool)));
            connect(_side_widget->arc_doArc0, SIGNAL(clicked(bool)), _gl_widget, SLOT(arc_set_arc_0(bool)));
            connect(_side_widget->arc_doArc1, SIGNAL(clicked(bool)), _gl_widget, SLOT(arc_set_arc_1(bool)));
            connect(_side_widget->arc_doArc2, SIGNAL(clicked(bool)), _gl_widget, SLOT(arc_set_arc_2(bool)));

        // Shaders
            connect(_side_widget->doShader, SIGNAL(clicked(bool)), _gl_widget, SLOT(shader_do(bool)));
            connect(_side_widget->shaderSelector, SIGNAL(valueChanged(int)), _gl_widget, SLOT(shader_set(int)));
            connect(_side_widget->shader_intensity, SIGNAL(valueChanged(double)), _gl_widget, SLOT(shader_intensity(double)));
            connect(_side_widget->shader_scale, SIGNAL(valueChanged(double)), _gl_widget, SLOT(shader_scale(double)));
            connect(_side_widget->shader_smoothing, SIGNAL(valueChanged(double)), _gl_widget, SLOT(shader_smoothing(double)));
            connect(_side_widget->shader_shading, SIGNAL(valueChanged(double)), _gl_widget, SLOT(shader_shading(double)));
    }

    void MainWindow::pc_fillCurveSelector()
    {
        std::vector<std::string> names = _gl_widget->get_test_names();
        for (GLuint it = 0; it < _gl_widget->get_pc_count(); it++)
        {
            QString s = QString::fromStdString(names[it]);
            _side_widget->pc_curveSelector->addItem(s);
        }
    }

    void MainWindow::ps_fillSurfaceSelector()
    {
        std::vector<std::string> names = _gl_widget->get_surface_test_names();
        for (GLuint it = 0; it < _gl_widget->get_ps_count(); it++)
        {
            QString s = QString::fromStdString(names[it]);
            _side_widget->ps_surfaceSelector->addItem(s);
        }
    }

    //--------------------------------
    // implementation of private slots
    //--------------------------------
    void MainWindow::on_action_Quit_triggered()
    {
        qApp->exit(0);
    }
}
