DEBUG_OPTS = -std=c++20 -O3 -Wall -Wfatal-errors -Wextra -pedantic
RUN_OPTS = -std=c++20 -O3 -DNDEBUG -fopenmp

greedy-debug: src/greedy.cpp
	g++ $(DEBUG_OPTS) src/greedy.cpp -o bin/greedy-debug

greedy: src/greedy.cpp
	g++ $(RUN_OPTS) src/greedy.cpp -o bin/greedy

opt-debug: src/optimizer.cpp
	g++ $(DEBUG_OPTS) src/optimizer.cpp -o bin/opt-debug

opt: src/optimizer.cpp
	g++ $(RUN_OPTS) src/optimizer.cpp -o bin/opt


clean:
	rm -f bin/greedy-debug bin/greedy bin/opt-debug bin/opt

zip-greedy:
	zip archive.zip src/utils.cpp src/greedy.cpp

zip-opt:
	zip archive.zip src/utils.cpp src/greedy.cpp src/optimizer.cpp


.PHONY: clean greedy-debug greedy opt-debug opt
