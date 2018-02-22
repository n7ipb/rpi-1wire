CC=gcc 
CFLAGS=-Wall
PROGRAMS=read_probes\
	 read_probe

all: $(PROGRAMS)

clean:
	rm -f read_probes read_probe read_probe.o read_probes.o

PREFIX = /usr

.PHONY: install
install: $(PROGRAMS)
	for f in $(PROGRAMS); do \
		install -D -t $(PREFIX)/bin $$f; \
	done

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin/read_probes
	rm -f $(PREFIX)/bin/read_probe
