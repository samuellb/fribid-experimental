#define _BSD_SOURCE 1
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "plugin.h"

Plugin *plugin_new(PluginType pluginType, const char *url,
                   const char *hostname, const char *ip) {
    Plugin *plugin = calloc(1, sizeof(Plugin));
    plugin->type = pluginType;
    plugin->url = strdup(url);
    plugin->hostname = strdup(hostname);
    plugin->ip = strdup(ip);
    return plugin;
}

void plugin_free(Plugin *plugin) {
    switch (plugin->type) {
        case PT_Version:
            break;
        case PT_Authentication:
            free(plugin->info.auth.challenge);
            free(plugin->info.auth.policys);
            free(plugin->info.auth.signature);
            break;
        case PT_Signer:
            free(plugin->info.sign.challenge);
            free(plugin->info.sign.policys);
            free(plugin->info.sign.subjectFilter);
            free(plugin->info.sign.message);
            free(plugin->info.sign.signature);
            break;
    }
    free(plugin->url);
    free(plugin->hostname);
    free(plugin->ip);
    free(plugin);
}


static char **getCommonParamPointer(Plugin *plugin, const char *name) {
    if (!strcmp(name, "Policys")) return &plugin->info.auth.policys;
    if (!strcmp(name, "Signature")) return &plugin->info.auth.signature;
    return NULL;
}

static char **getParamPointer(Plugin *plugin, const char *name) {
    switch (plugin->type) {
        case PT_Authentication:
            if (!strcmp(name, "Challenge")) return &plugin->info.auth.challenge;
            return getCommonParamPointer(plugin, name);
        case PT_Signer:
            if (!strcmp(name, "Nonce")) return &plugin->info.sign.challenge;
            if (!strcmp(name, "Subjects")) return &plugin->info.sign.subjectFilter;
            if (!strcmp(name, "TextToBeSigned")) return &plugin->info.sign.message;
            return getCommonParamPointer(plugin, name);
        default:
            return NULL;
    }
}

char *sign_getParam(Plugin *plugin, const char *name) {
    char **valuePtr = getParamPointer(plugin, name);
    
    char *value = NULL;
    if (*valuePtr) value = strdup(*valuePtr);
    
    return (value != NULL ? value : strdup(""));
}

bool sign_setParam(Plugin *plugin, const char *name, const char *value) {
    char **valuePtr = getParamPointer(plugin, name);
    
    if (valuePtr == NULL) return false;
    
    free(*valuePtr);
    *valuePtr = strdup(value);
    return true;
}

int sign_performAction(Plugin *plugin, const char *action) {
    if ((plugin->type == PT_Authentication) && !strcmp(action, "Authenticate")) {
        return sign_performAction_Authenticate(plugin);
    } else if ((plugin->type == PT_Signer) && !strcmp(action, "Sign")) {
        return sign_performAction_Sign(plugin);
    } else {
        return 1;
    }
}

int sign_getLastError(Plugin *plugin) {
    return plugin->lastError;
}

