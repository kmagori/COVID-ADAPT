library(Rcpp)
library(raster)
#sourceCpp(file="COVID-ADAPT/COVID-ADAPT.cpp")

virus_levels=read.csv(file="virus_levels.csv",header=TRUE)
people=read.csv(file="people.csv",header=TRUE)
layout=read.csv(file="layout.csv",header=FALSE)

#gridsize=sqrt(dim(virus_levels)[2]-2)
gridsize_x=19;
gridsize_y=9;


layout$position=seq(0,dim(layout)[1]-1,1)
layout$x=layout$position %% gridsize_x
layout$y=abs((layout$position %/% gridsize_x) - (gridsize_y-1))
  


people$position=as.numeric(substring(people$position,7,length(people$position)))-1
people$x=people$position %% gridsize_x
people$y=abs((people$position %/% gridsize_x) - (gridsize_y-1))


viruslevels=virus_levels[,3:dim(virus_levels)[2]]
current_viruslevels=matrix(as.numeric(viruslevels[1,]),nrow=gridsize_x,ncol=gridsize_y)
library(raster)
library(gifski)
r=raster(ncol=gridsize_x,nrow=gridsize_y,xmn=0,xmx=gridsize_x,ymn=0,ymx=gridsize_y)

cuts=as.integer(seq(0,round(max(viruslevels)),length.out=10)) #set breaks
pal <- colorRampPalette(c("white","red"))

makeplot_raster<-function(){
for (i in 1:dim(viruslevels)[1])
{
  png(sprintf("frame%03d.png",i))
  values(r)=as.numeric(viruslevels[i,])
#  values(r)=current_viruslevels
  par(mar=c(5,4,4,2))
#plot(0, xlim=c(0,gridsize_x+1),ylim=c(0,gridsize_y),xaxt = 'n', yaxt = 'n', bty = 'n', pch = '', ylab = '', xlab = '')
#rasterImage(as.raster(r),0,0,gridsize_x,gridsize_y,interpolate=FALSE)
plot(r,asp=NA,breaks=cuts,col=pal(10),xaxs="i",yaxs="i",add=FALSE)

#plot(r,breaks=cuts,col=pal(10))
#for (j in 1:gridsize) lines(c(0,gridsize),c(j,j),col="grey")
#for (j in 1:gridsize) lines(c(j,j),c(0,gridsize),col="grey")
#adding walls
for (j in 1:dim(layout)[1])
{
  lines(c(layout$x[j],layout$x[j]),c(layout$y[j]+1,layout$y[j]),col=rgb(layout$V1[j]*0.9,layout$V1[j]*0.9,layout$V1[j]*0.9))
  lines(c(layout$x[j]+1,layout$x[j]+1),c(layout$y[j]+1,layout$y[j]),col=rgb(layout$V2[j]*0.9,layout$V2[j]*0.9,layout$V2[j]*0.9))
  lines(c(layout$x[j],layout$x[j]+1),c(layout$y[j]+1,layout$y[j]+1),col=rgb(layout$V3[j]*0.9,layout$V3[j]*0.9,layout$V3[j]*0.9))
  lines(c(layout$x[j],layout$x[j]+1),c(layout$y[j],layout$y[j]),col=rgb(layout$V4[j]*0.9,layout$V4[j]*0.9,layout$V4[j]*0.9))
}
title(main=paste("Timepoint: ",virus_levels$time[i]," minutes."))
points(people$x[which(people$time==virus_levels$time[i])]+0.5,people$y[which(people$time==virus_levels$time[i])]+0.5,pch=people$status[which(people$time==virus_levels$time[i])],cex=10/gridsize_y,col=c('red','blue')[people$masked+1])
dev.off()
}
}

pngfiles=sprintf("frame%03d.png",1:dim(viruslevels)[1])
makeplot_raster()

gifski(pngfiles,"raster.gif",loop=FALSE,delay=0.5)

#plotting virus levels over time
virus_levels$Total_virus=rowSums(virus_levels[,3:dim(virus_levels)[2]])
virus_levels$Mean_virus=rowMeans(virus_levels[,3:dim(virus_levels)[2]])
library(ggplot2)
ggplot(virus_levels,aes(x=time,y=Total_virus))+geom_point()+theme_classic()
ggplot(virus_levels,aes(x=time,y=Mean_virus))+geom_point()+theme_classic()

#plotting number incidence over time -defined as #infectious
incidence=aggregate(people[,4],by=list(people$time),FUN=function(x) length(which(x==2)))
ggplot(incidence,aes(x=Group.1,y=x))+geom_point()+theme_classic()      

#plotting prevalence over time - defined as #infectious+recovered
prevalence=aggregate(people[,4],by=list(people$time),FUN=function(x) length(which(x==2|x==3)))
ggplot(prevalence,aes(x=Group.1,y=x))+geom_point()+theme_classic()      

