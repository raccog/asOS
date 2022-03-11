include $(MAKE_DIR)/*.mk

STD_SRC_DIR = $(ROOT_DIR)/libs/std
STD_BIN_DIR = $(BIN_DIR)/$(TARGET)/std

STD_SRC = 	alloc.c \
			printer.c \
			printf.c \
			print_formatters.c \
			scanner.c
STD_H =		alloc.h \
			log.h \
			printer.h \
			printf.h \
			print_formatters.h \
			scanner.h \
			std.h
STD_OBJ = $(patsubst %, $(STD_BIN_DIR)/%.o, $(STD_SRC))

STD_CC = clang
STD_AR = llvm-ar
STD_CFLAGS = $(CFLAGS) -target $(TARGET) -std=c11 -I$(ROOT_DIR)/libs -D__LIB__='"STDLIB"'

STD_LIB = $(BIN_DIR)/$(TARGET)/libstd.a

all: $(STD_LIB)
.PHONY: all

$(STD_LIB): $(STD_OBJ)
	$(MKCWD)
	$(STD_AR) rcs $@ $^

$(STD_BIN_DIR)/%.c.o: %.c $(STD_H)
	$(MKCWD)
	$(STD_CC) $(STD_CFLAGS) -c -o $@ $<

