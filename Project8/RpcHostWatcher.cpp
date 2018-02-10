#include "ZKClient.h"
#include "List_Watcher.h"
#include "RpcHostWatcher.h"

int RpcHostWatcher::init(ZKClient* c, const std::string& zkpath) {
	m_obs = new SvListWatcher<RpcHostWatcher>(this, &RpcHostWatcher::watchCallBack);
	return m_obs->init(c, zkpath);
}

int RpcHostWatcher::watchCallBack(const std::string& what) {
	try {
		std::cout << "here watchCallBack: " << what << "\n";
	}
	catch (const std::exception& e) {
		std::cout << "RpcHostWatcher::watchCallBack ex:" << e.what() << std::endl;
	}

	return 0;
}
