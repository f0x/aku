#include <kdemacros.h>
#include <KPluginFactory>
#include <KPluginLoader>

#define AKU_PLUGIN_EXPORT( c ) \
    K_PLUGIN_FACTORY( AkuFactory, registerPlugin< c >(); ) \
    K_EXPORT_PLUGIN( AkuFactory("c") )
