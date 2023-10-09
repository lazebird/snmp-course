all: trainingMIB
CC = gcc
CFLAGS = -g -pedantic -I/usr/local/snmp/include -I/usr/include
TFLAFS = -L/usr/local/snmp/lib -lpthread -lnetsnmpagent -lnetsnmpmibs -lnetsnmphelpers -lnetsnmp
OBJS = training.o netsnmptmp.13479.o data.o fileTrap.o
 
trainingMIB:$(OBJS)
	$(CC)	$(OBJS)  -lpthread -o trainingMIB $(TFLAFS)


$(OBJS):%.o:%.c
	$(CC)	-c $< $(CFLAGS) -o $@

.PHONY:clean
clean:
	rm -f trainingMIB $(OBJS)

.PHONY:run
run:
	./trainingMIB -f -Lo -x  tcp:localhost:705

.PHONY:snmpd
snmpd:
	snmpd -c /usr/local/snmp/snmpd.conf