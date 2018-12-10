This is a C++17 implementation of [Distance Vector Routing Protocol](https://en.wikipedia.org/wiki/Distance-vector_routing_protocol) using Virtual Ethernet Interface of **Linux Operating System**.  
This implementation was done as an assignment of Computer Networking Sessional Course of Department of Computer Science & Engineering, BUET  
  
  
  
Files Description:  
  - "setup.sh"      : Contains the necessary shell commands to setup virtual interface  
  - "unsetup.sh"    : Contains the necessary shell commands to remove the created virtual interface  
  - "driver.py"     : Contains the python program codes to operate the routers in virtual interface  
  - "topo.txt"      : Contains topology info of the network.(whole network picture given in "Assignment_spec.pdf")
  - "router.cpp"    : Contains codes of router program.  
  - all Other Files : Contains utility codes for router.cpp  
  - "Testing Commands": Contains some sample commands for "driver.py"
  
  
  
System Requirements:  
  - Linux Operating System (Tested on Ubuntu 18, 19beta)  
  - C++17 or higher (Tested using GNU G++ 8.2) 
  - Python2
  - Here IPs of virtual interfaces used from "192.168.10.[1-4]" and IP of driver is "192.168.10.100". If your system uses any of these IPs then you must change these in "topo.txt" and "driver.py". Moreover you should change your ethernet interface name ("enp1s0" here) according to your machine interface name (can be determined by "ifconfig" command) in "topo.txt"

  
  
To simulate the protocol:  
- First run "setup.sh" shell script in terminal  
    > ./setup.sh  
- Then compile and create router executable file from "router.cpp"  
    > c++ router.cpp -std=c++17 -o router.out  
- Run "router.out" with two arguments, router-ip as 1st argument and topology-file-name as 2nd one in separate terminals for each ip. 
    > ./router.out router-ip topo.txt  
- Finally run the "driver.py"   
    > ./driver.py topo.txt  
- Now, type help in python console to see the available commands.  
  
Sample Demonstration:  
  - [Terminal:01]   
    >> ./router.out 192.168.10.1 topo.txt  
  - [Terminal:02]   
    >> ./router.out 192.168.10.2 topo.txt  
  - [Terminal:03]  
    >> ./router.out 192.168.10.3 topo.txt  
  - [Terminal:04]   
    >> ./router.out 192.168.10.4 topo.txt  
  - [Terminal:05]   
    >> ./driver.py topo.txt  
    >> help  
    >> show 192.168.10.1  
  
  
For further details, see "Assignment_spec.pdf".  
  
This has been created as a CLion Project. That's why "CMakeLists.txt" exists here.
