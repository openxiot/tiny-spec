#include <tinyspec/stanza/iq/device/manager/properties_changed.h>

TinyRet QueryPropertiesChanged_Construct(QueryPropertiesChanged *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void QueryPropertiesChanged_Dispose(QueryPropertiesChanged *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}

TinyRet ResultPropertiesChanged_Construct(ResultPropertiesChanged *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void ResultPropertiesChanged_Dispose(ResultPropertiesChanged *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}
