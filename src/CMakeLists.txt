add_library(
  muebtransmitter SHARED
  ${CMAKE_SOURCE_DIR}/include/libmueb/libmueb_global.h
  ${CMAKE_SOURCE_DIR}/include/libmueb/muebtransmitter.h configuration.h
  muebtransmitter_p.h muebtransmitter.cc configuration.cc)
target_include_directories(muebtransmitter PUBLIC ../include/${PROJECT_NAME})
target_link_libraries(
  muebtransmitter
  PUBLIC Qt5::Core Qt5::Gui
  PRIVATE Qt5::Network Qt5::Concurrent)
target_compile_definitions(muebtransmitter PRIVATE LIBMUEB_LIBRARY)

add_library(
  muebreceiver SHARED
  ${CMAKE_SOURCE_DIR}/include/libmueb/libmueb_global.h
  ${CMAKE_SOURCE_DIR}/include/libmueb/muebreceiver.h configuration.h
  muebreceiver_p.h muebreceiver.cc configuration.cc)
target_include_directories(muebreceiver PUBLIC ../include/${PROJECT_NAME})
target_link_libraries(
  muebreceiver
  PUBLIC Qt5::Core Qt5::Gui
  PRIVATE Qt5::Network)
target_compile_definitions(muebreceiver PRIVATE LIBMUEB_LIBRARY)
