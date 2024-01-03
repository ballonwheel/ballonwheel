
//mkdir bow_tmpfs
//sudo mount -t tmpfs tmpfs ./bow_tmpfs/
//../scilab-6.0.2/bin/scilab-cli -f bow.sci 
//hexdump /bow_tmpfs/getbyte


fd_wb = mopen("./bow_tmpfs/getbyte", 'wb');
mput(65, 'c',  fd_wb);

mclose(fd_wb);
