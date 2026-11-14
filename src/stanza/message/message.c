/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/message/message.h>

#define STRING_MESSAGE_TOPIC_UNDEFINED      "undefined"
#define STRING_MESSAGE_TOPIC_DEVICE         "urn:xiot:device"

const char * message_topic_to_string(MessageTopic topic)
{
    switch (topic)
    {
        case MESSAGE_TOPIC_DEVICE:
            return STRING_MESSAGE_TOPIC_DEVICE;

        default:
            return STRING_MESSAGE_TOPIC_UNDEFINED;
    }
}

MessageTopic message_topic_from_string(const char *string)
{
    RETURN_VAL_IF_FAIL(string, MESSAGE_TOPIC_UNDEFINED);

    if (STR_EQUAL(STRING_MESSAGE_TOPIC_DEVICE, string))
    {
        return MESSAGE_TOPIC_DEVICE;
    }

    return MESSAGE_TOPIC_UNDEFINED;
}
