BLKLIB = blkid
E2PLIB = e2p
CMELIB = et
E2FLIB = ext2fs
SSLIB = ss
SUPLIB = support
UIDLIB = uuid

all: 
	@$(MAKE) -$(MAKEFLAGS) -C $(UIDLIB) all
	@$(MAKE) -$(MAKEFLAGS) -C $(BLKLIB) all
	@$(MAKE) -$(MAKEFLAGS) -C $(E2PLIB) all
	@$(MAKE) -$(MAKEFLAGS) -C $(CMELIB) all
	@$(MAKE) -$(MAKEFLAGS) -C $(E2FLIB) all
	@$(MAKE) -$(MAKEFLAGS) -C $(SSLIB) all
	@$(MAKE) -$(MAKEFLAGS) -C $(SUPLIB) all