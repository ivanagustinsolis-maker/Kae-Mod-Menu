
cmake_minimum_required(VERSION 3.25)

project(KaeModMenu VERSION 0.1.0)

# Estándar de C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Visibilidad de símbolos
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Buscar código fuente
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Comprobar que existan archivos fuente
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in src/")
endif()

# Crear biblioteca del mod
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Buscar Geode SDK
if(NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK environment variable is not set")
endif()

add_subdirectory(
    "$ENV{GEODE_SDK}"
    "${CMAKE_CURRENT_BINARY_DIR}/geode"
)

# Configurar mod
setup_geode_mod(${PROJECT_NAME})
