include_directories(../thirdparty/openvr/headers)
add_library(driver_faceless SHARED
  driverlog.cpp
  driverlog.h
  driver_faceless.cpp
)

add_definitions(-DDRIVER_SAMPLE_EXPORTS)

target_link_libraries(driver_faceless
  ${CMAKE_DL_LIBS}
)


if(WIN32)
set(ARCH_TARGET win64)
elseif(UNIX)
set(ARCH_TARGET linux64)
endif()

add_custom_target(copy-files ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/bin/drivers/faceless
${CMAKE_BINARY_DIR}/faceless
)

install(TARGETS driver_faceless)