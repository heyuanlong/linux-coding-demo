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

#------------ 
$(lobby):$(lobbyObj)
	$(CC) -o $@ $^



busi/lobby/control/%.o:busi/lobby/control/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
busi/lobby/model/%.o:busi/lobby/model/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
busi/room/control/%.o:busi/room/control/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
busi/room/model/%.o:busi/room/model/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
cahe/control/%.o:cahe/control/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
cahe/model/redis/%.o:cahe/model/redis/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
conf/%.o:conf/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
db/control/%.o:db/control/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
db/model/mysql/%.o:db/model/mysql/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
event/%.o:event/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
log/%.o:log/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
mem/%.o:mem/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
net/%.o:net/%.cpp
	$(CC)  -c -o $@ $^ -I./ 
tool/%.o:tool/%.cpp
	$(CC)  -c -o $@ $^ -I./ 

main/lobby_mian.o:main/lobby_mian.cpp
	$(CC)  -c -o $@ $^ -I./ 


clean:
	rm -rf $(testObj)
	rm -rf $(lobbyObj)
	rm -rf $(testClient)
	rm -rf $(lobby)
	rm -rf $(room)


# $(Dir)%.o: $(lobbySrc)
# 	$(CC) -c -o $@  $(patsubst %.o,%.cpp,$@ )  -I./ 

