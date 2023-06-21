# env controlled
DEBUG ?= 0
CROSS_COMPILE ?=
SH ?= sh

# Build configuration (static only, shared are broken)
override TOPDIR := $(shell cygpath -m $(shell pwd))
override STATIC := 1
override SVB_MINGW := 1
override SVB_FLAGS := -DSVB_WIN32 -DANDROID
override BUILD_FLAGS := -fno-exceptions -fdiagnostics-absolute-paths -Wno-deprecated-non-prototype \
						-DHOST -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
override BUILD_EXTRAS := 1
override BIN_EXT := .exe
override LIB_EXT := .a

ifeq ($(STATIC),0)
$(warning WARNING: Host libraries are statically linked)
override LIB_EXT := .dll
endif

ifeq ($(DEBUG),1)
override BUILD_FLAGS += -ggdb -ffunction-sections -O0 -Wall -Wextra -Wpedantic -Wconversion-null -Wno-gnu-include-next
override SVB_FLAGS += -DSVB_DEBUG
else
override BUILD_FLAGS += -Oz
endif
override LDFLAGS := -Wl,-gc-sections

ifeq ($(SVB_MINGW),1)
override SVB_FLAGS += -DSVB_MINGW -DHAVE_LIB_NT_H -I$(TOPDIR)/libnt/include
all:: print_info init_out res svbnt cyg
else
all:: print_info init_out res cyg
endif

override CC := $(CROSS_COMPILE)clang
override CFLAGS := $(CFLAGS) $(BUILD_FLAGS) $(SVB_FLAGS)
override CXX := $(CROSS_COMPILE)clang++
override CXXSTD := c++17
override CXXLIB := libc++
override CXXFLAGS := $(CXXFLAGS) -std=$(CXXSTD) -stdlib=$(CXXLIB) $(BUILD_FLAGS) $(SVB_FLAGS)
# LD is set for shared libs
ifeq ($(STATIC),0)
override LD := $(CROSS_COMPILE)clang $(BUILD_FLAGS)
override LDXX := $(CROSS_COMPILE)clang++ -std=$(CXXSTD) -stdlib=$(CXXLIB) $(BUILD_FLAGS) -static-libstdc++
#override LDFLAGS += -Wl,--large-address-aware
endif
override STRIP_CMD := $(CROSS_COMPILE)strip
override STRIPFLAGS := $(STRIPFLAGS) --strip-all -R .comment -R .gnu.version --strip-unneeded
override AR := $(CROSS_COMPILE)ar
override ARFLAGS := rcsD

override DEPLOY := $(TOPDIR)/build
override OUT := $(TOPDIR)/out
override SRP := $(OUT)
override OBJ := $(OUT)/obj
override LIB := $(OBJ)/lib
override SLIB := $(LIB)/shared
override LIB_OUT := $(LIB)
ifeq ($(STATIC),0)
override LIB_OUT := $(SLIB)
endif

override STRIP := $(SH) $(TOPDIR)/scripts/strip.sh
override MKDIR := $(SH) $(TOPDIR)/scripts/mkdir.sh

override BIN_RES := $(OBJ)/bin.res
override DLL_RES := $(OBJ)/dll.res

# run make TARGET=windows for a mingw32 build
ifeq ($(SVB_MINGW),1)
override LIBS := -lWs2_32 $(LIB)/libnt.a -limagehlp -lpthread
endif

override MAGISKBOOT := boot

override NTLIB := libnt
override SELINUXLIB := external/libselinux
override SPARSELIB := core/libsparse
override BASESLIB := core/libbase
override CUTILSLIB := core/libcutils
override E2FSPROGS := external/e2fsprogs
override EXT4UTILS := extras/ext4_utils

override E2FSPROGS_BINS = external/e2fsprogs/contrib/android

override GNUMAKEFLAGS += --output-sync=line --no-print-directory
override MAKEFLAGS := -$(MAKEFLAGS) $(GNUMAKEFLAGS) --warn-undefined-variables

BUILD_SHARED := \
	$(SLIB)/svbext2_com_err.dll \
	$(SLIB)/svbext2_blkid.dll \
	$(SLIB)/svbext2_quota.dll \
	$(SLIB)/svbext2_uuid.dll \
	$(SLIB)/svbext2_e2p.dll \
	$(SLIB)/svbext2_misc.dll \
	$(SLIB)/svbselinux.dll \
	$(SLIB)/svbext2fs.dll \
	$(SLIB)/svbcutils.dll \
	$(SLIB)/svbsparse.dll \
	$(SLIB)/svbmincrypt.dll \
	$(SLIB)/svbzopfli.dll \
	$(SLIB)/svbbase.dll \
	$(SLIB)/svblzma.dll \
	$(SLIB)/svblz4.dll \
	$(SLIB)/svbfdt.dll \
	$(SLIB)/svbbz2.dll \
	$(SLIB)/svbz.dll
