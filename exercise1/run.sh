#!/bin/bash
net-snmp-config --compile-subagent a.out data.c pethNotifications.c powerEthernetMIB.c  && ./a.out -f -Lo -x  tcp:localhost:705

