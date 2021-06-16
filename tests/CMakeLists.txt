add_executable(testmuebtransmitter testmuebtransmitter.cc)

target_link_libraries(testmuebtransmitter muebtransmitter Qt5::Core Qt5::Gui)

add_executable(testmuebreceiver testmuebreceiver.cc)

target_link_libraries(testmuebreceiver muebreceiver Qt5::Core Qt5::Gui
                      Qt5::Widgets)
