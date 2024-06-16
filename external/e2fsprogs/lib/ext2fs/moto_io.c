#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#if !defined(ENABLE_LIBSPARSE)
static errcode_t moto_open(const char *name EXT2FS_ATTR((unused)),
			     int flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	return EXT2_ET_UNIMPLEMENTED;
}
static errcode_t moto_close(io_channel channel EXT2FS_ATTR((unused)))
{
	return EXT2_ET_UNIMPLEMENTED;
}
static struct struct_io_manager struct_moto_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "MOTO sparse I/O Manager",
	.open			= moto_open,
	.close			= moto_close,
};
static struct struct_io_manager struct_motofd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "MOTO sparse fd I/O Manager",
	.open			= moto_open,
	.close			= moto_close,
};
#else
#include <sparse/sparse.h>

struct moto_map {
	int			fd;
	char			**blocks;
	int			block_size;
	uint64_t		blocks_count;
	char			*file;
	struct sparse_file	*moto_file;
	io_channel		channel;
};

struct moto_io_params {
	int			fd;
	char			*file;
	uint64_t		blocks_count;
	unsigned int		block_size;
};

static errcode_t moto_write_blk(io_channel channel, unsigned long block,
				  int count, const void *buf);

static void free_moto_blocks(struct moto_map *sm)
{
	uint64_t i;

	for (i = 0; i < sm->blocks_count; ++i)
		free(sm->blocks[i]);
	free(sm->blocks);
	sm->blocks = NULL;
}

static int moto_import_segment(void *priv, const void *data, size_t len,
				 unsigned int block, unsigned int nr_blocks)
{
	struct moto_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	if (!nr_blocks || len % sm->block_size)
		return 0;

	return moto_write_blk(sm->channel, block, nr_blocks, data);
}

static errcode_t io_manager_import_sparse(struct moto_io_params *params,
					  struct moto_map *sm, io_channel io)
{
	int fd;
	errcode_t retval;
	struct sparse_file *moto_file;

	if (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		if (fd < 0) {
			retval = -1;
			goto err_open;
		}
	} else
		fd = params->fd;
	moto_file = sparse_file_import(fd, false, false);
	if (!moto_file) {
		retval = -1;
		goto err_sparse;
	}

