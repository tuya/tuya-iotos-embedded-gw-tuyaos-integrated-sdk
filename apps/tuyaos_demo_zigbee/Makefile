ROOT_DIR ?= $(abspath ../../)
COMPILE_PREX ?=
USER_LINK_FLAGS ?=

-include $(ROOT_DIR)/build/tuya_iot.config

AR = $(COMPILE_PREX)ar
CC = $(COMPILE_PREX)gcc
NM = $(COMPILE_PREX)nm
CPP = $(COMPILE_PREX)g++
OBJCOPY = $(COMPILE_PREX)objcopy
OBJDUMP = $(COMPILE_PREX)objdump
APP_PACK = ./build/pack.sh

LINKFLAGS  = -L$(ROOT_DIR)/libs $(addprefix -l:,$(notdir $(shell find $(ROOT_DIR)/libs -name "*.a"))) -lpthread -lm -ldl
LINKFLAGS := -Wl,--start-group $(LINKFLAGS) -Wl,--end-group


CCFLAGS = -g -fPIC

DEFINES = -DAPP_BIN_NAME=\"$(APP_BIN_NAME)\" \
		  -DUSER_SW_VER=\"$(USER_SW_VER)\"

CFLAGS = $(CCFLAGS) $(DEFINES) $(USER_INCS) $(USER_CFLAGS)

# 编译输出变量
OUTPUT_DIR = $(ROOT_DIR)/output/$(APP_BIN_NAME)_$(USER_SW_VER)
OUTPUT_DIR_OBJS = $(OUTPUT_DIR)/.objs

# 源代码和头文件路径
USER_SRC_BASE_DIR  = ./src
USER_SRC_BASE_DIR += ./tuyaos_adapter
USER_INC_BASE_DIR  = ./include
USER_INC_BASE_DIR += $(shell find $(ROOT_DIR)/include -name include -type d)
USER_INC_BASE_DIR += $(shell find $(ROOT_DIR)/adapter -name include -type d)

# 源代码和头文件
USER_SRC_DIRS = $(shell find $(USER_SRC_BASE_DIR) -type d)
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.c)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.cpp)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.s)) 
USER_SRCS += $(foreach dir, $(USER_SRC_DIRS), $(wildcard $(dir)/*.S)) 
USER_INCS = $(addprefix -I ,  $(shell find $(USER_INC_BASE_DIR) -type d) )

# 编译目标文件
USER_OBJS = $(addsuffix .o, $(USER_SRCS))
USER_OBJS_OUT =  $(addprefix $(OUTPUT_DIR_OBJS)/, $(USER_OBJS))

all: pack

pack: build_app
ifeq ($(APP_PACK),$(wildcard $(APP_PACK)))
	sh $(APP_PACK) $(OUTPUT_DIR) $(ROOT_DIR) $(APP_BIN_NAME) $(USER_SW_VER) $(COMPILE_PREX)
endif

build_app: $(USER_OBJS_OUT)
	$(CC) $(USER_OBJS_OUT) $(CFLAGS) $(LINKFLAGS) -o $(OUTPUT_DIR)/$(APP_BIN_NAME)
	@cp config.json $(OUTPUT_DIR)
	@echo "Build APP Finish"

$(OUTPUT_DIR_OBJS)/%.c.o: %.c
	@mkdir -p $(dir $@);
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUT_DIR_OBJS)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@);
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUT_DIR_OBJS)/%.s.o: %.s
	@mkdir -p $(dir $@);
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUT_DIR_OBJS)/%.S.o: %.S
	@mkdir -p $(dir $@);
	$(CC) $(CFLAGS) -D__ASSEMBLER__ -o $@ -c $<


.PHONY: all clean SHOWARGS build_app pack
clean:
	rm -rf $(OUTPUT_DIR)
