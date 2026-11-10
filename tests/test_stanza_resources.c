/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include "tiny_test.h"

#include <stdio.h>
#include <string.h>

#include <tinycore/json_decoder.h>
#include <tinycore/json_object.h>
#include <tinyspec/tinyspec.h>

#ifndef TINYSPEC_TEST_RESOURCE_DIR
#  error "TINYSPEC_TEST_RESOURCE_DIR must be defined by the build system"
#endif

typedef struct
{
    const char *path;
    StanzaType type;
} ResourceCase;

/* Every JSON document in tests/resources/stanza. */
static const ResourceCase CASES[] = {
    /* IQ: basic */
    { "iq/basic/byebye/query.json", STANZA_IQ },
    { "iq/basic/byebye/result.json", STANZA_IQ },
    { "iq/basic/hello/query.json", STANZA_IQ },
    { "iq/basic/hello/result.json", STANZA_IQ },
    { "iq/basic/ping/query.json", STANZA_IQ },
    { "iq/basic/ping/result.json", STANZA_IQ },

    /* IQ: device/control */
    { "iq/device/control/get-properties/query.json", STANZA_IQ },
    { "iq/device/control/get-properties/result.json", STANZA_IQ },
    { "iq/device/control/get-property/query.json", STANZA_IQ },
    { "iq/device/control/get-property/result.json", STANZA_IQ },
    { "iq/device/control/set-properties/query.json", STANZA_IQ },
    { "iq/device/control/set-properties/result.json", STANZA_IQ },
    { "iq/device/control/set-property/query.json", STANZA_IQ },
    { "iq/device/control/set-property/result.json", STANZA_IQ },

    /* IQ: device/key */
    { "iq/device/key/get/query.json", STANZA_IQ },
    { "iq/device/key/get/result.json", STANZA_IQ },
    { "iq/device/key/set/query.json", STANZA_IQ },
    { "iq/device/key/set/result.json", STANZA_IQ },

    /* IQ: device/verify */
    { "iq/device/verify/initialize/query.json", STANZA_IQ },
    { "iq/device/verify/initialize/result.json", STANZA_IQ },
    { "iq/device/verify/verify-finished/query.json", STANZA_IQ },
    { "iq/device/verify/verify-finished/result.json", STANZA_IQ },
    { "iq/device/verify/verify-start/query.json", STANZA_IQ },
    { "iq/device/verify/verify-start/result.json", STANZA_IQ },

    /* IQ: error and node/control */
    { "iq/error.json", STANZA_IQ },
    { "iq/node/get-nodes/query.json", STANZA_IQ },
    { "iq/node/get-nodes/result.json", STANZA_IQ },
    { "iq/node/get-children/query.json", STANZA_IQ },
    { "iq/node/get-children/result.json", STANZA_IQ },

    /* Messages */
    { "message/device/device-event-occurred.json", STANZA_MESSAGE },
    { "message/device/device-property-changed.json", STANZA_MESSAGE },
    { "message/device/device-properties-changed.json", STANZA_MESSAGE },
    { "message/device/device-properties-notify.json", STANZA_MESSAGE },
    { "message/node/node-found.json", STANZA_MESSAGE },
    { "message/node/node-lost.json", STANZA_MESSAGE },
    { "message/owner/ownership-taken.json", STANZA_MESSAGE },
    { "message/owner/ownership-disclaimed.json", STANZA_MESSAGE },
    { "message/connection/status-changed.json", STANZA_MESSAGE },
};

static size_t read_file(const char *path, char *buffer, size_t capacity)
{
    FILE *file = NULL;
    size_t size = 0;

    file = fopen(path, "rb");
    if (file == NULL)
    {
        return 0;
    }

    size = fread(buffer, 1, capacity - 1, file);
    buffer[size] = '\0';

    fclose(file);

    return size;
}

static void check_resource(const ResourceCase *test_case)
{
    char path[512];
    char buffer[32 * 1024];
    size_t size = 0;
    JsonDecoder decoder;
    JsonDecoderStatus status = JSON_DECODER_STATUS_INVALID;
    JsonObject *object = NULL;
    JsonObject *encoded = NULL;
    Stanza *stanza = NULL;

    snprintf(path, sizeof(path), "%s/%s", TINYSPEC_TEST_RESOURCE_DIR, test_case->path);

    size = read_file(path, buffer, sizeof(buffer));
    TINY_TEST_CHECK_MSG(size > 0, "cannot read %s", path);
    if (size == 0)
    {
        return;
    }

    TINY_TEST_CHECK(JsonDecoder_Construct(&decoder) == TINY_RET_OK);

    object = JsonDecoder_Parse(&decoder, buffer, &status);
    JsonDecoder_Dispose(&decoder);

    TINY_TEST_CHECK_MSG(object != NULL, "%s: JSON decode failed (status %d)",
                        test_case->path, (int) status);
    if (object == NULL)
    {
        return;
    }

    stanza = StanzaCodec_Decode(object);
    TINY_TEST_CHECK_MSG(stanza != NULL, "%s: StanzaCodec_Decode failed", test_case->path);
    if (stanza == NULL)
    {
        JsonObject_Delete(object);
        return;
    }

    TINY_TEST_CHECK_MSG(stanza->type == test_case->type,
                        "%s: stanza type %d, expected %d",
                        test_case->path, (int) stanza->type, (int) test_case->type);

    encoded = StanzaCodec_Encode(stanza);
    TINY_TEST_CHECK_MSG(encoded != NULL, "%s: StanzaCodec_Encode failed", test_case->path);
    if (encoded != NULL)
    {
        TINY_TEST_CHECK_MSG(JsonObject_Equals(object, encoded),
                            "%s: round trip changed the document", test_case->path);
        JsonObject_Delete(encoded);
    }

    Stanza_Delete(stanza);
    JsonObject_Delete(object);
}

int main(void)
{
    size_t i = 0;

    TINY_TEST_BEGIN("stanza_resources");

    for (i = 0; i < sizeof(CASES) / sizeof(CASES[0]); ++i)
    {
        check_resource(&CASES[i]);
    }

    return TINY_TEST_FINISH();
}
