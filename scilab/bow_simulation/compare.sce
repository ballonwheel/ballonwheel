



meas1_time=csvRead('/home/imre/ballonwheel/scilab/bow_simulation/meas1_time.csv')
meas1_values=csvRead('/home/imre/ballonwheel/scilab/bow_simulation/meas1_values.csv')
meas2_values=csvRead('/home/imre/ballonwheel/scilab/bow_simulation/meas2_values.csv')
meas3_values=csvRead('/home/imre/ballonwheel/scilab/bow_simulation/meas3_values.csv')
meas4_values=csvRead('/home/imre/ballonwheel/scilab/bow_simulation/meas4_values.csv')


subplot(2,1,1)
plot(meas1_time, meas1_values(:,1))
plot(meas1_time, meas2_values(:,2))
plot(meas1_time, meas3_values(:,2))
plot(meas1_time, meas4_values(:,1))

subplot(2,1,2)
plot(meas1_time, meas1_values(:,2))
plot(meas1_time, meas2_values(:,1))
plot(meas1_time, meas3_values(:,1))
plot(meas1_time, meas4_values(:,2))



