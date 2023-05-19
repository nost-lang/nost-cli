
project(nost)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
add_compile_definitions(NOST_ALL_DEBUG_SYSTEMS)

add_executable(nost main.c
    nost/vm.c nost/fiber.c nost/bytecode.c nost/gc.c nost/val.c nost/str.c nost/mem.c
    nost/compiler.c nost/ast.c nost/reader.c nost/src.c nost/error.c nost/sym.c nost/list.c
    nost/debug.c nost/fn.c nost/analysis.c nost/pkg.c nost/stdlib.c nost/embed.c nost/util.c)
