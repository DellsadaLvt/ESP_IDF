#ifndef __USER_JSON_H_
#define __USER_JSON_H_


#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"

// parse for simple json 
esp_err_t parse_json(const char *json_data, const char *token, char *output);
// parse with object json, ex: parse the feeds in response message from thingspeak
esp_err_t parse_json_multi_obj( char *json, char *parent_token, char *child_token, char *out);
// ex: parse for the channel in message response from thingspeak
esp_err_t parse_json_multi_obj_2( char *json, char *parent_token, char *child_token, char *out);

#ifdef __cplusplus
}
#endif

#endif  /* __USER_JSON_H_ */

