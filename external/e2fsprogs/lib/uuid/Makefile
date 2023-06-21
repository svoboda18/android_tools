UUID_SRC = clear.c compare.c copy.c gen_uuid.c isnull.c pack.c parse.c unpack.c unparse.c uuid_time.c
UUID_OBJS = $(patsubst %.c,$(OBJ)/e2fsprogs/uuid/%.o,$(UUID_SRC))

all: $(LIB_OUT)/libext2_uuid$(LIB_EXT)

$(SLIB)/libext2_uuid.dll: $(LIB)/libext2_uuid.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbext2_uuid.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(LIBS) $(DLL_RES)

$(OBJ)/e2fsprogs/uuid/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I.. -I../../include -c $< -o $@

$(LIB)/libext2_uuid.a: $(UUID_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^
