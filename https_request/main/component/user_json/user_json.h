#ifndef __USER_JSON_H_
#define __USER_JSON_H_


#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"

esp_err_t parse_json(const char *json_data, const char *token, char *output);

#ifdef __cplusplus
}
#endif

#endif  /* __USER_JSON_H_ */

