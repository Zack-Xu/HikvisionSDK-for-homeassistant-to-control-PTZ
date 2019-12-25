## HOW TO USE

### 1.download the hik folder to your device which run the homeassistant


###	2.enter the xx/xx/hik, then use the make command .
	the make command: just type in make then press Enter
	if your device dont have g++ ,you should apt-get install g++
	
### 3.then you will have a new file named hikvision. use it like this ./hikvision -h
	Im not good at doing this so it may show some other things not just the help list

### 4.follow the help ,you may try command like this ./hikvision -I 192.168.1.22 -p 8000 -U admin -P 12345 -u 1
	I for camera_ip;
	p for port，usually is 8000;
	U for Username; 
	P for Password;
	u for turn up。
	 you can find other cammand in help
	
### 5.make a shell_command into homeassistant like the example
###   make sure you use cd cammand to your hikvision  folder just like the example
	shell_command:
	up: cd ~/hik&&./hikvision -I 192.168.1.22 -p 8000 -U admin -P 12345 -u 1
	down: cd ~/hik&&./hikvision -I 192.168.1.22 -p 8000 -U admin -P 12345 -d 1
	left: cd ~/hik&&./hikvision -I 192.168.1.22 -p 8000 -U admin -P 12345 -l 1
	right: cd ~/hik&&./hikvision -I 192.168.1.22 -p 8000 -U admin -P 12345 -r 1