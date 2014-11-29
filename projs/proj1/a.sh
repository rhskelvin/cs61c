echo 1
hc large jar proj1.jar Proj1 -conf conf.xml -Dcombiner=false  -DrunJob2=true s3n://cs61cUsenet/2005-tmp  hdfs:///intermediate11 hdfs:///output11 2>&1 | tee log1
echo 1 done
echo 2
hc large jar proj1.jar Proj1 -conf conf.xml -Dcombiner=true  -DrunJob2=true s3n://cs61cUsenet/2005-tmp  hdfs:///intermediate22 hdfs:///output22 2>&1 | tee log2
echo 2 done
echo 3
hc large jar proj1.jar Proj1 -conf conf.xml -Dcombiner=true  -DrunJob2=true s3n://cs61cUsenet/s2006  hdfs:///intermediate33 hdfs:///output33 2>&1 | tee log3
echo 3 done
