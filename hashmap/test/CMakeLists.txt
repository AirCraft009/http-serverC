add_executable(test_hash test.c)
target_link_libraries(test_hash PRIVATE Hashmap)
add_test(NAME hashmap_tests COMMAND test_hash)
