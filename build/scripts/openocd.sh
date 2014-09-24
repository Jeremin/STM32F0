#!/bin/bash

SCRIPT_PATH="/usr/local/Cellar/open-ocd/0.8.0/share/openocd/scripts/board/stm32f0discovery.cfg"

echo 'trying to shutdown OpenOCD in case its running...'
echo 'shutdown' | nc localhost 4444 -w 1 > /dev/null 2>&1 && sleep 1
echo 'starting OpenOCD'

openocd -f ${SCRIPT_PATH} -c init
