compile: 
	clang++ -g -std=c++14 ./Code/*.cpp -o output

run:
	./output
	@echo "Running Python visualization..."
	@conda run -n intromlenv python ./Code/visualize_detailed_profiling.py
	@conda run -n intromlenv python ./Code/visualize_profilerStats.py
