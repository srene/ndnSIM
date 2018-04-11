# Copyright (c) 2012,2015  Alexander Afanasyev <alexander.afanasyev@ucla.edu>

# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)

#########################
# Rate trace processing #
#########################
data = read.table("results_inrpp/rates.txt", header=T)
data$Node = factor(data$Node)
data$FaceId <- factor(data$FaceId)
data$bits <- data$Kilobytes * 8000
data$Type = factor(data$Type)

# exlude irrelevant types
data = subset(data, Type == "OutData") #%in% c("InInterests", "OutInterests", "InData", "OutData"))

# combine stats from all faces
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)

#data.root = subset (data.combined, Node %in% c("Src1","Src2","Src3"))
data.root = subset (data.combined, Node == "Dst1")#%in% c("Rtr1","Rtr2","Rtr3"))
#data.leaves = subset(data.combined, Node %in% c("leaf-1", "leaf-2", "leaf-3", "leaf-4"))


# graph rates on all nodes in Kilobits
#g.all <- ggplot(data.combined) +
# geom_point(aes (x=Time, y=bits.sum, color=Type), size=1) +
#  ylab("Goodput (bps)") +
#  facet_wrap(~ Node)

#print(g.all)

# graph rates on the root nodes in Packets
g.root2 <- ggplot(data.root) +
  geom_point(aes (x=Time, y=bits.sum, color=Node), size=2) +
  geom_line(aes (x=Time, y=bits.sum, color=Node), size=0.5) +
  ylab("Goodput (bps)") +
  xlab("Time (sec)") +
  scale_colour_manual(labels = c("Flow1", "Flow2","Flow3"), values = c("darkblue", "red","black")) +
  scale_x_continuous(limits = c(0, 30)) +
  theme(legend.position = "bottom")


print(g.root2)

#png("src/ndnSIM/docs/source/_static/root-rates.png", width=500, height=250)
#print(g.root2)
retval <- dev.off()
