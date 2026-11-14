# TinySpec

TinySpec is a small, portable C99 library that describes xiot devices and
the JSON stanzas they exchange. It contains the device model (products,
devices, services, properties, actions, events), the IQ/message stanza
model with its factories, and JSON codecs for both.

TinySpec depends on [TinyCore](https://github.com/openxiot/tiny-core) for
allocation, containers, JSON and logging. It has no other dependencies
beyond the C99 standard library.

## Layout

```
include/tinyspec/   public headers, grouped by module
src/                implementation, mirroring the include tree
tests/              self contained test suite (CTest) and JSON fixtures
cmake/              package config templates
```

Every header is installed under `tinyspec/`, so users include either one
module, e.g. `#include <tinyspec/stanza_codec/stanza_codec.h>`, or
everything at once:

```c
#include <tinyspec/tinyspec.h>
```

## Building

Requirements: CMake 3.16 or newer, a C99 compiler, and a TinyCore source
tree or installation.

By default the sibling directory `../tiny-core` is used:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

For convenience `scripts/build.sh` configures, builds and runs the suite in one
step, and drives the sanitizer build below:

```sh
./scripts/build.sh        # default build  -> build/
./scripts/build.sh asan   # sanitizer build -> build-asan/
./scripts/build.sh shared # shared library  -> build-shared/
```

To use an installed TinyCore instead, put it on the CMake search path
(`-DCMAKE_PREFIX_PATH=/some/prefix`) or point at a source tree explicitly:

```sh
cmake -S . -B build -DTINYSPEC_TINYCORE_DIR=/path/to/tiny-core
```

### CLion

Open the repository root in CLion. CLion detects `CMakeLists.txt`
automatically; select the `Debug` (or `Release`) CMake profile and reload.
Tests are registered with CTest, so they appear in the run configuration
list and can be executed one by one or through `ctest`.

### CMake options

| Option | Default | Meaning |
| --- | --- | --- |
| `BUILD_SHARED_LIBS` | `OFF` | Build a shared library instead of a static one. |
| `TINYSPEC_BUILD_TESTS` | top level only | Build the test suite. |
| `TINYSPEC_ENABLE_SANITIZERS` | `OFF` | Build with AddressSanitizer and UBSan. |
| `TINYSPEC_INSTALL` | top level only | Generate install and package config rules. |
| `TINYSPEC_WARNINGS_AS_ERRORS` | `OFF` | Promote compiler warnings to errors. |
| `TINYSPEC_TINYCORE_DIR` | `../tiny-core` | TinyCore source tree used when no installed package is found. |

Install and consume from another CMake project:

```sh
cmake --install build --prefix /some/prefix
```

```cmake
find_package(tinyspec CONFIG REQUIRED)
target_link_libraries(app PRIVATE tinyspec::tinyspec)
```

A pkg-config file (`tinyspec.pc`) is installed as well; it requires
`tinycore`.

## Modules

| Module | Headers | Contents |
| --- | --- | --- |
| IotTypedef | `iot_typedef/` | Device description model: product, device, service, property, action, event, argument, URN, access/format/unit and value constraints (list, range), XID3 identifiers, property/action/event operations and notices. |
| Stanza | `stanza/` | IQ query, result and error model, device messages (device/node notices), methods and one factory per stanza type. |
| StanzaCodec | `stanza_codec/` | JSON encode/decode for every stanza type, including the notice payloads. |
| Utilities | `json_util.h` | Shared JSON helpers (`JsonArray_CopyInto`). |

The JSON document shape is unchanged from the original tiny-spec project:

```json
{
    "iq": {
        "id": "5001",
        "type": "query",
        "method": "urn:xiot:get-properties",
        "content": {
            "properties": [
                { "pid": "1.2.3" }
            ]
        }
    }
}
```

## API conventions

* Types and functions are prefixed with their model name, e.g.
  `PropertyOperations_Construct`, `StanzaCodec_Decode`, `XID3_FromString`.
* Every model object supports the same lifecycle:
  `X_Construct()` / `X_Dispose()` for stack instances and
  `X_New()` / `X_Delete()` for heap instances.
* List based models (`Device`, `Service`, `Product`, ...) take ownership of
  the objects added to them; `X_Get*()` returns borrowed pointers. Delete
  listeners free the elements when the list is disposed.
* Errors are reported with `TINYCORE_RET` (an `int32_t` from TinyCore).
  Test for success with `RET_SUCCEEDED(ret)`; `RET_FAILED(ret)` is the
  inverse, and `TinyRet_GetDescription(ret)` renders a readable message.
* JSON builders follow the TinyCore ownership rules: `JsonObject_PutObject`
  and `JsonObject_PutArray` transfer ownership of the object or array,
  while `JsonObject_AddValue` stores a deep copy. Codecs that must not
  consume a model value (for example `SetPropertyCodec_EncodeQuery`) use
  `JsonObject_AddValue`.
* Exported symbols are marked with `TINYSPEC_API`; headers open and close
  with `TINYSPEC_BEGIN_DECLS` / `TINYSPEC_END_DECLS` so they can be used
  from C++.

### Portability notes

* C99 is required; the library builds cleanly with `-Wall -Wextra
  -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wcast-qual
  -Wformat=2 -Wundef -Wpointer-arith -Wstrict-prototypes
  -Wmissing-prototypes`.
* All allocations are routed through TinyCore's `tiny_malloc()` /
  `tiny_free()` wrappers, so a host can supply its own allocator.
* Only standard C headers and TinyCore are used; the tests avoid directory
  iteration and hard-code their resource file names, so they run on
  platforms without `<dirent.h>`.
* Shared builds hide all symbols that are not marked `TINYSPEC_API`
  (`C_VISIBILITY_PRESET hidden`), which keeps internal codec helpers out of
  the ABI.

## Tests

```sh
ctest --test-dir build --output-on-failure
```

For a sanitizer build, `scripts/build.sh asan` is the equivalent of:

```sh
cmake -S . -B build-asan -DTINYSPEC_ENABLE_SANITIZERS=ON
cmake --build build-asan
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    ctest --test-dir build-asan --output-on-failure
```

(`detect_leaks` is not supported by AddressSanitizer on macOS, so `build.sh`
turns it off there; on Linux it leaves it at the default, which also verifies
for memory leaks. `UBSAN_OPTIONS=halt_on_error=1` makes UBSan stop on the
first error instead of drowning it in the suite output.)

| Test | Contents |
| --- | --- |
| `test_json_util` | `JsonArray_CopyInto` ownership and error handling. |
| `test_model` | XID3 and URN parsing, operations, service/action/product model, stanza factories and IQ error encoding. |
| `test_typedef` | Format/access/URN-type string maps, `ValueList`/`ValueRange` constraints, `Property_CheckValue` for every format, read/write/notify/subscribe status codes, arguments, events, devices and services. |
| `test_operations` | `PropertyOperation`, `PropertyOperations`, `ActionOperation` and `EventOperation` construction, deep copy and self-copy handling, plus the property and action operation codecs (including invalid input). |
| `test_notice` | Notice type and message topic string maps, property-changed/event-occurred/node-info codecs, and complete device/node message stanzas through `StanzaCodec_Encode`/`StanzaCodec_Decode`. |
| `test_stanza` | IQ method strings, `IQ_InitializeQuery`/`IQ_InitializeResult`, every stanza factory, IQ error stanzas, encode/decode round trips for the key, verify, control and node methods, and malformed IQ documents. |
| `test_product_operator` | `Product_CheckHandler`, `Product_GetAllProperties`, read/write/invoke dispatch across services and child products (including every error status), property value changes with and without saving, changed-property snapshots and event production. |
| `test_stanza_resources` | Decodes every fixture under `tests/resources/stanza/`, re-encodes it and checks that the JSON document is unchanged. |

The fixtures are the ones shipped with the original tiny-spec project
(`test/StanzaCodec/resources`).

## License

MIT. See [LICENSE](LICENSE).
