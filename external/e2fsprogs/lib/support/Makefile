E2FLIB = ../ext2fs
BLKLIB = ../blkid
CMELIB = ../et
QUOTA_SRC = dict.c mkquota.c parse_qtype.c plausible.c profile.c profile_helpers.c prof_err.c quotaio.c quotaio_tree.c quotaio_v2.c
QUOTA_OBJS = $(patsubst %.c,$(OBJ)/e2fsprogs/support/%.o,$(QUOTA_SRC)) 
EX2PF_OBJS = $(OBJ)/e2fsprogs/support/prof_err.o $(OBJ)/e2fsprogs/support/profile.o

DLL_DEP := $(SLIB)/svbext2fs.dll $(SLIB)/svbext2_blkid.dll $(SLIB)/svbext2_com_err.dll

all: $(LIB_OUT)/libext2_quota$(LIB_EXT) $(LIB_OUT)/libext2_profile$(LIB_EXT) $(LIB_OUT)/libext2_support$(LIB_EXT)

$(SLIB)/lib%.dll: $(LIB)/lib%.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svb$*.dll \
	    -Wl,--export-all-symbols \
	    -Wl,--enable-auto-import \
	    -Wl,--whole-archive $^ \
	    -Wl,--no-whole-archive $(DLL_DEP) $(DLL_RES)

$(OBJ)/e2fsprogs/support/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I../../include -I../ -c $< -o $@

$(LIB)/libext2_quota.a: $(QUOTA_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

$(LIB)/libext2_profile.a: $(EX2PF_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

$(LIB)/libext2_support.a: $(OBJ)/e2fsprogs/support/cstring.o
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

