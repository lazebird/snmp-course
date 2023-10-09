all: powerEthernetMIB
CC = gcc
CFLAGS = -g -pedantic -I/usr/local/snmp/include -I/usr/include
TFLAFS = -L/usr/local/snmp/lib -lpthread -lnetsnmpagent -lnetsnmpmibs -lnetsnmphelpers -lnetsnmp
OBJS = powerEthernetMIB.o pethNotifications.o data.o netsnmptmp.122581.o
 
powerEthernetMIB:$(OBJS)
	$(CC)	$(OBJS)  -lpthread -o powerEthernetMIB $(TFLAFS)


$(OBJS):%.o:%.c
	$(CC)	-c $< $(CFLAGS) -o $@

.PHONY:clean
clean:
	rm -f powerEthernetMIB $(OBJS)

.PHONY:run
run:
	./powerEthernetMIB -f -Lo -x  tcp:localhost:705

.PHONY:snmpd
snmpd:
	snmpd -c /usr/local/snmp/snmpd.conf