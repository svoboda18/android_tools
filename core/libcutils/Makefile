BSLIB := ../libbase
LIBCUTILS_SRC := canned_fs_config.cpp fs_config.cpp
LIBCUTILS_OBJS := $(patsubst %.cpp,$(OBJ)/cutils/%.o,$(LIBCUTILS_SRC))

DLL_DEP := $(SLIB)/svbbase.dll

all: $(LIB_OUT)/libcutils$(LIB_EXT)

$(SLIB)/libcutils.dll: $(LIB)/libcutils.a
	@echo -e "  LDXX\t    `basename $@`"
	@$(LDXX) -static-libstdc++ -shared -o $(SLIB)/svbcutils.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $(LIB)/libcutils.a \
		-Wl,--no-whole-archive $(DLL_DEP) $(LIBS) $(DLL_RES)

$(LIB)/libcutils.a: $(LIBCUTILS_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

$(OBJ)/cutils/%.o: %.cpp
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CXX\t    `basename $@`"
	@$(CXX) -static $(CXXFLAGS) -Iinclude -I$(BSLIB)/include -c $< -o $@