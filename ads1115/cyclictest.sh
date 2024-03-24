

#https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/preemptrt_setup



#https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rt-tests#compile-and-install

# sudo apt-get install build-essential libnuma-dev
# git clone git://git.kernel.org/pub/scm/utils/rt-tests/rt-tests.git
# cd rt-tests
# git checkout stable/v1.0
# make all
# make install


#https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/start
sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0


#https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/tracing
#cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0
#cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --breaktrace=2000 --tracemark
#cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --breaktrace=180 --tracemark


