/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_MESSAGE_MESSAGE_H
#define TINYSPEC_STANZA_MESSAGE_MESSAGE_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/notice/notice.h>

TINYSPEC_BEGIN_DECLS

#define MESSAGE_ID_LENGTH       64

typedef enum
{
    MESSAGE_TOPIC_UNDEFINED = 0,
    MESSAGE_TOPIC_DEVICE    = 1,
} MessageTopic;

typedef struct
{
    char                    id[MESSAGE_ID_LENGTH + 1];
    MessageTopic            topic;
    int                     type;
    Notice                  payload;
} Message;

TINYSPEC_API
const char * message_topic_to_string(MessageTopic topic);
TINYSPEC_API
MessageTopic message_topic_from_string(const char *string);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_MESSAGE_MESSAGE_H */
