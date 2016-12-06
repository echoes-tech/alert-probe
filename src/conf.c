/* 
 * Alert - Probe is a collect probe, part of the Alert software
 * Copyright (C) 2013 Florent Poinsaut, Thomas Saquet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#include "conf.h"

int loadConf(Conf *conf, const char *confPath)
{
    GKeyFile *keyFile = g_key_file_new();
    GError *error = NULL;
    char *value;
    
    /* Open file */
    if (!g_key_file_load_from_file(keyFile, confPath, G_KEY_FILE_NONE, &error))
    {
        g_critical("Could not open file '%s': %s\n", confPath, error->message);
        return EXIT_FAILURE;
    }

    /* Get probe_id */
    conf->probeID = g_key_file_get_integer(keyFile, "probe", "probe_id", &error);
    if (error != NULL)
    {
        g_critical("%s: %s\n", confPath, error->message);
        return EXIT_FAILURE;
    }

    /* Get token */
    value = g_key_file_get_string(keyFile, "probe", "token", &error);
    if (value == NULL)
    {
        g_critical("%s: %s\n", confPath, error->message);
        return EXIT_FAILURE;
    }
    else
    {
        strcpy(conf->token, value);
    }

    /* Get probe_plugin_dir */
    value = g_key_file_get_string(keyFile, "probe", "probe_plugin_dir", &error);
    if (value == NULL)
    {
#ifdef NDEBUG
        strcpy(conf->probePluginDir, "/opt/echoes-alert/probe/etc/plugins/");
#else
        strcpy(conf->probePluginDir, "./plugins/");
#endif
        g_warning("%s: %s\n", confPath, error->message);
        error = NULL;
    }
    else
    {
#ifdef NDEBUG
        strcpy(conf->probePluginDir, value);
#else
        strcpy(conf->probePluginDir, "./plugins/");
#endif
    }

    /* Get transport_proto */
    conf->transportProto = g_key_file_get_integer(keyFile, "transport", "transport_proto", &error);
    if (error != NULL)
    {
        conf->transportProto = 0;
        g_warning("%s: %s\n", confPath, error->message);
        error = NULL;
    }

    /* Get transport_message_version */
    conf->transportMsgVersion = g_key_file_get_integer(keyFile, "transport", "transport_message_version", &error);
    if (error != NULL)
    {
        conf->transportMsgVersion = 2;
        g_warning("%s: %s\n", confPath, error->message);
        error = NULL;
    }

    /* Get engine_fqdn */
    value = g_key_file_get_string(keyFile, "engine", "engine_fqdn", &error);
    if (value == NULL)
    {
        strcpy(conf->engineFQDN, "alert-engine.echoes-tech.com");
        g_warning("%s: %s\n", confPath, error->message);
        error = NULL;
    }
    else
    {
        strcpy(conf->engineFQDN, value);
    }

    /* Get engine_port */
    conf->enginePort = g_key_file_get_integer(keyFile, "engine", "engine_port", &error);
    if (error != NULL)
    {
        conf->enginePort = 443;
        g_warning("%s: %s\n", confPath, error->message);
    }

    return EXIT_SUCCESS;
}

