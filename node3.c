#include <stdio.h>
#include "dvr.h"
#include <stdlib.h>

// Macros
#define INF 999
#define NODE 3

struct distance_table dt3;

struct rtpkt *minSyncPkt; 

// Prototypes
void sendNeighbour3(int dest);
void sendAll3();

void printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}

/* students to write the following two routines, and maybe some others */
void rtinit3() 
{
  printf("----------------------------------------------\n");
  printf("Node %d initialized at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");

  minSyncPkt = (struct rtpkt *)malloc( sizeof(struct rtpkt) );
  minSyncPkt->sourceid = 3;


  // Initialize the costs
  //-------------------------------------------------
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
        dt3.costs[i][j] = INF;
        dt3.costs[j][i] = INF;
    }
  }
  dt3.costs[NODE][NODE] = 0;

  dt3.costs[NODE][0] = 7;
  dt3.costs[NODE][2] = 2;

  dt3.costs[0][0] = 3;
  dt3.costs[2][2] = 2;
  //-------------------------------------------------

  minSyncPkt->mincost[0] = 3;
  minSyncPkt->mincost[1] = INF;
  minSyncPkt->mincost[2] = 2;
  minSyncPkt->mincost[3] = 0;

  printdt3(&dt3);
  sendAll3();
}

void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  int sid = rcvdpkt->sourceid;

  int dist2node;
  short changed = 0;

  printf("----------------------------------------------\n");
  printf("Change pkt received at Node %d at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");
  
  
  if (rcvdpkt->destid == 3) { //discard if not directed to it

    for (int i=0; i<4; i++) {
      if ((i != NODE) && (i != sid)) {
        
        dist2node = rcvdpkt-> mincost[i] + dt3.costs[sid][sid];
        printf("dist2node to node %d from %d is %d\n", i, sid, dist2node);
        dt3.costs[i][sid] = (dist2node > rcvdpkt-> mincost[i])?rcvdpkt-> mincost[i]:dist2node;

        if (dt3.costs[i][sid] > dist2node) {
          changed = 1;
          minSyncPkt->mincost[i] = dist2node;
        }  
      }
    }


    if (changed == 1) {
      for (int i=0; i<4; i++) {
        if ((i != NODE) && (i != sid)) {
          sendNeighbour3(i);   
        }
      }
    }
  }
  printdt3(&dt3);  
}

void sendNeighbour3(int dest) {
  minSyncPkt->destid = dest;
  tolayer2(*minSyncPkt);
}

void sendAll3() {
  sendNeighbour3(0);
  sendNeighbour3(2);
}