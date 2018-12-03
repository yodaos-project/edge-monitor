cmake_minimum_required(VERSION 2.8)
project(libwebsockets)

include_directories(./)

file(GLOB LIBWEBSOCKETS
        "core/*.c"
        "misc/*.c"
        "roles/pipe/*.c"
        "roles/http/*.c"
        "roles/http/server/*.c"
        "roles/h1/*.c"
        "roles/ws/*.c"
        "roles/raw-skt/*.c"
        "roles/raw-file/*.c"
        "roles/http/client/*.c"
        "roles/listen/*.c"
        "core/listen/*.c"
        "misc/*.c"
        "unix/*.c"
        "event-libs/poll/*.c"
        "event-libs/libuv/*.c"
        "plat/unix/*.c"
        )

add_library(libwebsockets STATIC ${LIBWEBSOCKETS})
