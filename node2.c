#include <stdio.h>
#include "dvr.h"
#include <stdlib.h>

// Macros
#define INF 999
#define NODE 2

struct distance_table dt2;

struct rtpkt *minSyncPkt; 

// Prototypes
void sendNeighbour2(int dest);
void sendAll2();

void printdt2(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
   dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
   dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
   dtptr->costs[3][1],dtptr->costs[3][3]);
}

/* students to write the following two routines, and maybe some others */
void rtinit2() 
{
  printf("----------------------------------------------\n");
  printf("Node %d initialized at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");
  
  
  minSyncPkt = (struct rtpkt *)malloc( sizeof(struct rtpkt) );
  minSyncPkt->sourceid = 2;


  // Initialize the costs
  //-------------------------------------------------
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
        dt2.costs[i][j] = INF;
        dt2.costs[j][i] = INF;
    }
  }
  dt2.costs[NODE][NODE] = 0;

  dt2.costs[NODE][0] = 3;
  dt2.costs[NODE][1] = 1;
  dt2.costs[NODE][3] = 2;

  dt2.costs[0][0] = 3;
  dt2.costs[1][1] = 1;
  dt2.costs[3][3] = 2;

  //-------------------------------------------------

  printdt2(&dt2);
  sendAll2();
}

void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  int sid = rcvdpkt->sourceid;

  int dist2node;
  short changed = 0;

  printf("----------------------------------------------\n");
  printf("Change pkt received at Node %d at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");
  
  
  if (rcvdpkt->destid == 0) { //discard if not directed to it

    for (int i=0; i<4; i++) {
      if ((i != NODE) && (i != sid)) {
        
        dist2node = rcvdpkt-> mincost[i] + dt2.costs[sid][sid];
        dt2.costs[i][sid] = dist2node;

        if (dt2.costs[i][sid] > dist2node) {
          changed = 1;
          minSyncPkt->mincost[i] = dist2node;
        }  
      }
    }


    if (changed == 1) {
      for (int i=0; i<4; i++) {
        if ((i != NODE) && (i != sid)) {
          sendNeighbour2(i);   
        }
      }
    }
  }
  printdt2(&dt2);  
}

void sendNeighbour2(int dest) {
  minSyncPkt->destid = dest;
  tolayer2(minSyncPkt);
}

void sendAll2() {
  sendNeighbour2(0);
  sendNeighbour2(1);
  sendNeighbour2(3);
}