default: compile

yapmat: compile

compile:
	@mkdir -p bin/
	@g++ -std=c++14 src/yapmat.cpp -o bin/yapmat -lstdc++fs
	@g++ -std=c++14 src/wrapper.cpp -o bin/wrapper -lstdc++fs

run:
	@bin/yapmat

clean:
	@rm -f bin/yapmat
	@rm -f bin/wrapper
	@rm -rf /tmp/yapmat/*
