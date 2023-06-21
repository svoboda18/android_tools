E2P_SRC = encoding.c errcode.c feature.c fgetflags.c fsetflags.c fgetproject.c fsetproject.c fgetversion.c fsetversion.c \
           getflags.c getversion.c hashstr.c iod.c ljs.c ls.c mntopts.c parse_num.c pe.c pf.c ps.c \
	   setflags.c setversion.c uuid.c ostype.c percent.c
E2P_OBJS = $(patsubst %.c,$(OBJ)/e2fsprogs/e2p/%.o,$(E2P_SRC)) 

all: $(LIB_OUT)/libext2_e2p$(LIB_EXT)

$(SLIB)/libext2_e2p.dll: $(LIB)/libext2_e2p.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbext2_e2p.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(LIBS) $(DLL_RES)

$(OBJ)/e2fsprogs/e2p/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I../../include -I.. -c $< -o $@

$(LIB)/libext2_e2p.a: $(E2P_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^
