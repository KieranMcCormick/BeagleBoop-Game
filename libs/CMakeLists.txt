add_library(model
	Accelerometer.c
	audioMixer.c
	client_interface.c
	game_core.c
	InputManager.c
    Joystick.c
    LED.c
    Potentiometer.c
    seg_display.c
    textToSpeech.c
)

add_library(testlib SHARED IMPORTED)
set_property(TARGET testlib PROPERTY IMPORTED_LOCATION "/home/octavian13/cmpt433/public/asound_lib_BBB/lasound.so")
target_link_libraries(model testlib)

install(TARGETS model DESTINATION lib)
