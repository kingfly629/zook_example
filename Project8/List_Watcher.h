#ifndef __LIST_WATCHER_H__
#define __LIST_WATCHER_H__

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <json/json.h>
#include "Zk_watcher.h"

typedef std::map<std::string/*id*/, std::string>  ChildrenMap;
typedef int(*ChildrenWatcherCallBack)(void* ctx, const ChildrenMap& children);

class ZKClient;
class ListWatcherBase :public ZKWatcher
{
public:
	ListWatcherBase() :m_loaddata(true) {};
	virtual ~ListWatcherBase() {};

	int init(ZKClient* c, const std::string& path, bool loaddata = true);

	int getChildren(ChildrenMap& m);

	virtual int onChildrenChange(const std::vector<std::string>& children);

	virtual int onListChange(const ChildrenMap& c) {
		return 0;
	}

private:
	int getChildrenData(const std::vector<std::string>& v, ChildrenMap& m);
private:
	bool m_loaddata;
};

class SvListWatcherBase : public ListWatcherBase {
public:
	virtual ~SvListWatcherBase() {}
	int getChildren(std::string& m);
	virtual int onListChange(const ChildrenMap& c);
	virtual int onServerListChange(const std::string& c) {
		return 0;
	}
private:
};

template<class T>
class SvListWatcher :public SvListWatcherBase {
public:
	typedef int (T::*CB)(const std::string&);

	SvListWatcher(T* t, CB c) :m_ctx(t), m_cb(c) {}
	virtual ~SvListWatcher() {}

	virtual int onServerListChange(const std::string& c) {

	std::cout << "[" << __FUNCTION__ << "]  wtf" << std::endl;
		if (m_ctx)
			return (m_ctx->*m_cb)(c);

		return 0;
	}

private:
	T* m_ctx;
	CB m_cb;
};

#endif //__LIST_WATCHER_H__ 
