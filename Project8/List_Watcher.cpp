#include "ZKClient.h"
#include "List_Watcher.h"

int ListWatcherBase::init(ZKClient* c, const std::string& path, bool loaddata)
{
	setPath(path);
	addToClient(c);
	addWatch(ZKWatcher::CHILD_EVENT);
	//addWatch(ZKWatcher::CREATE_EVENT);
	m_loaddata = loaddata;
	return 0;
}

int ListWatcherBase::getChildren(ChildrenMap& m) {
	s_vector v;

	int ret = getClient()->getChildren(m_path, v);
	if (ret < 0)
		return ret;

	ret = getChildrenData(v, m);

	return ret;
}

int ListWatcherBase::getChildrenData(const std::vector<std::string>& v, ChildrenMap& m) {
	std::cout << "[" << __FUNCTION__ << "]  size=" << v.size()<< std::endl;
	for (unsigned int n = 0; n < v.size(); n++)
	{
		std::string p = zkPath(m_path, v[n]);
		std::string data;
		int ret = getClient()->getNodeData(p, data);
		if (ret < 0) {
			return ret;
		}
		m[v[n]] = data;
	}

	return 0;
}

int ListWatcherBase::onChildrenChange(const std::vector<std::string>& children) {
	std::cout << "[" << __FUNCTION__ << "]  size="<<children.size() << std::endl;
	ChildrenMap m;

	int ret = 0;

	if (m_loaddata) {
	std::cout << "[" << __FUNCTION__ << "]  load_data" << std::endl;
		ret = getChildrenData(children, m);
	}
	else {
		for (std::vector<std::string>::const_iterator it = children.begin();
			it != children.end(); it++) {
			m[*it] = std::string();
		}
	}

	if (ret >= 0)
		return onListChange(m);
	return 0;
}

int SvListWatcherBase::getChildren(std::string& jm) {
	ChildrenMap m;
	int ret = ListWatcherBase::getChildren(m);

	ChildrenMap::const_iterator it = m.begin();
	for (; it != m.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}

	return ret;
}


int SvListWatcherBase::onListChange(const ChildrenMap& c) {
	std::cout << "[" << __FUNCTION__ << "]  wtf" << std::endl;
	int ret = 0;
	//std::string m;

	ChildrenMap::const_iterator it = c.begin();
	for (; it != c.end(); ++it) {
		std::cout << "[" << __FUNCTION__ << "]  second="<<it->second << std::endl;
		ret = onServerListChange(it->second);
	}

	return ret;
}

