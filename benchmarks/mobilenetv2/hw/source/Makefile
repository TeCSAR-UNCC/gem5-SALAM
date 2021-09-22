CLUSTERS=\
	head \
	body \
	tail \
	classifier

.PHONY: build clean all

build:
	@( for b in $(CLUSTERS); do $(MAKE) -C $$b; done )

clean:
	@( for b in $(CLUSTERS); do $(MAKE) -C $$b clean || exit ; done )

all: clean build
