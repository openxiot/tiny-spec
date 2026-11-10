/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NODE_CHILD_H
#define TINYSPEC_IOT_TYPEDEF_NODE_CHILD_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

#define CHILD_DID_LENGTH            64
#define CHILD_TYPE_LENGTH           128
#define CHILD_PROTOCOL_LENGTH       32
#define CHILD_PARENT_ID_LENGTH      64

/*
 * One entry of a get-children result. On the wire a child is
 * {"did": "...", "summary": {"type" | "productId"/"productVersion",
 *                            "online", "protocol", "parentId"}}.
 * When `type` is not empty the summary carries the type URN; otherwise the
 * product id/version pair describes it.
 */
typedef struct
{
    char    did[CHILD_DID_LENGTH];
    char    type[CHILD_TYPE_LENGTH];
    int     productId;
    int     productVersion;
    bool    online;
    char    protocol[CHILD_PROTOCOL_LENGTH];
    char    parentId[CHILD_PARENT_ID_LENGTH];
} Child;

TINYSPEC_API
Child * Child_New(void);

TINYSPEC_API
void Child_Delete(Child *thiz);

TINYSPEC_API
TINYCORE_RET Child_Construct(Child *thiz);

TINYSPEC_API
void Child_Dispose(Child *thiz);

TINYSPEC_API
TINYCORE_RET Child_Copy(Child *dst, Child *src);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NODE_CHILD_H */
