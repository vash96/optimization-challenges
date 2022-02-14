DEBUG_OPTS = -std=c++17 -O3 -Wall -Wfatal-errors -Wextra -pedantic
RUN_OPTS = -std=c++17 -O3 -DNDEBUG -fopenmp

greedy-debug: src/greedy.cpp
	g++ $(DEBUG_OPTS) src/greedy.cpp -o bin/greedy-debug

greedy: src/greedy.cpp
	g++ $(RUN_OPTS) src/greedy.cpp -o bin/greedy

opt-debug: src/optimizer.cpp
	g++ $(DEBUG_OPTS) src/optimizer.cpp -o bin/opt-debug

opt: src/optimizer.cpp
	g++ $(RUN_OPTS) src/optimizer.cpp -o bin/opt


clean:
	rm -f greedy-debug greedy opt-debug opt


.PHONY: clean greedy-debug greedy opt-debug opt
