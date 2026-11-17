#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/typedef/operation/action_operation.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    TinyList        actions;   /* ActionOperation* */
} QueryInvokeActions;

TINYSPEC_API
TINYCORE_RET QueryInvokeActions_Construct(QueryInvokeActions *thiz);
TINYSPEC_API
void QueryInvokeActions_Dispose(QueryInvokeActions *thiz);

typedef struct
{
    TinyList        actions;   /* ActionOperation* */
} ResultInvokeActions;

TINYSPEC_API
TINYCORE_RET ResultInvokeActions_Construct(ResultInvokeActions *thiz);
TINYSPEC_API
void ResultInvokeActions_Dispose(ResultInvokeActions *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_H */
