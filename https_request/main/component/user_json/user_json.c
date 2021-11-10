#include "user_json.h"
#include "unity_config.h"
#include "unity.h"
#include "common.h"

esp_err_t parse_json(const char *json_data, const char *token, char *output ){
    cJSON *parse_json = cJSON_Parse(json_data);
    if (parse_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }

        cJSON_Delete(parse_json);
        return ESP_FAIL;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(parse_json, token);
    if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        strcpy(output, name->valuestring);
    }
    else{
        printf("Token not found\n");
    }

    cJSON_Delete(parse_json);
    return ESP_OK;
}


