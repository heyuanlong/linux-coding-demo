Dir =  busi/lobby/control/ \
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

testDir = testClient/

Src = $(foreach dir,$(Dir), $(wildcard $(dir)*.cpp))
lobbySrc = Src main/lobby_mian.cpp
roomSrc = Src main/room_mian.cpp
testSrc =  $(foreach dir,$(testDir), $(wildcard $(dir)*.cpp))

lobbyObjDir = obj/lobby/
roomObjDir = obj/room/
testObjDir = obj/test/

lobbyFileSrc=$(notdir $(lobbySrc))
roomFileSrc=$(notdir $(roomSrc))
testFileSrc=$(notdir $(testSrc))
lobbyObj = $(patsubst %.cpp,$(lobbyObjDir)%.o,$(lobbyFileSrc))
roomObj = $(patsubst %.cpp,$(roomObjDir)%.o,$(roomFileSrc))
testObj = $(patsubst %.cpp,$(testObjDir)%.o,$(testFileSrc))


makeDir=./makeDir
lobby=bin/lobby
room=bin/room
testClient=bin/testClient

Inc = Dir main/
LibDbMysql=-L../third-party/libs -lmysqlclient


CC=g++
CFLAGS=


#------------------------------------------------------------



.PHONY: all

all:    $(makeDir) $(testClient)
	@echo $(Src)

test:$(makeDir) $(testClient)
	@echo $(testClient)

$(makeDir):$(MAIN)
	mkdir -p bin/
	mkdir -p obj/lobby
	mkdir -p obj/room
	mkdir -p obj/test

$(testClient):$(testObj)
	$(CC) -o $@ $^

$(testObj):$(testSrc)
	$(CC) -c -o $@ $<



clean:
