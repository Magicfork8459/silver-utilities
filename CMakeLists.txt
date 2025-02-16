cmake_minimum_required(VERSION 3.29.2)
project(silver_utilities)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

file(GLOB SINGLETON_SOURCE
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.cpp
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.h
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.hpp
)

add_library(${PROJECT_NAME} ${SINGLETON_SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS} 
)

target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
    )

set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    CXX_STANDARD 23
) 

if(${TESTS_ENABLED})
    enable_testing()

    set(TEST_NAME_SINGLETON test_singleton)
    set(TEST_NAME_REGISTRY test_registry)

    add_executable(${TEST_NAME_SINGLETON} 
        ${PROJECT_SOURCE_DIR}/tests/test_singleton.cpp
        ${PROJECT_SOURCE_DIR}/tests/test_singleton.h
    )

    add_executable(${TEST_NAME_REGISTRY}
        ${PROJECT_SOURCE_DIR}/tests/test_registry.cpp
        ${PROJECT_SOURCE_DIR}/tests/test_registry.h
    )

    target_include_directories(${TEST_NAME_SINGLETON} PUBLIC 
        ${Boost_INCLUDE_DIRS}        
        ${PROJECT_SOURCE_DIR}
    )

    target_include_directories(${TEST_NAME_REGISTRY} PUBLIC 
        ${Boost_INCLUDE_DIRS}        
        ${PROJECT_SOURCE_DIR}
    )

    target_link_libraries(${TEST_NAME_SINGLETON}
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}
    )

    target_link_libraries(${TEST_NAME_REGISTRY}
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}
    )

    set_target_properties(${TEST_NAME_SINGLETON} ${TEST_NAME_REGISTRY} PROPERTIES 
        LINKER_LANGUAGE CXX
        CXX_STANDARD 23
    ) 



    add_test(NAME ${TEST_NAME_SINGLETON} COMMAND ${TEST_NAME_SINGLETON})
    add_test(NAME ${TEST_NAME_REGISTRY} COMMAND ${TEST_NAME_REGISTRY})
endif()

