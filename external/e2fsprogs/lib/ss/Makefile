CMELIB = ../et
SS_SRC = ss_err.c std_rqs.c invocation.c help.c execute_cmd.c listen.c parse.c error.c prompt.c request_tbl.c list_rqs.c pager.c requests.c data.c get_readline.c
SS_OBJS = $(patsubst %.c,$(OBJ)/e2fsprogs/ss/%.o,$(SS_SRC)) 

DLL_DEP := $(SLIB)/svbext2_com_err.dll

all: $(LIB_OUT)/libext2_ss$(LIB_EXT)

$(SLIB)/libext2_ss.dll: $(LIB)/libext2_ss.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbext2_ss.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(DLL_DEP) $(LIBS) $(DLL_RES)

$(OBJ)/e2fsprogs/ss/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I../../include -I.. -c $< -o $@

$(LIB)/libext2_ss.a: $(SS_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^