BUILD_FILES := \
	$(SRP)/make_ext4fs$(BIN_EXT) \
	$(SRP)/magiskboot$(BIN_EXT) \
	$(SRP)/e2fsdroid$(BIN_EXT) \
	$(SRP)/e2fstool$(BIN_EXT) \
	$(SRP)/mke2fs$(BIN_EXT) \
	$(OUT)/mke2fs.conf
BUILD_EXTRA := \
	$(SRP)/simg2img$(BIN_EXT) \
	$(SRP)/img2simg$(BIN_EXT) \
	$(SRP)/ext2simg$(BIN_EXT)

ifeq (1,$(STATIC))
override BUILD_SHARED :=
endif

ifeq (1,$(BUILD_EXTRAS))
override BUILD_FILES := $(BUILD_FILES) $(BUILD_EXTRA)
endif

override MAKEFLAGS += -rsR

export TOPDIR DEBUG STATIC SVB_MINGW SELINUXLIB BUILD_FILES CROSS_COMPILE AR LIBS SVB_FLAGS CC CFLAGS CXX CXXSTD CXXLIB CXXFLAGS \
	LD LDXX LDFLAGS STRIP STRIP_CMD STRIPFLAGS AR ARFLAGS LIBS DEPLOY OUT OBJ LIB SRP SLIB BIN_RES DLL_RES BIN_EXT LIB_EXT LIB_OUT MKDIR \
	MAGISKBOOT ZLIB NTLIB SPARSELIB BASESLIB CUTILSLIB E2FSPROGS EXT4UTILS GNUMAKEFLAGS

.PHONY: all

print_info:
	$(info INFO: CXX STD VERSION '$(CXXSTD)')
	$(info INFO: CXX STD LIB '$(CXXLIB)')
	$(info INFO: CC '$(CC) $(CFLAGS)')
	$(info INFO: CXX '$(CXX) $(CXXFLAGS)')
	$(info INFO: LD '$(CXX) $(CXXFLAGS) $(LDFLAGS) $(BIN_RES) $(LIBS)')
	$(info INFO: AR '$(AR) $(ARFLAGS)')
	$(info INFO: STRIP '$(STRIP) $(STRIPFLAGS)')

init_out:
	@$(MKDIR) -p $(OUT)
	@$(MKDIR) -p $(OBJ)
	@$(MKDIR) -p $(LIB)
	@if [[ $(STATIC) -eq 0 ]]; then \
		$(MKDIR) -p $(SLIB); \
	fi

res: $(BIN_RES) $(DLL_RES)

svbnt: init_out print_info res
	@$(MAKE) $(MAKEFLAGS) -C $(NTLIB)

cyg: svbselinux svbboot
	@$(MAKE) $(MAKEFLAGS) -C $(BASESLIB) all
	@$(MAKE) $(MAKEFLAGS) -C $(SPARSELIB) all
	@$(MAKE) $(MAKEFLAGS) -C $(CUTILSLIB) all
	@$(MAKE) $(MAKEFLAGS) -C $(EXT4UTILS) all
	@$(MAKE) $(MAKEFLAGS) -C $(E2FSPROGS) all

$(OBJ)/%.res: %.rc
	@echo -e "  WINDRES   `basename $@`"
	@windres --input=$< --output-format=coff --output=$@

svbsparse:
	@$(MAKE) $(MAKEFLAGS) -C $(SPARSELIB) $(LIB_OUT)/libsparse$(LIB_EXT)

svbcutils:
	@$(MAKE) $(MAKEFLAGS) -C $(CUTILSLIB)
	
svbbase:
	@$(MAKE) $(MAKEFLAGS) -C $(BASESLIB)
	
svbselinux:
	@$(MAKE) $(MAKEFLAGS) -C $(SELINUXLIB)

e2fsdroid:
	@$(MAKE) $(MAKEFLAGS) -C $(E2FSPROGS_BINS) $(OUT)/e2fsdroid$(BIN_EXT)

e2fstool:
	@$(MAKE) $(MAKEFLAGS) -C $(E2FSPROGS_BINS) $(OUT)/e2fstool$(BIN_EXT)

svbboot:
	@$(MAKE) $(MAKEFLAGS) -C $(MAGISKBOOT) magiskboot

clean:
	@echo -e "  RM\t    obj"
	@rm -rf $(OBJ)
	@echo -e "  RM\t    bin"
	@rm -rf $(OUT)

stamp:
	@cd "$(DEPLOY)";
	@powershell -c '$(shell cat scripts/stamp-ps)'

deploy: all
	@$(MKDIR) -p $(DEPLOY)
	@echo -e "  DEPLOY    `basename $(DEPLOY)`"
	@cp -rpf $(BUILD_FILES) \
	    $(BUILD_SHARED) \
	    $(DEPLOY)
	@$(MAKE) $(MAKEFLAGS) stamp