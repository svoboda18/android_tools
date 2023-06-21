UIDLIB = ../uuid
BLKID_SRC = cache.c dev.c devname.c devno.c getsize.c llseek.c probe.c read.c resolve.c save.c tag.c version.c
BLKID_OBJS = $(patsubst %.c,$(OBJ)/e2fsprogs/blkid/%.o,$(BLKID_SRC))

DLL_DEP := $(SLIB)/svbext2_uuid.dll

all: $(LIB_OUT)/libext2_blkid$(LIB_EXT)

$(SLIB)/libext2_blkid.dll: $(LIB)/libext2_blkid.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbext2_blkid.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(DLL_DEP) $(LIBS) $(DLL_RES)

$(OBJ)/e2fsprogs/blkid/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I../../include -I.. -c $< -o $@

$(LIB)/libext2_blkid.a: $(BLKID_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^
