PLUG=preamp
DEST=/usr/local/lib/ladspa
ARCH=`uname -m`

#OPTS=-O6 -ffast-math -march=$(ARCH) -mcpu=$(ARCH)

all: $(PLUG).so

run: all
	sudo python -i preamp.py

$(PLUG).so: $(PLUG).o
	g++   -fPIC -Wall $(OPTS) -shared -o $@ $<

.c.s:
	gcc  -fPIC -O6 -I/usr/local/include -S $<

.cc.o:
	g++   -fPIC -Wall $(OPTS) -I/usr/local/include -c $<

tags:
	ctags *.cc *.h

install: all
	install -m 644 $(PLUG).so $(DEST)

uninstall:
	rm $(DEST)/$(PLUG).so

tarball:
	@ln -s . $(PLUG)
	@tar chvfz $(PLUG).tar.gz $(PLUG)/Makefile $(PLUG)/$(PLUG).cc $(PLUG)/*.h $(PLUG)/README $(PLUG)/COPYING
	@rm $(PLUG)
	@cp $(PLUG).tar.gz /www/quitte/dsp
	@cp neck.ogg /www/quitte/dsp
