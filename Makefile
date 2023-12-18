CFLAGS := -W -g
CPPFLAGS := -Iinclude -MMD -DP

BASE_BUILD_DIR:=build
BASE_SRC_DIR:=src
BASE_INC_DIR:=./include
BASE_OBJ_DIR:=./build/obj
BIN_DIR:=./build/bin

COMMON_SRC_DIR:=$(BASE_SRC_DIR)/common/
CLIENT_SRC_DIR:=$(BASE_SRC_DIR)/client/
SERVER_SRC_DIR:=$(BASE_SRC_DIR)/server/

COMMON_INC_DIR:=$(BASE_INC_DIR)/common/
CLIENT_INC_DIR:=$(BASE_INC_DIR)/client/
SERVER_INC_DIR:=$(BASE_INC_DIR)/server/


COMMON_OBJ_DIR:=$(BASE_OBJ_DIR)/common/
CLIENT_OBJ_DIR:=$(BASE_OBJ_DIR)/client/
SERVER_OBJ_DIR:=$(BASE_OBJ_DIR)/server/


COMMON_SRCS := $(shell find $(COMMON_SRC_DIR) -type f -name '*.c')
COMMON_HEADERS := $(shell find $(COMMON_INC_DIR) -type f -name '*.h')
COMMON_OBJS := $(COMMON_SRCS:.c=.o)


CLIENT_HEADERS := $(shell find $(CLIENT_INC_DIR) -type f -name '*.h') $(COMMON_HEADERS)
SERVER_HEADERS := $(shell find $(SERVER_INC_DIR) -type f -name '*.h') $(COMMON_HEADERS)


# CLIENT_MAIN = $(addprefix $(CLIENT_SRC_DIR), main.c)
# SERVER_MAIN = $(addprefix $(SERVER_SRC_DIR), main.c)

# CLIENT_MAIN_OBJ = $(addprefix $(CLIENT_OBJ_DIR), $(patsubst %.c, %.o, $(notdir $(CLIENT_MAIN))))
# SERVER_MAIN_OBJ = $(addprefix $(CLIENT_OBJ_DIR), $(patsubst %.c, %.o, $(notdir $(SERVER_MAIN))))


# CLIENT_SRCS = $(filter-out $(CLIENT_MAIN), $(shell find $(CLIENT_SRC_DIR) -type f -name '*.c')) $(COMMON_SRCS)
# SERVER_SRCS = $(filter-out $(SERVER_MAIN), $(shell find $(SERVER_SRC_DIR) -type f -name '*.c')) $(COMMON_SRCS)



CLIENT_SRCS := $(shell find $(CLIENT_SRC_DIR) -type f -name '*.c') $(COMMON_SRCS)
SERVER_SRCS := $(shell find $(SERVER_SRC_DIR) -type f -name '*.c') $(COMMON_SRCS)


CLIENT_OBJS := $(addprefix $(CLIENT_OBJ_DIR), $(patsubst %.c, %.o, $(notdir $(CLIENT_SRCS))))
SERVER_OBJS := $(addprefix $(SERVER_OBJ_DIR), $(patsubst %.c, %.o, $(notdir $(SERVER_SRCS))))


# $(info $$SERVER [${SERVER_SRCS}])

# $(info $$COMMON [${COMMON_OBJS}])
# $(info $$SERVER_OBJS [${SERVER_OBJS}])



server: $(SERVER_SRCS) $(SERVER_HEADERS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@


client: $(CLIENT_SRCS) $(CLIENT_HEADERS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@


clean:
	rm -rf $(BASE_BUILD_DIR)


.PHONY: clean 

-include $(SERVER_OBJS:.o=.d)




