#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include "ZKClient.h"
#include "RpcHostWatcher.h"
#include "Derive.h"

//class TWatcher :public Watcher {
//public:
//
//	virtual int onChildrenChange(const s_vector& children) {
//		for (size_t i = 0; i < children.size(); ++i) {
//			std::cout << "url[" << i << "]:" << children[i] << std::endl;
//		}
//		return 0;
//	}
//
//	virtual int onDataChange(const std::string& data) {
//		std::cout << "data:" << data << std::endl;
//		return 0;
//
//	}
//
//private:
//
//};

typedef std::map<int, std::string> map_int_string;
struct A {
	char a;
	int* b;
	long c;
};

int main(int argc, char* argv[])
{
	try {
		//1-sizeof(struct A)
		std::cout << "sizeof(A)=" << sizeof(A) << std::endl;
		std::cout << "sizeof(long)=" << sizeof(long) << std::endl;
		std::cout << "sizeof(long *)=" << sizeof(long *) << std::endl;
		std::cout << "sizeof(int)=" << sizeof(int) << std::endl;
		std::cout << "sizeof(int *)=" << sizeof(int *) << std::endl;
		std::cout << "sizeof(double)=" << sizeof(double) << std::endl;

		//2-virtual Destruct
		Base *base = new Derive();
		base->test();
		delete base;

		//3-
		map_int_string m_svcons;
		m_svcons[10352] = "china";
		map_int_string m_svcons2;
		m_svcons2[0x2870] = "china";
		map_int_string::iterator it2 = m_svcons2.begin();
		map_int_string::iterator it = m_svcons.find(it2->first);
		if (it != m_svcons.end()) {
			std::cout << it->first << ": " << it->second << std::endl;
		}
		else {
			std::cout << "not found...\n";
		}

		//4-zookeeper client
		/*if (argc < 4) {
			std::cout << "test zkurl [watchchild|watchdata|watchall] path" << std::endl;
			return 0;
		}

		ZkClient cli;
		cli.init(argv[1], 100);

		std::string op = argv[2];
		std::string path = argv[3];

		TWatcher w;
		w.setPath(path);
		w.addToClient(&cli);

		if (op == "watchchild") {
			w.addWatch(Watcher::CHILD_EVENT);
		}
		else if (op == "watchdata") {
			w.addWatch(Watcher::CHANGE_EVENT);
		}
		else if (op == "watchall") {
			w.addWatch(Watcher::CHILD_EVENT | Watcher::CHANGE_EVENT);
		}*/

		if (argc < 3) {
			std::cout << "test zkurl path" << std::endl;
			return 0;
		}
		std::string url = argv[1];
		std::string path = argv[2];
		std::cout << "args url: " << url << std::endl;
		std::cout << "args path: " << path << std::endl;

		ZKClient cli;
		int ret = 0;
		if ((ret = cli.init(url, 100)) < 0) {
			std::cout << "zkclient init fail: " << ret << std::endl;
		}

		RpcHostWatcher* w = new RpcHostWatcher();
		w->init(&cli, path);

		while (1)
		{
#ifndef _WIN32
			sleep(1);
#endif // !_WIN32
		}

	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
