ZLLIB = ../../../zlib
SELIB = $(TOPDIR)/external/libselinux
GULIB = ../../../libgnu
BSLIB = ../../../core/libbase
CULIB = ../../../core/libcutils
SPLIB = ../../../core/libsparse
BLKLIB = ../lib/blkid
UIDLIB = ../lib/uuid
E2FLIB = ../lib/ext2fs
E2PLIB = ../lib/e2p
CMLIB = ../lib/et
QTALIB = ../lib/support
MKE2FS_SRC = mke2fs.c util.c mk_hugefiles.c default_profile.c
MKE2FS_OBJS := $(patsubst %.c,$(OBJ)/e2fsprogs/misc/%.o,$(MKE2FS_SRC))
MKE2FS_LD = $(LIB)/libext2_com_err.a $(LIB)/libext2_misc.a $(LIB)/libext2_e2p.a $(LIB)/libext2_blkid.a \
	    $(LIB)/libext2_uuid.a $(LIB)/libext2_quota.a $(LIB)/libext2fs.a $(LIB)/libsparse.a $(LIB)/libz.a $(LIB)/libbase.a

DLL_DEP := $(SLIB)/svbext2_com_err.dll $(SLIB)/svbext2fs.dll $(SLIB)/svbext2_quota.dll

all: $(LIB_OUT)/libext2_misc$(LIB_EXT) $(OUT)/mke2fs$(BIN_EXT) $(OUT)/mke2fs.conf

$(SLIB)/libext2_misc.dll: $(LIB)/libext2_misc.a
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbext2_misc.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(DLL_DEP) $(LIBS) $(DLL_RES)

$(OBJ)/e2fsprogs/misc/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I../lib -I../include -c $< -o $@

$(LIB)/libext2_misc.a: $(OBJ)/e2fsprogs/misc/create_inode.o
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

ifeq (0,$(STATIC))
override MKE2FS_LD := $(shell echo $(MKE2FS_LD) | sed "s@\.a@\.dll@g" | sed "s@obj/lib/lib@obj/lib/shared/svb@g")
endif

$(OUT)/mke2fs$(BIN_EXT): $(MKE2FS_OBJS) $(MKE2FS_LD)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  LD\t    `basename $@`"
	@$(CXX) $(CXXFLAGS) $^ -o $@ -static $(LDFLAGS) $(BIN_RES) $(LIBS)
	@$(STRIP) $(STRIPFLAGS) $@

$(OUT)/mke2fs.conf: mke2fs.conf.in
	@echo -e "  CONF\t    `basename $@`"
	@cp -f $^ $@