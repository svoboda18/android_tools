ZLLIB = $(TOPDIR)/external/zlib
BSLIB = ../libbase

LIBSPARSE_SRC = backed_block.cpp output_file.cpp sparse.cpp sparse_err.cpp sparse_read.cpp sparse_crc32.cpp
LIBSPARSE_OBJS := $(patsubst %.cpp,$(OBJ)/sparse/%.o,$(LIBSPARSE_SRC))

SIMG2IMG_SRC = simg2img.cpp sparse_crc32.cpp
SIMG2IMG_OBJS := $(patsubst %.cpp,$(OBJ)/sparse/%.o,$(SIMG2IMG_SRC))

DLL_DEP := $(SLIB)/svbz.dll $(SLIB)/svbbase.dll

all: $(LIB_OUT)/libsparse$(LIB_EXT) $(OUT)/simg2img$(BIN_EXT) $(OUT)/img2simg$(BIN_EXT)

$(SLIB)/libsparse.dll: $(LIB)/libsparse.a
	@echo -e "  LDXX\t    `basename $@`"
	@$(LDXX) -shared -o $(SLIB)/svbsparse.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $(LIB)/libsparse.a \
		-Wl,--no-whole-archive $(DLL_DEP) $(DLL_RES)

$(LIB)/libsparse.a: $(LIBSPARSE_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

$(OBJ)/sparse/%.o: %.cpp
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CXX\t    `basename $<`"
	@$(CXX) -static $(CXXFLAGS) -Iinclude -I$(ZLLIB) -I$(BSLIB)/include -c $< -o $@

SPARSE_LD := $(LIB)/libsparse.a $(LIB)/libz.a $(LIB)/libbase.a
ifeq (0,$(STATIC))
SPARSE_LD := $(shell echo $(SPARSE_LD) | sed "s@\.a@\.dll@g" | sed "s@obj/lib/lib@obj/lib/shared/svb@g")
endif
$(OUT)/simg2img$(BIN_EXT): $(SIMG2IMG_OBJS) $(SPARSE_LD)
	@echo -e "  LD\t    `basename $@`"
	@$(CXX) $(CXXFLAGS) $^ -o $@ -static $(LDFLAGS) $(BIN_RES) $(LIBS)
	@$(STRIP) $(STRIPFLAGS) $@

$(OUT)/img2simg$(BIN_EXT): $(OBJ)/sparse/img2simg.o $(SPARSE_LD)
	@echo -e "  LD\t    `basename $@`"
	@$(CXX) $(CXXFLAGS) $^ -o $@ -static $(LDFLAGS) $(BIN_RES) $(LIBS)
	@$(STRIP) $(STRIPFLAGS) $@

