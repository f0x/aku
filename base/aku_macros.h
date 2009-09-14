#include <kdemacros.h>
#include <KPluginFactory>
#include <KPluginLoader>

#define AKU_PLUGIN_EXPORT( c ) \
    K_PLUGIN_FACTORY( AkuFactory, registerPlugin< c >(); ) \
    K_EXPORT_PLUGIN( AkuFactory("c") )

#ifndef AKU_EXPORT
# if defined(MAKE_AKUCORE_LIB)
/* We are building this library */
#  define AKU_EXPORT KDE_EXPORT
# else
/* We are using this library */
#  define AKU_EXPORT KDE_IMPORT
# endif
#endif
