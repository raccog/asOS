STD_DIR = libs/std
STD_BUILD_DIR = $(BUILD_DIR)/$(STD_DIR)
STD_SRC = 	$(STD_DIR)/log/scanner.c \
			$(STD_DIR)/log/format.c \
			$(STD_DIR)/alloc/alloc.c \
			$(STD_DIR)/io/io.c
STD_OBJ = $(patsubst $(STD_DIR)/%, $(STD_BUILD_DIR)/%.o, $(STD_SRC))

STD_CC = clang
STD_AR = llvm-ar
STD_CFLAGS = -target x86_64-pc-unknown-windows -ffreestanding -mno-red-zone -std=c11 -Ilibs

STD_LIB = $(STD_BUILD_DIR)/libstd.a

ALL += $(STD_LIB)

$(STD_LIB): $(STD_OBJ)
	@$(MKCWD)
	$(STD_AR) rcs $@ $^

$(STD_BUILD_DIR)/log/%.c.o: $(STD_DIR)/log/%.c
	@$(MKCWD)
	$(STD_CC) $(STD_CFLAGS) -c -o $@ $<

$(STD_BUILD_DIR)/alloc/%.c.o: $(STD_DIR)/alloc/%.c
	@$(MKCWD)
	$(STD_CC) $(STD_CFLAGS) -c -o $@ $<

$(STD_BUILD_DIR)/%.c.o: $(STD_DIR)/%.c
	@$(MKCWD)
	$(STD_CC) $(STD_CFLAGS) -c -o $@ $<

