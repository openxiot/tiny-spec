/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_FACTORY_INVOKE_ACTIONS_FACTORY_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_FACTORY_INVOKE_ACTIONS_FACTORY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/action_operations.h>
#include <tinyspec/stanza/stanza.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
Stanza * ResultInvokeActions_New(const char *id, ActionOperations *operations);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_FACTORY_INVOKE_ACTIONS_FACTORY_H */