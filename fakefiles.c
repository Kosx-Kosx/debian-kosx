#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int
my_open(const char *path, int oflag, mode_t mode)
{
	char *root, *newpath;
	int result;
	if (path[0] != '/') {
		goto out;
	}
	if (!(root = getenv("FAKEFILES_ROOT"))) {
		goto out;
	}
	if (asprintf(&newpath, "%s%s", root, path) == -1) {
		goto out;
	}
	if ((result = open(newpath, oflag, mode)) == -1) {
		result = open(path, oflag, mode);
	}
	free(newpath);
	return result;
out:
	return open(path, oflag, mode);
}

struct {void *a; void *b;}
interposers[] __attribute__((section("__DATA, __interpose"))) = {
	{my_open, open},
};
