add_executable(libmueb-websocket-server muebwebsocketserver.h
                                        muebwebsocketserver.cc main.cc)

target_link_libraries(libmueb-websocket-server muebtransmitter
                      PRIVATE Qt5::Core Qt5::Gui Qt5::WebSockets)
