# ---------------------------------------------------------------------------
# TinySpec tests
# ---------------------------------------------------------------------------

set(TINYSPEC_TEST_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/stanza")

function(tinyspec_add_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE tinyspec::tinyspec)
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(${name} PRIVATE
            TINYSPEC_TEST_RESOURCE_DIR="${TINYSPEC_TEST_RESOURCE_DIR}")

    if(MSVC)
        target_compile_options(${name} PRIVATE /W4)
    else()
        target_compile_options(${name} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wstrict-prototypes
                -Wmissing-prototypes)

        if(TINYSPEC_ENABLE_SANITIZERS)
            target_compile_options(${name} PRIVATE
                    -fsanitize=address,undefined
                    -fno-omit-frame-pointer)
            target_link_options(${name} PRIVATE -fsanitize=address,undefined)
        endif()
    endif()

    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES TIMEOUT 120)
endfunction()

tinyspec_add_test(test_json_util test_json_util.c)
tinyspec_add_test(test_model test_model.c)
tinyspec_add_test(test_property test_property.c)
tinyspec_add_test(test_typedef test_typedef.c)
tinyspec_add_test(test_operations test_operations.c)
tinyspec_add_test(test_notice test_notice.c)
tinyspec_add_test(test_stanza test_stanza.c)
tinyspec_add_test(test_product_operator test_product_operator.c)
tinyspec_add_test(test_stanza_resources test_stanza_resources.c)
