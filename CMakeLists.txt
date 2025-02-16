cmake_minimum_required(VERSION 3.29.2)
project(silver_utilities)



file(GLOB SINGLETON_SOURCE
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.cpp
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.h
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.hpp
)

add_library(${PROJECT_NAME} ${SINGLETON_SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    CXX_STANDARD 23
) 

if(${TESTS_ENABLED})
    find_package(Boost REQUIRED COMPONENTS unit_test_framework)

    enable_testing()

    set(TEST_NAME_SINGLETON test_singleton)

    add_executable(${TEST_NAME_SINGLETON} 
        ${PROJECT_SOURCE_DIR}/tests/test_singleton.cpp
        ${PROJECT_SOURCE_DIR}/tests/test_singleton.h
    )

    target_include_directories(${TEST_NAME_SINGLETON} PUBLIC 
        ${Boost_INCLUDE_DIRS}
        #${PROJECT_SOURCE_DIR}/silver/utilities/singleton
        ${PROJECT_SOURCE_DIR}
    )

    target_link_libraries(${TEST_NAME_SINGLETON}
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}
    )

    set_target_properties(${TEST_NAME_SINGLETON} PROPERTIES 
        LINKER_LANGUAGE CXX
        CXX_STANDARD 23
    ) 

    add_test(NAME ${TEST_NAME_SINGLETON} COMMAND ${TEST_NAME_SINGLETON})
endif()

