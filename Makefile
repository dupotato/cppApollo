example=client
default:
	#g++ *.cpp  -o client -g -D_DEBUG_ -fpermissive -lcurl -l ssl -l crypto -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -l jsoncpp -L/usr/lib64 -I./libs/jsoncpp/include -L./libs/jsoncpp/lib
	g++ *.cpp  -o $(example) -g  -fpermissive -lcurl -l ssl -l crypto -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -l jsoncpp -L/usr/lib64 -I./libs/jsoncpp/include -L./libs/jsoncpp/lib
clean:
	rm -f *.o core.* $(example)
