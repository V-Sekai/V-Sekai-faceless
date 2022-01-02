include_directories(../thirdparty/openvr/headers)
add_library(driver_faceless SHARED
  driverlog.cpp
  driverlog.h
  driver_faceless.cpp
)

add_definitions(-DDRIVER_SAMPLE_EXPORTS)

target_link_libraries(driver_faceless
  ${CMAKE_SOURCE_DIR}/thirdparty/openvr/lib/win64/openvr_api.lib
  ${CMAKE_DL_LIBS}
)


install (TARGETS driver_faceless
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin/drivers/faceless/bin/win64
)