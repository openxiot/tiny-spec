#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/action_operations.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    ActionOperations        actions;   /* ActionOperation* */
} QueryInvokeActions;

TINYSPEC_API
TinyRet QueryInvokeActions_Construct(QueryInvokeActions *thiz);
TINYSPEC_API
void QueryInvokeActions_Dispose(QueryInvokeActions *thiz);

typedef struct
{
    ActionOperations        actions;   /* ActionOperation* */
} ResultInvokeActions;

TINYSPEC_API
TinyRet ResultInvokeActions_Construct(ResultInvokeActions *thiz);
TINYSPEC_API
void ResultInvokeActions_Dispose(ResultInvokeActions *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_H */
