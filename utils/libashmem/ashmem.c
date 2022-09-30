#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <utils/Compat.h>

#define PAGE_SIZE 4096

static bool ashmem_validate_stat(int fd, struct stat *buf)
{
    int result = fstat(fd, buf);
    if (result == -1) {
        return false;
    }

    if (!(buf->st_nlink == 0 && S_ISREG(buf->st_mode))) {
        errno = ENOTTY;
        return false;
    }
    return true;
}

int ashmem_valid(int fd)
{
    struct stat buf;
    return ashmem_validate_stat(fd, &buf);
}

int ashmem_create_region(const char* name, size_t size)
{
    char pattern[PATH_MAX];
    snprintf(pattern, sizeof(pattern), "/tmp/android-ashmem-%d-XXXXXXXXX", getpid());
    int fd = mkstemp(pattern);
    if (fd == -1) return -1;

    unlink(pattern);

    if (TEMP_FAILURE_RETRY(ftruncate(fd, size)) == -1) {
      close(fd);
      return -1;
    }

    return fd;
}

int ashmem_set_prot_region(int fd __unused, int prot __unused)
{
    return 0;
}

int ashmem_unpin_region(int fd __unused, size_t offset __unused, size_t len __unused)
{
    return 0;
}

int ashmem_get_size_region(int fd)
{
    struct stat buf;
    if (!ashmem_validate_stat(fd, &buf)) {
        return -1;
    }

    return buf.st_size;
}
