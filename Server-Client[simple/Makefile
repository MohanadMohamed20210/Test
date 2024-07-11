

compile:
	g++ app.cpp -o app -lwiringPi
	gcc test.c -o out

run:
	sudo chmod u+x script.sh
	sudo chmod u+x script2.sh
	nohup ./script.sh > output.log 2>&1 &

stop:
	# rm app out
	kill -2  $(shell ps -aux | grep script.sh | cut -d" " -f 5,6 | head -n 1)
	kill -2  $(shell ps -aux | grep script2.sh | cut -d" " -f 5,6 | head -n 1)
