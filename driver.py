#!/usr/bin/python2

import os
import socket
import struct
import sys
import threading
import time


class Driver(object):

    def __init__(self, topo):
        """
            constructor
        """
        self.topo = topo
        self.host = '192.168.10.100'  # driver ip address
        self.port = 4747  # port number must match the one in router.py
        self.hosts = self.populate_hosts()  # populate hosts
        self.clk = 1  # number of sync clocks
        self.s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # open socket
        self.s.bind((self.host, self.port))  # bind to socket

    def populate_hosts(self):
        """
            populates list of hosts
        """
        temp = []
        with open(self.topo) as f:
            for line in f.readlines():
                segments = line.split()
                if segments[0] not in temp:
                    temp.append(segments[0])
                if segments[1] not in temp:
                    temp.append(segments[1])
        return temp

    def show_hosts(self):
        """
            shows a list of hosts
        """
        for host in self.hosts:  # iterate over hosts
            print(host)  # print host

    def show_help(self):
        """
            shows help menu
        """
        print('list of commands')
        print('----------------')
        print('help - shows a list of available commands')
        print('hosts - lists all hosts')
        print('cost <ip1> <ip2> <cost> - updates link cost between ip1 and ip2')
        print('down <ip1> <ip2> - deactivates link between ip1 and ip2')
        print('up <ip1> <ip2> - reactivates link between ip1 and ip2')
        print('send <ip1> <ip2> <message-length> <message> - instruct ip1 to send message to ip2')
        print('show <ip> - instruct ip to show its routing table')
        print('clear - clears the screen')
        print('exit - terminates the driver')

    def update_cost(self, command):
        """
            send cost to relevant routers
            Note: sending to relevant routers only
        """
        # 1 -> h1, 2 -> h2, 3 -> cost
        segments = command.split()  # split command on spaces
        if len(segments) != 4 or segments[1] not in self.hosts or segments[2] not in self.hosts:  # sanity check
            print('invalid arguments')
            return
        else:
            h1 = map(int, segments[1].split('.'))
            h2 = map(int, segments[2].split('.'))
            for host in [segments[1], segments[2]]:  # iterate over hosts
                print('sending cost update to {0}'.format(host))  # print a message before sending to each host
                buf = struct.pack('4s4B4Bh', segments[0], h1[0], h1[1], h1[2], h1[3], h2[0], h2[1], h2[2], h2[3],
                                  int(segments[3]))
                self.s.sendto(buf, (host, self.port))  # send to each host

    def send(self, command):
        """
            sends message from source to destination
        """
        # 1 -> src, 2 -> dest, 3:end -> message
        segments = command.split()  # split command on spaces
        if len(segments) < 5 or segments[1] not in self.hosts or segments[2] not in self.hosts:  # sanity check
            print('invalid arguments')
            return
        else:
            h1 = map(int, segments[1].split('.'))
            h2 = map(int, segments[2].split('.'))
            fmt = '4s4B4Bh' + segments[3] + 's'
            buf = struct.pack(fmt, segments[0], h1[0], h1[1], h1[2], h1[3], h2[0], h2[1], h2[2], h2[3],
                              int(segments[3]), ' '.join(segments[4:]))
            print('sending to {0}'.format(segments[1]))  # print a message before sending to source
            self.s.sendto(buf, (segments[1], self.port))  # send to source

    # self.s.sendto(command, (segments[1], self.port)) # send to source

    def show_rt(self, command):
        """
            instructs a router to show its routing table
        """
        # 1 -> router
        segments = command.split()
        if len(segments) != 2 or segments[1] not in self.hosts:
            print('invalid arguments')
            return
        else:
            router = map(int, segments[1].split('.'))
            fmt = '4s4B';
            buf = struct.pack(fmt, segments[0], router[0], router[1], router[2], router[3])
            print('sending to {0}'.format(segments[1]))
            self.s.sendto(buf, (segments[1], self.port))

    def link_down(self, command):
        """
            deactivates link between two routers
            Note: should be done both ways
        """
        # 1 -> h1, 2 -> h2
        segments = command.split()
        if len(segments) != 3 or segments[1] not in self.hosts or segments[2] not in self.hosts:  # sanity check
            print('invalid arguments')
            return
        else:
            os.system(
                'sudo iptables -I OUTPUT -s {0} -d {1} -j DROP'.format(segments[1], segments[2]))  # drop from h1 to h2
            os.system(
                'sudo iptables -I OUTPUT -s {0} -d {1} -j DROP'.format(segments[2], segments[1]))  # drop from h2 to h1

    def link_up(self, command):
        """
            reactivates link between two routers
            Note: should be done both ways
        """
        # 1 -> h1, 2 -> h2
        segments = command.split()
        if len(segments) != 3 or segments[1] not in self.hosts or segments[2] not in self.hosts:  # sanity check
            print('invalid arguments')
            return
        else:
            os.system('sudo iptables -I OUTPUT -s {0} -d {1} -j ACCEPT'.format(segments[1],
                                                                               segments[2]))  # accept from h1 to h2
            os.system('sudo iptables -I OUTPUT -s {0} -d {1} -j ACCEPT'.format(segments[2],
                                                                               segments[1]))  # accept from h2 to h1

    def send_clock(self):
        """
            sends clock to routers to exchange routing table
        """
        for host in self.hosts:  # iterate over hosts
            self.s.sendto('clk {0}'.format(self.clk), (host, self.port))  # send to each host
        self.clk += 1  # increment clock
        t = threading.Timer(5, self.send_clock)  # get a reference to timer
        t.daemon = True  # mark it daemonic
        t.start()  # start timer

    def run(self):
        """
            runs in an infinite loop
        """

        self.send_clock()

        print('type help to see a list of commands')
        while True:
            self.command = raw_input('> ')
            if self.command == 'help':
                self.show_help()
            elif self.command == 'hosts':
                self.show_hosts()
            elif self.command.startswith('cost'):
                self.update_cost(self.command)
            elif self.command.startswith('down'):
                self.link_down(self.command)
            elif self.command.startswith('up'):
                self.link_up(self.command)
            elif self.command.startswith('send'):
                self.send(self.command)
            elif self.command.startswith('show'):
                self.show_rt(self.command)
            elif self.command == 'clear':
                os.system('clear')
            elif self.command == 'exit':
                break

    def __del__(self):
        """
            destructor
        """
        self.s.close()


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('python driver.py <topo>')
        sys.exit(0)

    driver = Driver(sys.argv[1])
    driver.run()
