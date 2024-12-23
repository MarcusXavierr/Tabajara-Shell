cmake_minimum_required(VERSION 3.27)
project(tsh C)

set(CMAKE_C_STANDARD 99)

# Option to build tests
option(BUILD_TESTS "Build the tests" OFF)
add_definitions(-DUNITY_OUTPUT_COLOR)

# Executables
file(GLOB SOURCES "src/*.c")
file(GLOB HEADERS "src/*.c")
add_executable(tsh tsh.c ${SOURCES} ${HEADERS})

target_compile_options(tsh PRIVATE -g -Wall -Werror -m64)

if(BUILD_TESTS)
    # Create a library from your code
    add_library(unity STATIC external/unity/unity.c)
    add_library(src STATIC ${SOURCES} ${HEADERS})

    add_subdirectory(tests)

    add_custom_target(test
      COMMENT "Running all tests..."
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test  # Executa os testes no diretório correto
    )

    # Busca todos os alvos de teste adicionados e os inclui no target customizado de teste
    get_directory_property(TestTargets DIRECTORY tests BUILDSYSTEM_TARGETS)
    foreach(Target ${TestTargets})
      # Garante que cada teste seja construído antes de ser executado no target customizado
      add_dependencies(test ${Target})

      # Adiciona um comando personalizado para executar cada teste individual
      add_custom_command(
        TARGET test
        POST_BUILD
        COMMAND ${Target}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
        COMMENT "Running ${Target}..."
        VERBATIM
      )
    endforeach()
endif()
