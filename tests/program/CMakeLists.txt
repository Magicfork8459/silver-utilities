cmake_minimum_required(VERSION 3.29.2)

set(TEST_NAME_PROGRAM test_program)

add_executable(${TEST_NAME_PROGRAM}
    test_program.cpp
    test_program.h
)

target_include_directories(${TEST_NAME_PROGRAM} PUBLIC 
    ${Boost_INCLUDE_DIRS}        
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${TEST_NAME_PROGRAM} 
    ${Boost_LIBRARIES}
    ${PROJECT_NAME}        
)

set_target_properties(${TEST_NAME_PROGRAM} PROPERTIES 
    LINKER_LANGUAGE CXX
    CXX_STANDARD 23
) 

add_test(NAME ${TEST_NAME_PROGRAM} COMMAND ${TEST_NAME_PROGRAM})