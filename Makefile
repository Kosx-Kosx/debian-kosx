TARGETS = fakefiles.dylib

all: $(TARGETS)

clean:
	-rm $(TARGETS)

%.dylib: %.c
	$(CC) -shared -o $@ $<

.PHONY: all clean
