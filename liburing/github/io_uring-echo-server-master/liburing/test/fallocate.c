/* SPDX-License-Identifier: MIT */
/*
 * Description: test io_uring fallocate
 *
 */
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "liburing.h"

static int no_fallocate;

static int test_fallocate(struct io_uring *ring)
{
	struct io_uring_cqe *cqe;
	struct io_uring_sqe *sqe;
	struct stat st;
	char buf[32];
	int fd, ret;

	sprintf(buf, "./XXXXXX");
	fd = mkstemp(buf);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	sqe = io_uring_get_sqe(ring);
	if (!sqe) {
		fprintf(stderr, "get sqe failed\n");
		goto err;
	}
	io_uring_prep_fallocate(sqe, fd, 0, 0, 128*1024);

	ret = io_uring_submit(ring);
	if (ret <= 0) {
		fprintf(stderr, "sqe submit failed: %d\n", ret);
		goto err;
	}

	ret = io_uring_wait_cqe(ring, &cqe);
	if (ret < 0) {
		fprintf(stderr, "wait completion %d\n", ret);
		goto err;
	}

	if (cqe->res == -EINVAL) {
		fprintf(stdout, "Fallocate not supported, skipping\n");
		no_fallocate = 1;
		goto out;
	}
	if (cqe->res) {
		fprintf(stderr, "cqe->res=%d\n", cqe->res);
		goto err;
	}
	io_uring_cqe_seen(ring, cqe);

	if (fstat(fd, &st) < 0) {
		perror("stat");
		goto err;
	}

	if (st.st_size != 128*1024) {
		fprintf(stderr, "Size mismatch: %llu\n",
					(unsigned long long) st.st_size);
		goto err;
	}

out:
	unlink(buf);
	return 0;
err:
	unlink(buf);
	return 1;
}

static int test_fallocate_fsync(struct io_uring *ring)
{
	struct io_uring_cqe *cqe;
	struct io_uring_sqe *sqe;
	struct stat st;
	char buf[32];
	int fd, ret, i;

	if (no_fallocate)
		return 0;

	sprintf(buf, "./XXXXXX");
	fd = mkstemp(buf);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	sqe = io_uring_get_sqe(ring);
	if (!sqe) {
		fprintf(stderr, "get sqe failed\n");
		goto err;
	}
	io_uring_prep_fallocate(sqe, fd, 0, 0, 128*1024);
	sqe->flags |= IOSQE_IO_LINK;
	sqe->user_data = 1;

	sqe = io_uring_get_sqe(ring);
	if (!sqe) {
		fprintf(stderr, "get sqe failed\n");
		goto err;
	}
	io_uring_prep_fsync(sqe, fd, 0);
	sqe->user_data = 2;

	ret = io_uring_submit(ring);
	if (ret <= 0) {
		fprintf(stderr, "sqe submit failed: %d\n", ret);
		goto err;
	}

	for (i = 0; i < 2; i++) {
		ret = io_uring_wait_cqe(ring, &cqe);
		if (ret < 0) {
			fprintf(stderr, "wait completion %d\n", ret);
			goto err;
		}
		if (cqe->res) {
			fprintf(stderr, "cqe->res=%d,data=%llu\n", cqe->res,
							cqe->user_data);
			goto err;
		}
		io_uring_cqe_seen(ring, cqe);
	}

	if (fstat(fd, &st) < 0) {
		perror("stat");
		goto err;
	}

	if (st.st_size != 128*1024) {
		fprintf(stderr, "Size mismatch: %llu\n",
					(unsigned long long) st.st_size);
		goto err;
	}

	unlink(buf);
	return 0;
err:
	unlink(buf);
	return 1;
}

int main(int argc, char *argv[])
{
	struct io_uring ring;
	int ret;

	ret = io_uring_queue_init(8, &ring, 0);
	if (ret) {
		fprintf(stderr, "ring setup failed\n");
		return 1;
	}

	ret = test_fallocate(&ring);
	if (ret) {
		fprintf(stderr, "test_fallocate failed\n");
		return ret;
	}

	ret = test_fallocate_fsync(&ring);
	if (ret) {
		fprintf(stderr, "test_fallocate_fsync failed\n");
		return ret;
	}

	return 0;
}