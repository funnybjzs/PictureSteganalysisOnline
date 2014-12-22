all:Condition

IncludePath=/usr/include/oracle/12.1/client64
LibPath=/usr/lib/oracle/12.1/client64/lib

Condition:RecvCmdAndConfig.cpp SendHeartbeat.cpp OraDBOpration.cpp Config.cpp Condition.cpp
	gcc -g -Wall -o $@ $^ -I ${IncludePath} -L ${LibPath} -locci -lclntsh -lnnz12 -lons -lclntshcore -lpthread
.PHONY:
clean:
	- $(RM) *.o
	- $(RM) Condition
