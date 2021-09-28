add_library(
  muebtransmitter SHARED
  ${CMAKE_SOURCE_DIR}/include/libmueb/libmueb_global.h
  ${CMAKE_SOURCE_DIR}/include/libmueb/muebtransmitter.h configuration.h
  muebtransmitter_p.h muebtransmitter.cc configuration.cc)
target_include_directories(muebtransmitter PUBLIC ../include/${PROJECT_NAME})
target_link_libraries(
  muebtransmitter
  PUBLIC Qt6::Core Qt6::Gui
  PRIVATE Qt6::Network Qt6::Concurrent)
target_compile_definitions(muebtransmitter PRIVATE LIBMUEB_LIBRARY)

add_library(
  muebreceiver SHARED
  ${CMAKE_SOURCE_DIR}/include/libmueb/libmueb_global.h
  ${CMAKE_SOURCE_DIR}/include/libmueb/muebreceiver.h configuration.h
  muebreceiver_p.h muebreceiver.cc configuration.cc)
target_include_directories(muebreceiver PUBLIC ../include/${PROJECT_NAME})
target_link_libraries(
  muebreceiver
  PUBLIC Qt6::Core Qt6::Gui
  PRIVATE Qt6::Network)
target_compile_definitions(muebreceiver PRIVATE LIBMUEB_LIBRARY)

add_library(
  mueb SHARED
  ${CMAKE_SOURCE_DIR}/include/libmueb/libmueb_global.h
  ${CMAKE_SOURCE_DIR}/include/libmueb/muebtransmitter.h
  muebtransmitter_p.h
  ${CMAKE_SOURCE_DIR}/include/libmueb/muebreceiver.h
  muebreceiver_p.h
  configuration.h
  muebtransmitter.cc
  muebreceiver.cc
  configuration.cc)
target_include_directories(mueb PUBLIC ../include/${PROJECT_NAME})
target_link_libraries(
  mueb
  PUBLIC Qt6::Core Qt6::Gui
  PRIVATE Qt6::Network Qt6::Concurrent)
target_compile_definitions(mueb PRIVATE LIBMUEB_LIBRARY)
