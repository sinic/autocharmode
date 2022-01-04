CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS += $(shell pkg-config --libs-only-L gtk+-3.0)
LDLIBS += $(shell pkg-config --libs-only-l gtk+-3.0)

dir = immodules

.PHONY: all clean
all: immodules.cache
clean:
	$(RM) -r $(dir) immodules.cache

$(dir):
	mkdir $(dir)
$(dir)/%.so: %.c $(dir)
	$(CC) -fPIC -shared -o $@ $(CFLAGS) $(LDFLAGS) $< $(LDLIBS)
immodules.cache: $(dir)/autocharmode.so
	gtk-query-immodules-3.0 $< > $@
