#include <tinyspec/stanza/iq/device/manager/properties_changed.h>

TINYCORE_RET QueryPropertiesChanged_Construct(QueryPropertiesChanged *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void QueryPropertiesChanged_Dispose(QueryPropertiesChanged *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}

TINYCORE_RET ResultPropertiesChanged_Construct(ResultPropertiesChanged *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void ResultPropertiesChanged_Dispose(ResultPropertiesChanged *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}
