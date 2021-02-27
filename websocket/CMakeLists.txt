add_executable(libmueb-websocket-server muebwebsocketserver.h
                                        muebwebsocketserver.cc main.cc)

find_package(
  Qt5
  COMPONENTS WebChannel WebSockets
  REQUIRED)
target_link_libraries(
  libmueb-websocket-server PRIVATE Qt5::Core Qt5::Gui Qt5::WebSockets
                                   muebtransmitter)
