#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "config.h"

#define BINNAME             "bankid-se"
#define RELEASE_TIME        1245574966

#define LIB_PATH            EPREFIX "/lib/" BINNAME
#define SHARE_PATH          PREFIX "/share/" BINNAME

#define SIGNING_EXECUTABLE  LIB_PATH "/sign"
#define UI_PATH             SHARE_PATH "/ui"
#define UI_GTK_XML          UI_PATH "/sign.xml"
#define NPAPI_PLUGIN_LIB    LIB_PATH "/libplugins.so"

#endif
