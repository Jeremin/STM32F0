#!/bin/bash

echo 'shutting down OpenOCD'
echo 'shutdown' | nc localhost 4444 -w 1 > /dev/null 2>&1 && sleep 1