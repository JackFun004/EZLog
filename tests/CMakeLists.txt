add_executable(test1 test1.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/threadSafeQueue.cpp)

target_link_libraries(test1 pthread)

add_test(NAME test1 COMMAND test1)

add_executable(test2 test2.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/threadSafeQueue.cpp)

target_link_libraries(test2 pthread)

add_test(NAME test2 COMMAND test2)