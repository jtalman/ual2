void
InitGlui( void )
{
	GLUI_Panel *panel, *bigpanel;
	GLUI_RadioGroup *group;
	GLUI_Rotation *rot;
	GLUI_Translation *trans, *scale;

 GLUI_Checkbox *glcb;
 GLUI_Checkbox *gldc;

	// setup the glui window:

	glutInitWindowPosition( 50, 0 );
//	Glui = GLUI_Master.create_glui( (char *) GLUITITLE );
	Glui = GLUI_Master.create_glui( (char *) "JDT TEST!" );


	Glui->add_statictext( (char *) GLUITITLE );
	Glui->add_separator();

	Glui->add_checkbox( "Axes", &AxesOn, AXES, (GLUI_Update_CB) Buttons );
//	Glui->add_checkbox( "Axes", &AxesOn );
//	Glui->add_button_to_panel( panel, "Reset", RESET, (GLUI_Update_CB) Buttons );

  glcb = Glui->add_checkbox("Perspective", &WhichProjection, PROJ, (GLUI_Update_CB) Buttons);

  gldc = Glui->add_checkbox( "Intensity Depth Cue", &DepthCueOn, DEPTH, (GLUI_Update_CB) Buttons );

	bigpanel = Glui->add_panel( "" );

	panel = Glui->add_panel_to_panel( bigpanel, "Display Mode" );
		group = Glui->add_radiogroup_to_panel( panel, &WhichObject, RADIOBUTTON_ID, (GLUI_Update_CB) display_cb );
			Glui->add_radiobutton_to_group( group, "Sun - Test Charge - Jupiter");
GLUI_RadioButton *rb2 = Glui->add_radiobutton_to_group( group, "Simulation" );
			Glui->add_radiobutton_to_group( group, "Initial Location" );
			Glui->add_radiobutton_to_group( group, "Current Location" );

rb2->disable();

/*
	Glui->add_column_to_panel( bigpanel, true );

	panel = Glui->add_panel_to_panel( bigpanel, "Object Color" );
		group = Glui->add_radiogroup_to_panel( panel, &WhichColor );
			Glui->add_radiobutton_to_group( group, "Red" );
			Glui->add_radiobutton_to_group( group, "Yellow" );
			Glui->add_radiobutton_to_group( group, "Green" );
			Glui->add_radiobutton_to_group( group, "Cyan" );
			Glui->add_radiobutton_to_group( group, "Blue" );
			Glui->add_radiobutton_to_group( group, "Magenta" );
			Glui->add_radiobutton_to_group( group, "White" );
			Glui->add_radiobutton_to_group( group, "Black" );
*/

	panel = Glui->add_panel( "Object Transformation" );

//                                  GLUI_Rotation( panel, name,                 value_ptr, id,   callback )
//		rot = Glui->add_rotation_to_panel( panel, "Rotation", (float *) RotMatrix, 1000, (GLUI_Update_CB) rotation_callback );

		// allow the object to be spun via the glui rotation widget:

//		rot->set_spin( 1.0 );


		Glui->add_column_to_panel( panel, false );
		scale = Glui->add_translation_to_panel( panel, "Scale",  GLUI_TRANSLATION_Y , &Scale2 );
		scale->set_speed( 0.005f );

		Glui->add_column_to_panel( panel, false );
		trans = Glui->add_translation_to_panel( panel, "Trans XY", GLUI_TRANSLATION_XY, &TransXYZ[0] );
		trans->set_speed( 0.05f );

		Glui->add_column_to_panel( panel, false );
		trans = Glui->add_translation_to_panel( panel, "Trans Z",  GLUI_TRANSLATION_Z , &TransXYZ[2] );
		trans->set_speed( 0.05f );

 GLUI_Spinner    *spinner_C;
 spinner_C  = new GLUI_Spinner( Glui, "C: ", &C, C_step, (GLUI_Update_CB) Buttons );
 spinner_C->set_int_limits( 0, 4500 );

 GLUI_Spinner    *spinner_S_x0;
 spinner_S_x0  = new GLUI_Spinner( Glui, "       Sun x0:", &S_x0, SUN_POS, (GLUI_Update_CB) Buttons );
 spinner_S_x0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_S_y0;
 spinner_S_y0  = new GLUI_Spinner( Glui, "       Sun y0:", &S_y0, SUN_POS, (GLUI_Update_CB) Buttons );
 spinner_S_y0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_S_z0;
 spinner_S_z0  = new GLUI_Spinner( Glui, "       Sun z0:", &S_z0, SUN_POS, (GLUI_Update_CB) Buttons );
 spinner_S_z0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_M_x0;
 spinner_M_x0  = new GLUI_Spinner( Glui, "Test Charge x0:", &M_x0, MOON_POS, (GLUI_Update_CB) Buttons );
 spinner_M_x0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_M_y0;
 spinner_M_y0  = new GLUI_Spinner( Glui, "Test Charge y0:", &M_y0, MOON_POS, (GLUI_Update_CB) Buttons );
 spinner_M_y0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_M_z0;
 spinner_M_z0  = new GLUI_Spinner( Glui, "Test Charge z0:", &M_z0, MOON_POS, (GLUI_Update_CB) Buttons );
 spinner_M_z0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_E_x0;
 spinner_E_x0  = new GLUI_Spinner( Glui, "Jupiter x0:", &E_x0, SUN_POS, (GLUI_Update_CB) Buttons );
 spinner_E_x0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_E_y0;
 spinner_E_y0  = new GLUI_Spinner( Glui, "Jupiter y0:", &E_y0, SUN_POS, (GLUI_Update_CB) Buttons );
 spinner_E_y0->set_float_limits( -100.f, 100.f );

 GLUI_Spinner    *spinner_E_z0;
 spinner_E_z0  = new GLUI_Spinner( Glui, "Jupiter z0:", &E_z0, SUN_POS, (GLUI_Update_CB) Buttons );
 spinner_E_z0->set_float_limits( -100.f, 100.f );

	Glui->add_checkbox( "Debug", &Debug, DBUG, (GLUI_Update_CB) Buttons );

	panel = Glui->add_panel( "", false );

	Glui->add_button_to_panel( panel, "Reset", RESET, (GLUI_Update_CB) Buttons );

	Glui->add_column_to_panel( panel, false );

	Glui->add_button_to_panel( panel, "Quit", QUIT, (GLUI_Update_CB) Buttons );


	// tell glui what graphics window it needs to post a redisplay to:

	Glui->set_main_gfx_window( handle );
//	Glui->set_main_gfx_window( MainWindow );


	// set the graphics window's idle function:

	GLUI_Master.set_glutIdleFunc( NULL );
}
