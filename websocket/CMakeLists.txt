add_executable(libmueb-websocket-server muebwebsocketserver.h
        muebwebsocketserver.cc main.cc)
find_package(
        Qt6
        COMPONENTS WebSockets
        REQUIRED)
target_link_libraries(libmueb-websocket-server
        PRIVATE Qt6::Core Qt6::Gui Qt6::WebSockets muebtransmitter)
