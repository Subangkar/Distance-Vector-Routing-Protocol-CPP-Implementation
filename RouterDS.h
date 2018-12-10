//
// Created by subangkar on 12/7/18.
//

#ifndef DVR_ROUTERDS_H
#define DVR_ROUTERDS_H

#include "Utils.h"
#include <unistd.h>
#include <ostream>
#include "Socket.h"

#define INF (std::numeric_limits<int>::max())
#define UP 1
#define DOWN 0

#define SHOW_ROUTING_TABLE "show"
#define RECV_ROUTING_TABLE string("ntbl")
#define SEND_MESSAGE "send"
#define FORWARD_MESSAGE string("frwd")
#define UPDATE_COST "cost"
#define DRIVER_CLOCK "clk"
#define CLEAR_SCREEN "clscr"


#define NONE "-\t"

typedef string routerip_t;
typedef string packet_t;
typedef int cost_t;

struct RoutingTableEntry {
	routerip_t destination;
	routerip_t nextHop;
	cost_t cost{};

	RoutingTableEntry() = default;

	RoutingTableEntry(const routerip_t &destination, const routerip_t &nextHop, cost_t cost) : destination(destination),
	                                                                                nextHop(nextHop), cost(cost) {}

	friend ostream &operator<<(ostream &os, const RoutingTableEntry &entry) {
		os << entry.destination << "\t" << setw(12) << entry.nextHop << "\t" << entry.cost;
		return os;
	}

	bool operator==(const RoutingTableEntry &rhs) const {
		return destination == rhs.destination &&
		       nextHop == rhs.nextHop &&
		       cost == rhs.cost;
	}

	bool operator!=(const RoutingTableEntry &rhs) const {
		return !(rhs == *this);
	}
};

struct Link {
	routerip_t neighbor;
	int cost;
	int recvClock;
	int status;

	Link() {
		cost = -1;
		recvClock = -1;
		status = DOWN;
	}

	Link(const routerip_t &neighbor, int cost, int recvClock, int status) : neighbor(neighbor), cost(cost),
	                                                                    recvClock(recvClock), status(status) {}

	friend ostream &operator<<(ostream &os, const Link &link1) {
		os << "neighbor: " << link1.neighbor << " cost: " << link1.cost << " recvClock: " << link1.recvClock
		   << " status: " << link1.status;
		return os;
	}

	bool operator==(const Link &rhs) const {
//		return neighbor == rhs.neighbor &&
//		       cost == rhs.cost &&
//		       recvClock == rhs.recvClock &&
//		       status == rhs.status;
		return neighbor == rhs.neighbor;
	}

	bool operator!=(const Link &rhs) const {
		return !(rhs == *this);
	}
};


typedef map<routerip_t, RoutingTableEntry> routingtable_t;

#endif //DVR_ROUTERDS_H
