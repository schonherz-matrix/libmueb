add_executable(testmuebtransmitter testmuebtransmitter.cc)

target_link_libraries(testmuebtransmitter PRIVATE muebtransmitter Qt5::Core
                                                  Qt5::Gui)

add_executable(testmuebreceiver testmuebreceiver.cc)

find_package(
  Qt5
  COMPONENTS Widgets
  REQUIRED)
target_link_libraries(testmuebreceiver PRIVATE muebreceiver Qt5::Core Qt5::Gui
                                               Qt5::Widgets)
