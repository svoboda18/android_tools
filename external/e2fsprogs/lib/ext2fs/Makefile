CMELIB = ../et
ZLLIB = ../../../../zlib
SPLIB = ../../../../core/libsparse
BSLIB = ../../../../core/libbase
E2F_SRC = \
	ext2_err.c \
	alloc.c \
	alloc_sb.c \
	alloc_stats.c \
	alloc_tables.c \
	atexit.c \
	badblocks.c \
	bb_inode.c \
	bitmaps.c \
	bitops.c \
	blkmap64_ba.c \
	blkmap64_rb.c \
	blknum.c \
	block.c \
	bmap.c \
	check_desc.c \
	crc16.c \
	crc32c.c \
	csum.c \
	closefs.c \
	dblist.c \
	dblist_dir.c \
	digest_encode.c \
	dirblock.c \
	dirhash.c \
	dir_iterate.c \
	dupfs.c \
	expanddir.c \
	ext_attr.c \
	extent.c \
	fallocate.c \
	fileio.c \
	finddev.c \
	flushb.c \
	freefs.c \
	gen_bitmap.c \
	gen_bitmap64.c \
	get_num_dirs.c \
	get_pathname.c \
	getsize.c \
	getsectsize.c \
	hashmap.c \
	i_block.c \
	icount.c \
	imager.c \
	ind_block.c \
	initialize.c \
	inline.c \
	inline_data.c \
	inode.c \
	io_manager.c \
	ismounted.c \
	link.c \
	llseek.c \
	lookup.c \
	mmp.c \
	mkdir.c \
	mkjournal.c \
	namei.c \
	native.c \
	newdir.c \
	nls_utf8.c \
	openfs.c \
	progress.c \
	punch.c \
	qcow2.c \
	rbtree.c \
	read_bb.c \
	read_bb_file.c \
	res_gdt.c \
	rw_bitmaps.c \
	sha256.c \
	sha512.c \
	swapfs.c \
	symlink.c \
	undo_io.c \
	unix_io.c \
	sparse_io.c \
	unlink.c \
	valid_blk.c \
	version.c
E2F_OBJS = $(patsubst %.c,$(OBJ)/e2fsprogs/ext2fs/%.o,$(E2F_SRC))

DLL_DEP := $(SLIB)/svbext2_com_err.dll $(SLIB)/svbsparse.dll $(SLIB)/svbz.dll $(SLIB)/svbbase.dll

all: $(LIB_OUT)/libext2fs$(LIB_EXT)

$(SLIB)/libext2fs.dll: $(LIB)/libext2fs.a
	@$(MKDIR) -p `dirname $@`
	@echo -e "  LD\t    `basename $@`"
	@$(LD) -shared -o $(SLIB)/svbext2fs.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $^ \
		-Wl,--no-whole-archive $(DLL_DEP) $(LIBS) $(DLL_RES)

$(OBJ)/e2fsprogs/ext2fs/%.o: %.c
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CC\t    `basename $@`"
	@$(CC) $(CFLAGS) -I$(SPLIB)/include -I../../include -I.. -c $< -o $@

$(LIB)/libext2fs.a: $(E2F_OBJS)
	@$(MKDIR) -p `dirname $@`
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^
