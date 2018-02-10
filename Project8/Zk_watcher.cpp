#include "ZKClient.h"
#include "Zk_watcher.h"

volatile int ZKWatcher::s_id = 0;

ZKWatcher::ZKWatcher(const std::string& path)
	:m_cli(NULL), m_path(path), m_event_mask(0),
	m_check_exists(false) {
	__sync_add_and_fetch(&s_id, 1);
	m_id = s_id;
}

ZKWatcher::~ZKWatcher() {
	delFromClient();
}

int ZKWatcher::delFromClient() {
	if (m_cli) {
		m_cli->delWatcher(this);
		m_cli = NULL;
	}

	return 0;
}

int ZKWatcher::setClient(ZKClient* c) {
	m_cli = c;
	return 0;
}

int ZKWatcher::checkExists() {
	if (!m_cli) {
		return 0;
	}

	if (m_check_exists) {
		return 0;
	}

	m_check_exists = true;

	return m_cli->watchCreate(this);

}

int ZKWatcher::addToClient(ZKClient* cli) {
	cli->addWatcher(this);
	setClient(cli);

	return 0;
}

int ZKWatcher::addWatch(int eventflag) {
	if (!m_cli) {
		return -1;
	}

	if (eventflag & CHANGE_EVENT) {
		m_cli->watchData(this);
	}

	if (eventflag & CHILD_EVENT) {
		m_cli->watchChildren(this);
	}

	if (eventflag & CREATE_EVENT || eventflag & DELETE_EVENT) {
		m_cli->watchExists(this);
	}

	m_event_mask |= eventflag;

	return 0;
}

int ZKWatcher::handleEvent(int event) {
	if (event & m_event_mask) {
		addWatch(event);
	}

	switch (event) {
	case DELETE_EVENT:
		if (!(event & m_event_mask)) {
			checkExists();
		}
		break;
	case CREATE_EVENT:
		if (!(event & m_event_mask)) {
			nodeExists();
		}
		break;
	}

	return 0;
}

int ZKWatcher::nodeExists() {
	if (!m_cli) {
		return -1;
	}

	if (m_event_mask & CHANGE_EVENT) {
		m_cli->watchData(this);
	}

	if (m_event_mask & CHILD_EVENT) {
		m_cli->watchChildren(this);
	}

	if (m_event_mask & CREATE_EVENT || m_event_mask & DELETE_EVENT) {
		m_cli->watchExists(this);
	}

	m_check_exists = false;

	return 0;
}

