FOLDERS=source

.PHONY: build clean all

build:
	@( for b in $(FOLDERS); do $(MAKE) -C $$b; done )

clean:
	@( for b in $(FOLDERS); do $(MAKE) -C $$b clean || exit ; done )

all: clean build
