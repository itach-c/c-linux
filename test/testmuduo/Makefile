tcp_server: test.cc
	g++ -o $@ $^ -std=c++11 -I./include -L./lib -lmuduo_net -lmuduo_base -lmuduo_http -lpthread
.PHONY:clean
clean:
	rm -rf tcp_server