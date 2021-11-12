#include "user_json.h"
#include "unity_config.h"
#include "unity.h"
#include "common.h"

static const char *TAG = "usr_json.c";

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

esp_err_t parse_json_multi_obj( char *json, char *parent_token, char *child_token, char *out){
    const cJSON *child_obj = NULL;
    const cJSON *parent_obj = NULL;
    
    esp_err_t status = ESP_FAIL;
    cJSON *json_obj = cJSON_Parse(json);
    if (json_obj == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            ESP_LOGE(TAG, "Error before: %s\n", error_ptr);
        }
        
        goto end;
    }

    parent_obj = cJSON_GetObjectItemCaseSensitive(json_obj, parent_token);
    //printf("parent obj======> %s\n",cJSON_Print(parent_obj));
    cJSON_ArrayForEach(child_obj, parent_obj)
    {
        //printf("child obj======> %s\n",cJSON_Print(child_obj));
        cJSON *parse = cJSON_GetObjectItemCaseSensitive(child_obj, child_token);

        if (cJSON_IsString(parse) && (parse->valuestring != NULL))
        {
            strcpy(out, parse->valuestring);

            status = ESP_OK;
        }
    }

end:
    cJSON_Delete(json_obj);
    return status;
}

esp_err_t parse_json_multi_obj_2( char *json, char *parent_token, char *child_token, char *out){
    const cJSON *parent_obj = NULL;
    
    esp_err_t status = ESP_FAIL;
    cJSON *json_obj = cJSON_Parse(json);
    if (json_obj == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            ESP_LOGE(TAG, "Error before: %s\n", error_ptr);
        }
        
        goto end;
    }

    parent_obj = cJSON_GetObjectItemCaseSensitive(json_obj, parent_token);
    //printf("parent obj======> %s\n",cJSON_Print(parent_obj));
   
    cJSON *parse = cJSON_GetObjectItemCaseSensitive(parent_obj, child_token);

        if (cJSON_IsString(parse) && (parse->valuestring != NULL))
        {
            strcpy(out, parse->valuestring);

            status = ESP_OK;
        }
    

end:
    cJSON_Delete(json_obj);
    return status;
}
