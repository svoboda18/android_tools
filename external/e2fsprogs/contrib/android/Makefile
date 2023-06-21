ZLLIB = $(TOPDIR)/zlib
SELIB = $(TOPDIR)/external/libselinux
BSLIB = $(TOPDIR)/core/libbase
CULIB = $(TOPDIR)/core/libcutils
SPLIB = $(TOPDIR)/core/libsparse
E2FLIB = ../../lib/ext2fs
CMLIB = ../../lib/et
EPLIB = ../../lib/e2p
MSLIB = ../../misc

E2FSDROID_SRC = e2fsdroid.c block_range.c fsmap.c block_list.c base_fs.c perms.c basefs_allocator.c
E2FSDROID_OBJS := $(patsubst %.c,$(OBJ)/contrib/android/%.o,$(E2FSDROID_SRC))

E2FSDROID_LD = $(LIB)/libext2_com_err.a $(LIB)/libext2_misc.a $(LIB)/libext2fs.a $(LIB)/libsparse.a $(LIB)/libz.a $(LIB)/libcutils.a $(LIB)/libbase.a $(LIB)/libselinux.a
COMMON_LD = $(LIB)/libext2_com_err.a $(LIB)/libext2_e2p.a $(LIB)/libext2fs.a $(LIB)/libsparse.a $(LIB)/libz.a $(LIB)/libbase.a

all: $(OUT)/ext2simg$(BIN_EXT) $(OUT)/e2fsdroid$(BIN_EXT) $(OUT)/e2fstool$(BIN_EXT)

$(OBJ)/contrib/android/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $<`"
	@$(CC) $(CFLAGS) -I$(CULIB)/include -I$(E2FLIB) -I../../include -I../../misc -I$(SELIB)/include -I$(SPLIB)/include -I$(BSLIB)/include -I. -I../../lib -I$(E2FLIB) -c $< -o $@

ifeq (0,$(STATIC))
override E2FSDROID_LD := $(shell echo $(E2FSDROID_LD) | sed "s@\.a@\.dll@g" | sed "s@obj/lib/lib@obj/lib/shared/svb@g")
override COMMON_LD := $(shell echo $(COMMON_LD) | sed "s@\.a@\.dll@g" | sed "s@obj/lib/lib@obj/lib/shared/svb@g")
endif

$(OUT)/e2fsdroid$(BIN_EXT): $(E2FSDROID_OBJS) $(E2FSDROID_LD)
	@echo -e "  LD\t    `basename $@`"
	@$(CXX) $(CXXFLAGS) $^ -o $@ -static $(LDFLAGS) $(BIN_RES) $(LIBS)
	@$(STRIP) $(STRIPFLAGS) $@

$(OUT)/ext2simg$(BIN_EXT): $(OBJ)/contrib/android/ext2simg.o $(COMMON_LD)
	@echo -e "  LD\t    `basename $@`"
	@$(CXX) $(CXXFLAGS) $^ -o $@ -static $(LDFLAGS) $(BIN_RES) $(LIBS)
	@$(STRIP) $(STRIPFLAGS) $@

$(OUT)/e2fstool$(BIN_EXT): $(OBJ)/contrib/android/e2fstool/e2fstool.o $(COMMON_LD)
	@echo -e "  LD\t    `basename $@`"
	@$(CXX) $(CXXFLAGS) $^ -o $@ -static $(LDFLAGS) $(BIN_RES) $(LIBS)
	@$(STRIP) $(STRIPFLAGS) $@
