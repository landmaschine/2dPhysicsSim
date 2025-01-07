cmake_minimum_required(VERSION 3.14)
project(physim2D)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(THIRD_PARTY ${CMAKE_SOURCE_DIR}/third_party)

set(SDL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/SDL)
set(SDL_LIBRARY ${CMAKE_SOURCE_DIR}/third_party/SDL/SDL3.lib)
set(SDL_DLL ${CMAKE_SOURCE_DIR}/third_party/SDL/SDL3.dll)

set(SDL_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/SDL_image)
set(SDL_IMAGE_LIBRARY ${CMAKE_SOURCE_DIR}/third_party/SDL_image/SDL3_image.lib)
set(SDL_IMAGE_DLL ${CMAKE_SOURCE_DIR}/third_party/SDL_image/SDL3_image.dll)

set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/glad/include)
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/engine)

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/glm/)

if(MSVC)
    add_compile_options(/O2 /GL /arch:AVX)
    add_link_options(/LTCG)
else()
    add_compile_options(-O3 -march=native -flto)
    add_compile_options(-mavx)
    add_link_options(-flto)
endif()

include_directories(
    ${THIRD_PARTY}
    ${SDL_INCLUDE_DIR}
    ${SDL_IMAGE_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${ENGINE_DIR}
)

file(GLOB GAME_SOURCES ${CMAKE_SOURCE_DIR}/game/*.cpp ${CMAKE_SOURCE_DIR}/game/*.h)

file(GLOB_RECURSE ENGINE_SOURCES
    ${CMAKE_SOURCE_DIR}/engine/*.c
    ${CMAKE_SOURCE_DIR}/engine/*.cpp
    ${CMAKE_SOURCE_DIR}/engine/*.h
    ${CMAKE_SOURCE_DIR}/engine/*.hpp
)

set(SOURCES ${GAME_SOURCES} ${ENGINE_SOURCES} ${GLAD_SOURCE})

add_executable(physim2D ${SOURCES})

target_link_libraries(physim2D PRIVATE ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARY})

target_link_libraries(physim2D PRIVATE glm)

add_custom_command(TARGET physim2D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${SDL_DLL} ${SDL_IMAGE_DLL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)