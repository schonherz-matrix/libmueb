add_executable(libmueb-websocket-server muebwebsocketserver.h
                                        muebwebsocketserver.cc main.cc)

target_link_libraries(libmueb-websocket-server muebtransmitter qt::qt)
