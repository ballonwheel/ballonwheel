#!/bin/bash

set -x

read pid_ads1115 < ./stop_pid_ads1115
kill -9 $pid_ads1115


