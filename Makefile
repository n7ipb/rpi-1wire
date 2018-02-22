CC=gcc 
CFLAGS=-Wall
PROGRAMS=read_probes\
	 read_probe

all: $(PROGRAMS)

clean:
	rm -f read_probes read_probe read_probe.o read_probes.o

PREFIX = /usr
SERVICE = link_probes.service

.PHONY: install
install: $(PROGRAMS)
	for f in $(PROGRAMS); do \
		install -D -t $(PREFIX)/bin $$f; \
	done

.PHONY: link_probes
link_probes:
	install -D -t $(PREFIX)/bin link_probes
	install -D -m 644 -t /lib/systemd/system $(SERVICE)


.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin/read_probes
	rm -f $(PREFIX)/bin/read_probe
	rm -f $(PREFIX)/bin/link_probes
	systemctl stop $(SERVICE)
	systemctl disable $(SERVICE)
	rm /lib/systemd/system/$(SERVICE)
	systemctl daemon-reload
	systemctl reset-failed

