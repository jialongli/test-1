//  /usr/include/asm/unistd_32.h:354:#define __NR_memfd_create 356
//  /usr/include/asm/unistd_64.h:322:#define __NR_memfd_create 319
//  /usr/include/asm/unistd_x32.h:280:#define __NR_memfd_create (__X32_SYSCALL_BIT + 319)
//  /usr/include/linux/memfd.h:4:/* flags for memfd_create(2) (unsigned int) */
//  /usr/include/bits/syscall.h:788:#ifdef __NR_memfd_create
//  /usr/include/bits/syscall.h:789:# define SYS_memfd_create __NR_memfd_create



#define MFD_NAME_PREFIX "memfd:"
#define MFD_NAME_PREFIX_LEN (sizeof(MFD_NAME_PREFIX) - 1)
#define MFD_NAME_MAX_LEN (NAME_MAX - MFD_NAME_PREFIX_LEN)

#define MFD_ALL_FLAGS (MFD_CLOEXEC | MFD_ALLOW_SEALING | MFD_HUGETLB)


int memfd_create(const char __user *uname,unsigned int flags);

SYSCALL_DEFINE2(memfd_create,
		const char __user *, uname,
		unsigned int, flags)
{
	unsigned int *file_seals;
	struct file *file;
	int fd, error;
	char *name;
	long len;

	if (!(flags & MFD_HUGETLB)) {
		if (flags & ~(unsigned int)MFD_ALL_FLAGS)
			return -EINVAL;
	} else {
		/* Allow huge page size encoding in flags. */
		if (flags & ~(unsigned int)(MFD_ALL_FLAGS |
				(MFD_HUGE_MASK << MFD_HUGE_SHIFT)))
			return -EINVAL;
	}

	/* length includes terminating zero */
	len = strnlen_user(uname, MFD_NAME_MAX_LEN + 1);
	if (len <= 0)
		return -EFAULT;
	if (len > MFD_NAME_MAX_LEN + 1)
		return -EINVAL;

	name = kmalloc(len + MFD_NAME_PREFIX_LEN, GFP_KERNEL);
	if (!name)
		return -ENOMEM;

	strcpy(name, MFD_NAME_PREFIX);
	if (copy_from_user(&name[MFD_NAME_PREFIX_LEN], uname, len)) {
		error = -EFAULT;
		goto err_name;
	}

	/* terminating-zero may have changed after strnlen_user() returned */
	if (name[len + MFD_NAME_PREFIX_LEN - 1]) {
		error = -EFAULT;
		goto err_name;
	}

	fd = get_unused_fd_flags((flags & MFD_CLOEXEC) ? O_CLOEXEC : 0);
	if (fd < 0) {
		error = fd;
		goto err_name;
	}

	if (flags & MFD_HUGETLB) {
		struct user_struct *user = NULL;

		file = hugetlb_file_setup(name, 0, VM_NORESERVE, &user,
					HUGETLB_ANONHUGE_INODE,
					(flags >> MFD_HUGE_SHIFT) &
					MFD_HUGE_MASK);
	} else
		file = shmem_file_setup(name, 0, VM_NORESERVE);
	if (IS_ERR(file)) {
		error = PTR_ERR(file);
		goto err_fd;
	}
	file->f_mode |= FMODE_LSEEK | FMODE_PREAD | FMODE_PWRITE;
	file->f_flags |= O_LARGEFILE;

	if (flags & MFD_ALLOW_SEALING) {
		file_seals = memfd_file_seals_ptr(file);
		*file_seals &= ~F_SEAL_SEAL;
	}

	fd_install(fd, file);
	kfree(name);
	return fd;

err_fd:
	put_unused_fd(fd);
err_name:
	kfree(name);
	return error;
}

