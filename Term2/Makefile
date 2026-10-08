PROJECTS = Assignment1 Assignment2Calc Assignment3AVL Assignment4Int128 Assignment5Percolation Practice_13_05

.PHONY: all test sanitize clean cleanall $(PROJECTS)

all: $(PROJECTS)

$(PROJECTS):
	$(MAKE) -C $@ all

test:
	@set -e; for project in $(PROJECTS); do $(MAKE) -C $$project test; done

sanitize:
	@set -e; for project in $(PROJECTS); do $(MAKE) -C $$project sanitize; done

clean:
	@set -e; for project in $(PROJECTS); do $(MAKE) -C $$project clean; done

cleanall:
	@set -e; for project in $(PROJECTS); do $(MAKE) -C $$project cleanall; done
