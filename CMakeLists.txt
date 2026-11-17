cmake_minimum_required(VERSION 3.16)

project(tinyspec
        VERSION 1.0.1
        DESCRIPTION "TinySpec - a device framework for xiot"
        HOMEPAGE_URL "https://github.com/openxiot/tiny-spec"
        LANGUAGES C)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build tinyspec as a shared library" OFF)
option(TINYSPEC_BUILD_TESTS "Build the TinySpec test suite" ${PROJECT_IS_TOP_LEVEL})
option(TINYSPEC_ENABLE_SANITIZERS "Build with Address/UB sanitizers (debug builds)" OFF)
option(TINYSPEC_INSTALL "Generate install rules" ${PROJECT_IS_TOP_LEVEL})
option(TINYSPEC_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(TINYSPEC_TINYCORE_DIR "" CACHE PATH
        "Path to a tiny-core source tree (default: ../tiny-core)")

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS ON)
endif()

# ---------------------------------------------------------------------------
# Dependencies: an installed tinycore package, or the sibling source tree
# ---------------------------------------------------------------------------
find_package(tinycore 1.0 QUIET)

if(NOT TARGET tinycore::tinycore)
    if(TINYSPEC_TINYCORE_DIR)
        set(TINYSPEC_TINYCORE_ROOT "${TINYSPEC_TINYCORE_DIR}")
    else()
        set(TINYSPEC_TINYCORE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../tiny-core")
    endif()

    if(NOT EXISTS "${TINYSPEC_TINYCORE_ROOT}/CMakeLists.txt")
        message(FATAL_ERROR
                "tiny-core was not found.  Install it, set CMAKE_PREFIX_PATH, or "
                "point TINYSPEC_TINYCORE_DIR at a tiny-core source tree "
                "(looked in ${TINYSPEC_TINYCORE_ROOT}).")
    endif()

    add_subdirectory("${TINYSPEC_TINYCORE_ROOT}"
            "${CMAKE_CURRENT_BINARY_DIR}/tiny-core")
endif()

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------
set(TINYSPEC_SOURCES
        src/stanza/iq/device/control/invoke_actions.c
        src/stanza/iq/device/manager/get_children.c
        src/stanza/iq/device/manager/properties_changed.c
        src/typedef/product/child/child.c
        # IotTypedef: device description model
        src/typedef/device/action.c
        src/typedef/device/argument.c
        src/typedef/device/device.c
        src/typedef/device/event.c
        src/typedef/device/property.c
        src/typedef/device/service.c
        src/typedef/device/property/access/access.c
        src/typedef/device/property/constraint/value_list.c
        src/typedef/device/property/constraint/value_range.c
        src/typedef/device/property/format/format.c
        src/typedef/device/urn/urn.c
        src/typedef/device/urn/urn_type.c
        src/typedef/notice/device/device_notice.c
        src/typedef/notice/device/device_notice_codec.c
        src/typedef/notice/device/impl/device_event_occurred.c
        src/typedef/notice/device/impl/device_event_occurred_codec.c
        src/typedef/notice/device/impl/device_property_changed.c
        src/typedef/notice/device/impl/device_property_changed_codec.c
        src/typedef/operation/action_operation.c
        src/typedef/operation/event_operation.c
        src/typedef/operation/property_operation.c
        src/typedef/operation/property_operations.c
        src/typedef/operation/codec/action_operation_codec.c
        src/typedef/operation/codec/property_operations_codec.c
        src/typedef/product/product.c
        src/typedef/product/operator/product_operator.c
        src/typedef/xid/codec/xid3_codec.c

        # Stanza: IQ and message model
        src/stanza/stanza.c
        src/stanza/iq/iq.c
        src/stanza/iq/iq_error.c
        src/stanza/iq/iq_error_factory.c
        src/stanza/iq/iq_method.c
        src/stanza/iq/iq_query.c
        src/stanza/iq/iq_result.c
        src/stanza/iq/basic/factory/bye_factory.c
        src/stanza/iq/basic/factory/hello_factory.c
        src/stanza/iq/basic/factory/ping_factory.c
        src/stanza/iq/basic/factory/pong_factory.c
        src/stanza/iq/device/control/get_properties.c
        src/stanza/iq/device/control/get_property.c
        src/stanza/iq/device/control/invoke_action.c
        src/stanza/iq/device/control/set_properties.c
        src/stanza/iq/device/control/set_property.c
        src/stanza/iq/device/control/factory/get_properties_factory.c
        src/stanza/iq/device/control/factory/get_property_factory.c
        src/stanza/iq/device/control/factory/invoke_action_factory.c
        src/stanza/iq/device/control/factory/set_properties_factory.c
        src/stanza/iq/device/control/factory/set_property_factory.c
        src/stanza/iq/device/key/get_access_key.c
        src/stanza/iq/device/key/set_access_key.c
        src/stanza/iq/device/key/factory/get_access_key_factory.c
        src/stanza/iq/device/key/factory/set_access_key_factory.c
        src/stanza/iq/device/verify/initialize.c
        src/stanza/iq/device/verify/verify_finish.c
        src/stanza/iq/device/verify/verify_start.c
        src/stanza/iq/device/verify/factory/initialize_factory.c
        src/stanza/iq/device/verify/factory/verify_finish_factory.c
        src/stanza/iq/device/verify/factory/verify_start_factory.c
        src/stanza/message/message.c

        # StanzaCodec: JSON codecs
        src/codec/stanza_codec.c
        src/codec/iq/device/control/invoke_actions_codec.c
        src/codec/iq/device/manager/get_children_codec.c
        src/codec/iq/device/manager/properties_changed_codec.c
        src/codec/iq/iq_codec.c
        src/codec/iq/iq_error_codec.c
        src/codec/iq/iq_query_codec.c
        src/codec/iq/iq_result_codec.c
        src/codec/iq/device/control/get_properties_codec.c
        src/codec/iq/device/control/get_property_codec.c
        src/codec/iq/device/control/invoke_action_codec.c
        src/codec/iq/device/control/set_properties_codec.c
        src/codec/iq/device/control/set_property_codec.c
        src/codec/iq/device/key/get_access_key_codec.c
        src/codec/iq/device/key/set_access_key_codec.c
        src/codec/iq/device/verify/initialize_codec.c
        src/codec/iq/device/verify/verify_finish_codec.c
        src/codec/iq/device/verify/verify_start_codec.c
        src/codec/message/message_codec.c

        # Shared helpers
        src/util/json_util.c)

add_library(tinyspec ${TINYSPEC_SOURCES})
add_library(tinyspec::tinyspec ALIAS tinyspec)

set_target_properties(tinyspec PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS ON
        POSITION_INDEPENDENT_CODE ON
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(tinyspec
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(tinyspec PUBLIC tinycore::tinycore)

# The export macro needs to know whether the library is built as a DLL.
if(BUILD_SHARED_LIBS)
    target_compile_definitions(tinyspec
            PRIVATE TINYSPEC_EXPORTS
            PUBLIC TINYSPEC_SHARED)
    set_target_properties(tinyspec PROPERTIES
            C_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN YES)
endif()

if(TINYSPEC_WARNINGS_AS_ERRORS)
    target_compile_definitions(tinyspec PRIVATE TINYSPEC_WARNINGS_AS_ERRORS=1)
endif()

# ---------------------------------------------------------------------------
# Compiler warnings
# ---------------------------------------------------------------------------
if(MSVC)
    target_compile_options(tinyspec PRIVATE /W4)
    if(TINYSPEC_WARNINGS_AS_ERRORS)
        target_compile_options(tinyspec PRIVATE /WX)
    endif()
else()
    target_compile_options(tinyspec PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wcast-qual
            -Wformat=2
            -Wundef
            -Wpointer-arith
            -Wstrict-prototypes
            -Wmissing-prototypes)
    if(TINYSPEC_WARNINGS_AS_ERRORS)
        target_compile_options(tinyspec PRIVATE -Werror)
    endif()
endif()

if(TINYSPEC_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(tinyspec PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(tinyspec PRIVATE -fsanitize=address,undefined)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(TINYSPEC_BUILD_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

# ---------------------------------------------------------------------------
# Install / package config
# ---------------------------------------------------------------------------
if(TINYSPEC_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS tinyspec
            EXPORT tinyspecTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(DIRECTORY include/tinyspec
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")

    install(EXPORT tinyspecTargets
            FILE tinyspecTargets.cmake
            NAMESPACE tinyspec::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyspec)

    configure_package_config_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyspecConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/tinyspecConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyspec)

    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/tinyspecConfigVersion.cmake
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion)

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/tinyspecConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/tinyspecConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyspec)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyspec.pc.in
            ${CMAKE_CURRENT_BINARY_DIR}/tinyspec.pc @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tinyspec.pc
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()
