SRC = callbacks.c regex.c freecon.c label.c label_file.c
OBJS := $(patsubst %.c,$(OBJ)/external/selinux/%.o,$(SRC))

all: $(LIB_OUT)/libselinux$(LIB_EXT)

$(SLIB)/libselinux.dll: $(LIB)/libselinux.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbselinux.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(LIBS) $(DLL_RES)

$(LIB)/libselinux.a: $(OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

$(OBJ)/external/selinux/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -Iinclude -c $< -o $@
