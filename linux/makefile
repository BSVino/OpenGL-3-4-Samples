PROJECTS = Example01 Example02 Example02a Example03 Example04 Example05 Example06 Example07 Example08 Example09 Example10 Example11 Example12 Example13 Example14

all:	main

main:
	cd GLUS && make GLUS && cd ..
	for i in $(PROJECTS); do cd $$i && make main && cd ..; done

clean:
	cd GLUS && make clean && cd ..
	for i in $(PROJECTS); do cd $$i && make clean && cd ..; done
