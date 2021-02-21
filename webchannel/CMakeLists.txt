add_executable(
  libmueb-webchannel
  shared/websockettransport.h
  shared/websocketclientwrapper.h
  muebchannel.h
  shared/websockettransport.cpp
  shared/websocketclientwrapper.cpp
  main.cc
  muebchannel.cc)

add_library(
  muebwebchannel SHARED
  shared/websockettransport.h
  shared/websocketclientwrapper.h
  muebchannel.h
  shared/websockettransport.cpp
  shared/websocketclientwrapper.cpp
  main.cc
  muebchannel.cc)

find_package(
  Qt5
  COMPONENTS WebChannel WebSockets Widgets
  REQUIRED)
target_link_libraries(
  libmueb-webchannel PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebChannel
                             Qt5::WebSockets mueb)
target_link_libraries(
  muebwebchannel
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::WebChannel
  Qt5::WebSockets
  mueb)
