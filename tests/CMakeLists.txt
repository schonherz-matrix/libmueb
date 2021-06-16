add_executable(testmuebtransmitter testmuebtransmitter.cc)

target_link_libraries(testmuebtransmitter muebtransmitter qt::qt)

add_executable(testmuebreceiver testmuebreceiver.cc)

target_link_libraries(testmuebreceiver muebreceiver qt::qt)
