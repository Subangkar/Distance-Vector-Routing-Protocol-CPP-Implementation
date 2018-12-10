#!/usr/bin/env bash
sudo ifconfig enp0s3:1 192.168.10.1 netmask 255.255.255.0 down
sudo ifconfig enp0s3:2 192.168.10.2 netmask 255.255.255.0 down
sudo ifconfig enp0s3:3 192.168.10.3 netmask 255.255.255.0 down
sudo ifconfig enp0s3:4 192.168.10.4 netmask 255.255.255.0 down
sudo ifconfig enp0s3:5 192.168.10.5 netmask 255.255.255.0 down
sudo ifconfig enp0s3:100 192.168.10.100 netmask 255.255.255.0 down

#sudo ifconfig eth0:1 192.168.10.1 netmask 255.255.255.0 down
#sudo ifconfig eth0:2 192.168.10.2 netmask 255.255.255.0 down
#sudo ifconfig eth0:3 192.168.10.3 netmask 255.255.255.0 down
#sudo ifconfig eth0:4 192.168.10.4 netmask 255.255.255.0 down
#sudo ifconfig eth0:100 192.168.10.100 netmask 255.255.255.0 down

# sudo ifconfig enp0s3:1 10.0.2.1 netmask 255.255.255.0 down
# sudo ifconfig enp0s3:2 10.0.2.2 netmask 255.255.255.0 down
# sudo ifconfig enp0s3:3 10.0.2.3 netmask 255.255.255.0 down
# sudo ifconfig enp0s3:4 10.0.2.4 netmask 255.255.255.0 down
# sudo ifconfig enp0s3:100 10.0.2.100 netmask 255.255.255.0 down
