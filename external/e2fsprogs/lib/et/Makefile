CME_SRC = error_message.c et_name.c init_et.c com_err.c com_right.c
CME_OBJS = $(patsubst %.c,$(OBJ)/e2fsprogs/et/%.o,$(CME_SRC)) 

all: $(LIB_OUT)/libext2_com_err$(LIB_EXT)

$(SLIB)/libext2_com_err.dll: $(LIB)/libext2_com_err.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbext2_com_err.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(DLL_RES)

$(OBJ)/e2fsprogs/et/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I../../include -I../ -c $< -o $@

$(LIB)/libext2_com_err.a: $(CME_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

