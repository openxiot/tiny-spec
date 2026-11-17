#ifndef TINYSPEC_STANZA_IQ_DEVICE_MANAGER_GET_CHILDREN_H
#define TINYSPEC_STANZA_IQ_DEVICE_MANAGER_GET_CHILDREN_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>
#include <tinycore/json_object.h>
#include <tinycore/json_string.h>
#include <tinyspec/typedef/product/child/child.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    JsonString     * did;
} QueryGetChildren;

TINYSPEC_API
TINYCORE_RET QueryGetChildren_Construct(QueryGetChildren *thiz);
TINYSPEC_API
void QueryGetChildren_Dispose(QueryGetChildren *thiz);

typedef struct
{
    TinyList        children;   /* Child* */
} ResultGetChildren;

TINYSPEC_API
TINYCORE_RET ResultGetChildren_Construct(ResultGetChildren *thiz);
TINYSPEC_API
void ResultGetChildren_Dispose(ResultGetChildren *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_MANAGER_GET_CHILDREN_H */
