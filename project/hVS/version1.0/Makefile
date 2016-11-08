Dir=busi/lobby/control/ \
busi/lobby/model/ \
busi/room/control/ \
busi/room/model/ \
cahe/control/ \
cahe/model/redis/ \
conf/ \
db/control/ \
db/model/mysql/ \
event/ \
log/ \
mem/ \
net/ \
tool/ 

testDir=testClient/

#---------------------------
Src=$(foreach dir,$(Dir), $(wildcard $(dir)*.cpp))
lobbySrc=$(Src) main/lobby_mian.cpp
roomSrc=$(Src) main/room_mian.cpp
testSrc=$(foreach dir,$(testDir), $(wildcard $(dir)*.cpp))

lobbyObj=$(patsubst %.cpp,%.o,$(lobbySrc))
roomObj=$(patsubst %.cpp,%.o,$(roomSrc))
testObj=$(patsubst %.cpp,%.o,$(testSrc))






#---------------------------
makeDir=./makeDir
lobby=bin/lobby
room=bin/room
testClient=bin/testClient

#---------------------------
Inc=Dir main/
LibDbMysql=-L../third-party/libs -lmysqlclient
CC=g++
CFLAGS=

#---------------------------

.PHONY: all clean

all:	$(makeDir) $(testClient) $(lobby)
	@echo all is ok

test:$(makeDir) $(testClient)
	@echo test is ok

$(makeDir):$(MAIN)
	mkdir -p bin/
	@echo $(lobbySrc)



#------------
$(testClient):$(testObj)
	$(CC) -o $@ $^

#------------ 一个cpp改了，lobby的obj都重编了，头文件改了，却不会重编。
$(lobby):$(lobbyObj)
	$(CC) -o $@ $^

clean:
	rm -rf $(testObj)
	rm -rf $(lobbyObj)
	rm -rf $(testClient)

$(Dir)%.o: $(lobbySrc)
	$(CC) -c -o $@  $(patsubst %.o,%.cpp,$@ )  -I./ 