	sm->block_size = sparse_file_block_size(moto_file);
	sm->blocks_count = (sparse_file_len(moto_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_count, sizeof(char*));
	if (!sm->blocks) {
		retval = -1;
		goto err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(moto_file, true, false,
					   moto_import_segment, sm);

	if (retval)
		free_moto_blocks(sm);
err_alloc:
	sparse_file_destroy(moto_file);
err_sparse:
	close(fd);
err_open:
	return retval;
}

static errcode_t io_manager_configure(struct moto_io_params *params,
				      int flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	struct moto_map *sm = calloc(1, sizeof(*sm));
	if (!sm)
		return EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	if (retval) {
		if (!params->block_size || !params->blocks_count) {
			retval = EINVAL;
			goto err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_count = params->blocks_count;
		sm->blocks = calloc(params->blocks_count, sizeof(void*));
		if (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			goto err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_count;

	if (flags & IO_FLAG_RW) {
		sm->moto_file = sparse_file_new(sm->block_size, img_size);
		if (!sm->moto_file) {
			retval = EXT2_ET_NO_MEMORY;
			goto err_alloc;
		}
		if (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
				      0644);
			if (sm->fd < 0) {
				retval = errno;
				goto err_open;
			}
		} else
			sm->fd = params->fd;
	} else {
		sm->fd = -1;
		sm->moto_file = NULL;
	}
	return 0;

err_open:
	sparse_file_destroy(sm->moto_file);
err_alloc:
	free_moto_blocks(sm);
err_params:
	free(sm);
	return retval;
}

static errcode_t moto_open_channel(struct moto_io_params *moto_params,
				     int flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, sizeof(struct struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(moto_params, flags, io);
	if (retval) {
		free(io);
		return retval;
	}

	*channel = io;
	return 0;
}

static errcode_t read_moto_argv(const char *name, bool is_fd,
				  struct moto_io_params *moto_params)
{
	int ret;
	moto_params->fd = -1;
	moto_params->block_size = 0;
	moto_params->blocks_count = 0;

	moto_params->file = malloc(strlen(name) + 1);
	if (!moto_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		return EXT2_ET_NO_MEMORY;
	}

	if (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &moto_params->fd,
			     (unsigned long long *)&moto_params->blocks_count,
			     &moto_params->block_size);
	} else {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", moto_params->file,
			     (unsigned long long *)&moto_params->blocks_count,
			     &moto_params->block_size);
	}

	if (ret < 1) {
		free(moto_params->file);
		return EINVAL;
	}
	return 0;
}

static errcode_t moto_open(const char *name, int flags, io_channel *channel)
{
	errcode_t retval;
	struct moto_io_params moto_params;

	retval = read_moto_argv(name, false, &moto_params);
	if (retval)
		return EXT2_ET_BAD_DEVICE_NAME;

	retval = moto_open_channel(&moto_params, flags, channel);
	if (retval)
		return retval;
	(*channel)->manager = moto_io_manager;

	return retval;
}

static errcode_t motofd_open(const char *name, int flags, io_channel *channel)
{
	errcode_t retval;
	struct moto_io_params moto_params;

	retval = read_moto_argv(name, true, &moto_params);
	if (retval)
		return EXT2_ET_BAD_DEVICE_NAME;

	retval = moto_open_channel(&moto_params, flags, channel);
	if (retval)
		return retval;
	(*channel)->manager = motofd_io_manager;

	return retval;
}

static errcode_t moto_merge_blocks(struct moto_map *sm, uint64_t start,
					uint64_t num)
{
	char *buf;
	uint64_t i;
	unsigned int block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(num, block_size);
	if (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)num * block_size);
		return EXT2_ET_NO_MEMORY;
	}

	for (i = 0; i < num; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		free(sm->blocks[start + i]);
		sm->blocks[start + i] = NULL;
	}

	/* free_moto_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->moto_file, sm->blocks[start],
					block_size * num, start);

	return retval;
}

static errcode_t moto_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	struct moto_map *sm = channel->private_data;

	if (sm->moto_file) {
		int64_t chunk_start = (sm->blocks[0] == NULL) ? -1 : 0;
		for (i = 0; i < sm->blocks_count; ++i) {
			if (!sm->blocks[i] && chunk_start != -1) {
				retval = moto_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} else if (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			if (retval)
				goto ret;
		}
		if (chunk_start != -1) {
			retval = moto_merge_blocks(sm, chunk_start,
							sm->blocks_count - chunk_start);
			if (retval)
				goto ret;
		}
		retval = sparse_file_write(sm->moto_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	if (sm->moto_file)
		sparse_file_destroy(sm->moto_file);
	free_moto_blocks(sm);
	free(sm->file);
	free(sm);
	free(channel);
	return retval;
}

static errcode_t moto_close(io_channel channel)
{
	errcode_t retval;
	struct moto_map *sm = channel->private_data;
	int fd = sm->fd;

	retval = moto_close_channel(channel);
	if (fd >= 0)
		close(fd);

	return retval;
}

static errcode_t moto_set_blksize(io_channel channel, int blksize)
{
	channel->block_size = blksize;
	return 0;
}

static blk64_t block_to_moto_block(blk64_t block, blk64_t *offset,
			       io_channel channel, struct moto_map *sm)
{
	int ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	return ret;
}

static errcode_t check_block_size(io_channel channel, struct moto_map *sm)
{
	if (sm->block_size >= channel->block_size)
		return 0;
	return EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

static errcode_t moto_read_blk64(io_channel channel, blk64_t block,
				   int count, void *buf)
{
	int i;
	char *out = buf;
	blk64_t offset = 0, cur_block;
	struct moto_map *sm = channel->private_data;

	if (check_block_size(channel, sm))
		return EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	block += (4096 * 32) / channel->block_size;

	if (count < 0) { //partial read
		count = -count;
		cur_block = block_to_moto_block(block, &offset, channel, sm);
		if (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, count);
		else
			memset(out, 0, count);
	} else {
		for (i = 0; i < count; ++i) {
			cur_block = block_to_moto_block(block + i, &offset,
						    channel, sm);
			if (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			else if (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	return 0;
}

static errcode_t moto_read_blk(io_channel channel, unsigned long block,
				 int count, void *buf)
{
	return moto_read_blk64(channel, block, count, buf);
}

static errcode_t moto_write_blk64(io_channel channel, blk64_t block,
				    int count, const void *buf)
{
	int i;
	blk64_t offset = 0, cur_block;
	const char *in = buf;
	struct moto_map *sm = channel->private_data;

	if (check_block_size(channel, sm))
		return EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	if (count < 0) { //partial write
		count = -count;
		cur_block = block_to_moto_block(block, &offset, channel,
						  sm);
		if (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			if (!sm->blocks[cur_block])
				return EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, count);
	} else {
		for (i = 0; i < count; ++i) {
			if (block + i >= sm->blocks_count)
				return 0;
			cur_block = block_to_moto_block(block + i, &offset,
						    channel, sm);
			if (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				if (!sm->blocks[cur_block])
					return EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	return 0;
}

static errcode_t moto_write_blk(io_channel channel, unsigned long block,
				  int count, const void *buf)
{
	return moto_write_blk64(channel, block, count, buf);
}

static errcode_t moto_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long count)
{
	blk64_t cur_block, offset;
	struct moto_map *sm = channel->private_data;

	if (check_block_size(channel, sm))
		return EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	for (unsigned long long i = 0; i < count; ++i) {
		if (blk + i >= sm->blocks_count)
			return 0;
		cur_block = block_to_moto_block(blk + i, &offset, channel,
						  sm);
		if (!sm->blocks[cur_block])
			continue;
		free(sm->blocks[cur_block]);
		sm->blocks[cur_block] = NULL;
	}
	return 0;
}

static errcode_t moto_zeroout(io_channel channel, blk64_t blk,
				unsigned long long count)
{
	return moto_discard(channel, blk, count);
}

static errcode_t moto_flush(io_channel channel __attribute__((unused)))
{
	return 0;
}

static errcode_t moto_set_option(io_channel channel __attribute__((unused)),
                                   const char *option __attribute__((unused)),
                                   const char *arg __attribute__((unused)))
{
	return 0;
}

static errcode_t moto_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long count __attribute__((unused)))
{
	return 0;
}

static struct struct_io_manager struct_moto_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "MOTO sparse I/O Manager",
	.open			= moto_open,
	.close			= moto_close,
	.set_blksize	= moto_set_blksize,
	.read_blk		= moto_read_blk,
	.write_blk		= moto_write_blk,
	.flush			= moto_flush,
	.write_byte		= NULL,
	.set_option		= moto_set_option,
	.get_stats		= NULL,
	.read_blk64		= moto_read_blk64,
	.write_blk64	= moto_write_blk64,
	.discard		= moto_discard,
	.cache_readahead= moto_cache_readahead,
	.zeroout		= moto_zeroout,
};

static struct struct_io_manager struct_motofd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "MOTO sparse fd I/O Manager",
	.open			= motofd_open,
	.close			= moto_close,
	.set_blksize	= moto_set_blksize,
	.read_blk		= moto_read_blk,
	.write_blk		= moto_write_blk,
	.flush			= moto_flush,
	.write_byte		= NULL,
	.set_option		= moto_set_option,
	.get_stats		= NULL,
	.read_blk64		= moto_read_blk64,
	.write_blk64	= moto_write_blk64,
	.discard		= moto_discard,
	.cache_readahead= moto_cache_readahead,
	.zeroout		= moto_zeroout,
};

#endif

io_manager moto_io_manager = &struct_moto_manager;
io_manager motofd_io_manager = &struct_motofd_manager;