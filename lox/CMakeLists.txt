add_subdirectory(analysis)
add_subdirectory(ast)
add_subdirectory(error)
add_subdirectory(interpreter)
add_subdirectory(lexer)
add_subdirectory(parser)

add_executable(lox lox.cpp
    $<TARGET_OBJECTS:analysis>
    # $<TARGET_OBJECTS:ast>
    $<TARGET_OBJECTS:error>
    $<TARGET_OBJECTS:interpreter>
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:parser>
    )
