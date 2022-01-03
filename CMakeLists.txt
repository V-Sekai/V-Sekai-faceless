cmake_minimum_required(VERSION 3.0.2)

project(faceless)

set(PLATFORM 64)

set(SDL_REQUIRED_LIBRARIES SDL2)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory( thirdparty/sdl )

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
set(SHARED_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)

# -----------------------------------------------------------------------------
## COMPILER DETECTION ##
if(   (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
   OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
  # Better to use the prebuilt GNU preprocessor define __GNUC__,
  # kept for legacy reason with the sample code.
  add_definitions(-DGNUC)

  set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++11 -include ${SHARED_SRC_DIR}/compat.h")
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /W2 /DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /INCREMENTAL:NO")
else()
  message(FATAL_ERROR "Unsupported compiler '${CMAKE_CXX_COMPILER_ID}'")
endif()

set(SDL2_INCLUDE_DIR ${THIRDPARTY_DIR}/sdl/include)

file(GLOB SHARED_SRC_FILES
  ${SHARED_SRC_DIR}/*.cpp
  ${SHARED_SRC_DIR}/*.h
)

include_directories(
  .
  ${SDL2_INCLUDE_DIR}
  ${OPENVR_INCLUDE_DIR}
)

if(WIN32)
    set(IpSystemTypePath ip/win32)
    set(LIBS ${LIBS} Ws2_32 winmm)
else()
    set(IpSystemTypePath ip/posix)
endif()

include_directories(${THIRDPARTY_DIR})

add_library(oscpack
    ${THIRDPARTY_DIR}/ip/IpEndpointName.h
    ${THIRDPARTY_DIR}/ip/IpEndpointName.cpp
    ${THIRDPARTY_DIR}/ip/NetworkingUtils.h
    ${THIRDPARTY_DIR}/${IpSystemTypePath}/NetworkingUtils.cpp
    ${THIRDPARTY_DIR}/ip/UdpSocket.h
    ${THIRDPARTY_DIR}/${IpSystemTypePath}/UdpSocket.cpp
    ${THIRDPARTY_DIR}/ip/PacketListener.h
    ${THIRDPARTY_DIR}/ip/TimerListener.h
    ${THIRDPARTY_DIR}/osc/OscTypes.h
    ${THIRDPARTY_DIR}/osc/OscTypes.cpp 
    ${THIRDPARTY_DIR}/osc/OscHostEndianness.h
    ${THIRDPARTY_DIR}/osc/OscException.h
    ${THIRDPARTY_DIR}/osc/OscPacketListener.h
    ${THIRDPARTY_DIR}/osc/MessageMappingOscPacketListener.h
    ${THIRDPARTY_DIR}/osc/OscReceivedElements.h
    ${THIRDPARTY_DIR}/osc/OscReceivedElements.cpp
    ${THIRDPARTY_DIR}/osc/OscPrintReceivedElements.h
    ${THIRDPARTY_DIR}/osc/OscPrintReceivedElements.cpp
    ${THIRDPARTY_DIR}/osc/OscOutboundPacketStream.h
    ${THIRDPARTY_DIR}/osc/OscOutboundPacketStream.cpp
)

include_directories(thirdparty/openvr/headers)
add_library(driver_faceless SHARED
  driver_faceless/driverlog.cpp
  driver_faceless/driverlog.h
  driver_faceless/driver_faceless.cpp
)
target_link_libraries(driver_faceless oscpack ${LIBS})

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
add_custom_command(TARGET driver_faceless POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:driver_faceless> ${CMAKE_BINARY_DIR}/faceless/bin/${ARCH_TARGET}/${targetfile}
)