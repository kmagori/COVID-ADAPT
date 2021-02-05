library(Rcpp)
sourceCpp(file="COVID-ADAPT/COVID-ADAPT.cpp")

virus_levels=read.csv(file="virus_levels.csv",header=TRUE)
people=read.csv(file="people.csv",header=TRUE)

people$position=as.numeric(substring(people$position,7,length(people$position)))-1
people$x=people$position %% 5
people$y=abs((people$position %/% 5) - 4)


viruslevels=virus_levels[,3:27]
current_viruslevels=matrix(as.numeric(viruslevels[1,]),nrow=5,ncol=5)
library(raster)
library(gifski)
r=raster(ncol=5,nrow=5,xmn=0,xmx=5,ymn=0,ymx=5)

cuts=as.integer(seq(0,round(max(viruslevels)),length.out=10)) #set breaks
pal <- colorRampPalette(c("white","red"))

makeplot_raster<-function(){
for (i in 1:dim(viruslevels)[1])
{
  png(sprintf("frame%03d.png",i))
  values(r)=as.numeric(viruslevels[i,])
#  values(r)=current_viruslevels
plot(r,xlim=c(0,5),ylim=c(0,5),asp=NA,breaks=cuts,col=pal(10))
for (j in 1:5) lines(c(0,5),c(j,j))
for (j in 1:5) lines(c(j,j),c(0,5))
title(main=paste("Timepoint: ",virus_levels$time[i]," minutes."))
points(people$x[which(people$time==virus_levels$time[i])]+0.5,people$y[which(people$time==virus_levels$time[i])]+0.5,pch=people$status[which(people$time==virus_levels$time[i])],cex=2)
dev.off()
}
}

pngfiles=sprintf("frame%03d.png",1:dim(viruslevels)[1])
makeplot_raster()

gifski(pngfiles,"raster.gif",loop=FALSE,delay=0.5)
