#ifndef __PARSER_H__
#define __PARSER_H__

#include "../../common/network/request_type.h"
#include "../../common/network/request.h"
#include "../../common/debug/trace.h"
#include "../../common/map/item/item_type.h"

#include <stdio.h>
#include <stdlib.h>

request *parse_cmd_args(int argc, char **argv);

#endif