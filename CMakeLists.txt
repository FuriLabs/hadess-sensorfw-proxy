cmake_minimum_required(VERSION 3.16)
project(hadess-sensorfw-proxy)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/iio-sensor-proxy-resources.cpp
           ${CMAKE_CURRENT_BINARY_DIR}/iio-sensor-proxy-resources.h
    COMMAND ${GLIB_COMPILE_RESOURCES}
        --target=${CMAKE_CURRENT_BINARY_DIR}/iio-sensor-proxy-resources.cpp
        --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}
        --generate-source
        ${CMAKE_CURRENT_SOURCE_DIR}/iio-sensor-proxy.gresource.xml
    COMMAND ${GLIB_COMPILE_RESOURCES}
        --target=${CMAKE_CURRENT_BINARY_DIR}/iio-sensor-proxy-resources.h
        --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}
        --generate-header
        ${CMAKE_CURRENT_SOURCE_DIR}/iio-sensor-proxy.gresource.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/iio-sensor-proxy.gresource.xml
            ${CMAKE_CURRENT_SOURCE_DIR}/net.hadess.SensorProxy.xml
    COMMENT "Generating resource files"
)

add_subdirectory(sensorfw-core)

add_executable(
    hadess-sensorfw-proxy
    iio-sensor-proxy.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/iio-sensor-proxy-resources.cpp
    orientation.cpp
)

target_link_libraries(hadess-sensorfw-proxy PUBLIC
    ${GIO_UNIX_LDFLAGS} ${GIO_UNIX_LIBRARIES}
    sensorfw-core
)

target_include_directories(hadess-sensorfw-proxy PUBLIC
    ${GIO_UNIX_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    sensorfw-core/include
)

install(TARGETS hadess-sensorfw-proxy RUNTIME DESTINATION /usr/libexec/)
