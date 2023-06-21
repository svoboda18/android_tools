SELIB = $(TOPDIR)/external/libselinux
ZLLIB = ../../zlib
SPLIB = ../../core/libsparse
BSLIB = $(TOPDIR)/core/libbase
CULIB = $(TOPDIR)/core/libcutils

SRC = ext4fixup.c ext4_utils.c allocate.c contents.c extent.c indirect.c uuid.c sha1.c wipe.c crc16.c ext4_sb.c make_ext4fs_main.c make_ext4fs.c
OBJS := $(patsubst %.c,$(OBJ)/make_ext4fs/%.o,$(SRC))

MKE4FS_LD = $(LIB)/libsparse.a $(LIB)/libz.a $(LIB)/libcutils.a $(LIB)/libbase.a $(LIB)/libselinux.a

all: $(OUT)/make_ext4fs$(BIN_EXT)

ifeq (0,$(STATIC))
override MKE4FS_LD := $(shell echo $(MKE4FS_LD) | sed "s@\.a@\.dll@g" | sed "s@obj/lib/lib@obj/lib/shared/svb@g")
endif
$(OUT)/make_ext4fs$(BIN_EXT): $(OBJS) $(MKE4FS_LD)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  LD\t    `basename $@`"
	@$(CXX) $(CXXFLAGS) $^ -o $@ -static $(LDFLAGS) $(BIN_RES) $(LIBS)
	@$(STRIP) $(STRIPFLAGS) $@

$(OBJ)/make_ext4fs/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I. -I$(SELIB)/include -I$(CULIB)/include -I$(SPLIB)/include -c $< -o $@
