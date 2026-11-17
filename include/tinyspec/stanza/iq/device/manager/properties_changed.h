#ifndef TINYSPEC_STANZA_IQ_DEVICE_MANAGER_PROPERTIES_CHANGED_H
#define TINYSPEC_STANZA_IQ_DEVICE_MANAGER_PROPERTIES_CHANGED_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/property_operations.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    PropertyOperations      properties;
} QueryPropertiesChanged;

TINYSPEC_API
TINYCORE_RET QueryPropertiesChanged_Construct(QueryPropertiesChanged *thiz);
TINYSPEC_API
void QueryPropertiesChanged_Dispose(QueryPropertiesChanged *thiz);

typedef struct
{
    PropertyOperations      properties;
} ResultPropertiesChanged;

TINYSPEC_API
TINYCORE_RET ResultPropertiesChanged_Construct(ResultPropertiesChanged *thiz);
TINYSPEC_API
void ResultPropertiesChanged_Dispose(ResultPropertiesChanged *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_MANAGER_PROPERTIES_CHANGED_H */
