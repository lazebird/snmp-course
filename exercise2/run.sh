#!/bin/bash
# require libsnmp-dev package
net-snmp-config --compile-subagent a.out file_api.c fileTrap.c training.c  && ./a.out -f -Lo -x  tcp:localhost:705

