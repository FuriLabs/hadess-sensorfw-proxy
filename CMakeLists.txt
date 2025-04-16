cmake_minimum_required(VERSION 3.16)
project(hadess-sensorfw-proxy)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)

add_subdirectory(sensorfw-core)

add_executable(
    hadess-sensorfw-proxy

    iio-sensor-proxy.cpp
    iio-sensor-proxy-resources.cpp
    orientation.cpp
)

target_link_libraries(hadess-sensorfw-proxy PUBLIC
    ${GIO_UNIX_LDFLAGS} ${GIO_UNIX_LIBRARIES}

    sensorfw-core
)

target_include_directories(hadess-sensorfw-proxy PUBLIC
    ${GIO_UNIX_INCLUDE_DIRS}

    sensorfw-core/include
)

install(TARGETS hadess-sensorfw-proxy RUNTIME DESTINATION /usr/libexec/)
