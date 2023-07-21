#!/bin/bash

set -x

read pid_octave < ./stop_pid_octave
read pid_a < ./stop_pid_a
kill -9 $pid_a
kill -9 $pid_octave


