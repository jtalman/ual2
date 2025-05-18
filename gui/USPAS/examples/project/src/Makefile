include $(UAL_USPAS)/Makefile.config

APP_DIR  = ..
SRC_DIR  = .
OBJ_DIR  = $(APP_DIR)/lib/$(UAL_ARCH)/obj

CCFLAGS += -I$(UAL_USPAS)/player/src
LDFLAGS += -L$(UAL_USPAS)/player/lib/$(UAL_ARCH) -lUspasPlayer

# Files

OBJS     = 	$(OBJ_DIR)/Tracker.o \
		$(OBJ_DIR)/Mapper.o

all:  $(APP_DIR)/$(UAL_ARCH)/compare

compile:  $(APP_DIR)/$(UAL_ARCH)/compare

clean:
	rm  -f  $(OBJ_DIR)/*


$(APP_DIR)/$(UAL_ARCH)/compare: $(OBJ_DIR)/compare.o $(OBJS)
	$(LD) -o $@  $(OBJ_DIR)/compare.o $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CC) $(CCFLAGS) -c $< -o $@




