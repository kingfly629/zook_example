#ifndef __RPCHOSTWATCHER_H
#define __RPCHOSTWATCHER_H

#include <string>

class ZKClient;
class SvListWatcherBase;
class RpcHostWatcher {
public:
	int init(ZKClient* c, const std::string& zkpath);
private:
	int watchCallBack(const std::string& what);
private:
	SvListWatcherBase* m_obs;
};
#endif//RPCHOSTWATCHER_H