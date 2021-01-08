COVIDADAPT=read.csv(file="record.csv",header=TRUE)
viruslevels=COVIDADAPT[,6:30]
current_viruslevels=matrix(as.numeric(viruslevels[1,]),nrow=5,ncol=5)
library(raster)
r=raster(ncol=5,nrow=5)
values(r)=as.numeric(viruslevels[1953,])
plot(r)
