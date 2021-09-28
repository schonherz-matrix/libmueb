add_executable(testmuebtransmitter testmuebtransmitter.cc)

target_link_libraries(testmuebtransmitter muebtransmitter Qt6::Core Qt6::Gui)

add_executable(testmuebreceiver testmuebreceiver.cc)

find_package(
  Qt6
  COMPONENTS Widgets
  REQUIRED)
target_link_libraries(testmuebreceiver muebreceiver Qt6::Core Qt6::Gui
                      Qt6::Widgets)
