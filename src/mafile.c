#include "mafile.h"

mafile_obj *read_mafile(const char *fpath)
{
    FILE *fptr = fopen(fpath, "r");
    char fcontents[3072];

    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    
    if (fsize >= 3071)
    {
        fclose(fptr);
        return NULL;
    }

    rewind(fptr);
    fread(fcontents, fsize, 1, fptr);
    fcontents[fsize] = '\0';
    fclose(fptr);

    mafile_obj *obj = (mafile_obj *)malloc(sizeof(mafile_obj));

    if (NULL == obj)
        return NULL;

    memset(obj, 0, sizeof(mafile_obj));

    cJSON *json;
    if ((json = cJSON_ParseWithLength(fcontents, fsize)) == NULL)
        return NULL;

    cJSON *j_shared_secret = cJSON_GetObjectItemCaseSensitive(json, "shared_secret");
    if (cJSON_IsString(j_shared_secret) && NULL != j_shared_secret->valuestring)
        obj->shared_secret = strdup(j_shared_secret->valuestring);

    cJSON *j_account_name = cJSON_GetObjectItemCaseSensitive(json, "account_name");
    if (cJSON_IsString(j_account_name) && NULL != j_account_name->valuestring)
        obj->account_name = strdup(j_account_name->valuestring);

    do
    {
        cJSON *j_Session = cJSON_GetObjectItemCaseSensitive(json, "Session");
        if (!cJSON_IsObject(j_Session))
            break;

        obj->Session = (mafile_session_obj *)malloc(sizeof(mafile_session_obj));
        if (NULL == obj->Session)
            break;
        
        memset(obj->Session, 0, sizeof(mafile_session_obj));

        cJSON *j_SteamID = cJSON_GetObjectItemCaseSensitive(j_Session, "SteamID");
        if (cJSON_IsNumber(j_SteamID))
            obj->Session->SteamID = j_SteamID->valueint;

    } while (0);

    cJSON_free(json);

    return obj;
} // read_mafile

void free_mafile_obj(mafile_obj *obj)
{
    if (NULL == obj)
        return;
    
    if (obj->shared_secret)
        free(obj->shared_secret);

    if (obj->account_name)
        free(obj->account_name);

    if (NULL == obj->Session)
    {
        free(obj);
        return;
    }

    free(obj->Session);
    free(obj);
} // free_mafile_obj
