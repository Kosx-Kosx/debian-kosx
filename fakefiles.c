#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
my_open(const char *path, int oflag, mode_t mode)
{
	char *root, *newpath;
	int result;

	if (!(root = getenv("FAKEFILES_ROOT"))) {
		goto out;
	}

	if (path[0] != '/') {
		goto out;
	}
	if (asprintf(&newpath, "%s%s", root, path) == -1) {
		goto out;
	}

	result = open(newpath, oflag, mode);
	free(newpath);
	if (result != -1) {
		return result;
	}

out:
	result = open(path, oflag, mode);

	if (result == -1 && root && errno == ENOENT) {
		char target[PATH_MAX];
		int n;
		if ((n = readlink(path, target, PATH_MAX - 1)) == -1) {
			goto out2;
		}
		target[n] = '\0';
		if (target[0] != '/') {
			goto out2;
		}
		if (asprintf(&newpath, "%s%s", root, target) == -1) {
			goto out2;
		}
		result = open(newpath, oflag, mode);
		free(newpath);
	}

out2:
	return result;
}

struct {void *a; void *b;}
interposers[] __attribute__((section("__DATA, __interpose"))) = {
	{my_open, open},
};
