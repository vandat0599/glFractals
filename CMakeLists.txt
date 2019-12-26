cmake_minimum_required(VERSION 3.0)

project(glFractals)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/dep/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/dep/freetype2)

add_definitions(-Wall)
add_definitions(-DROOT_PATH=${CMAKE_CURRENT_LIST_DIR})

if (UNIX)
    set(OpenGL_GL_PREFERENCE GLVND)
endif()
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
               ${PROJECT_SOURCE_DIR}/dep/glad/src/glad.c
               ${PROJECT_SOURCE_DIR}/src/framework/Framework.cpp
               ${PROJECT_SOURCE_DIR}/src/gl/Shader.cpp
               ${PROJECT_SOURCE_DIR}/src/gl/FractalRenderer.cpp
               ${PROJECT_SOURCE_DIR}/src/gl/TextRenderer.cpp
               ${PROJECT_SOURCE_DIR}/src/gl/FreeTypeWrapper.cpp
               ${PROJECT_SOURCE_DIR}/src/controllers/MandelbrotController.cpp
               ${PROJECT_SOURCE_DIR}/src/controllers/JuliaController.cpp
               ${PROJECT_SOURCE_DIR}/src/Utils.cpp
               ${PROJECT_SOURCE_DIR}/src/Main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      CXX_STANDARD 14
                      CXX_EXTENSIONS OFF)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/controllers)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/gl)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/framework)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/dep)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/dep/freetype2/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/dep/glad/include)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} freetype ${OPENGL_LIBRARIES})
