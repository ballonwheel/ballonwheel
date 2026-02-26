#!/bin/bash

set -x

read pid_octave < ./stop_pid_octave
kill -9 $pid_octave


