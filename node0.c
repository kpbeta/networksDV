#include <stdio.h>
#include "dvr.h"
#include <stdlib.h>

// Macros
#define INF 999
#define NODE 0

struct distance_table dt0;

struct rtpkt *minSyncPkt; 

// Prototypes
void sendNeighbour0(int dest);
void sendAll0();

void printdt0(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
   dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
   dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
   dtptr->costs[3][2],dtptr->costs[3][3]);
}


/* students to write the following two routines, and maybe some others */
void rtinit0() 
{
  printf("----------------------------------------------\n");
  printf("Node %d initialized at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");
  
  minSyncPkt = (struct rtpkt *)malloc( sizeof(struct rtpkt) );
  minSyncPkt->sourceid = NODE;

  // Initialize the costs
  //-------------------------------------------------
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
        dt0.costs[i][j] = INF;
        dt0.costs[j][i] = INF;
    }
  }
  dt0.costs[NODE][NODE] = 0;

  dt0.costs[NODE][1] = 1;
  dt0.costs[NODE][2] = 3;
  dt0.costs[NODE][3] = 7;

  dt0.costs[1][1] = 1;
  dt0.costs[2][2] = 3;
  dt0.costs[3][3] = 7;
  //-------------------------------------------------

  minSyncPkt->mincost[0] = 0;
  minSyncPkt->mincost[1] = 1;
  minSyncPkt->mincost[2] = 3;
  minSyncPkt->mincost[3] = 7;

  printdt0(&dt0);
  sendAll0();
}

void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  int sid = rcvdpkt->sourceid; //source id = node via which other node can be reached

  int dist2node;
  short changed = 0;

  printf("----------------------------------------------\n");
  printf("Change pkt received at Node %d at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");
  
  
  if (rcvdpkt->destid == 0) { //discard if not directed to it

    for (int i=0; i<4; i++) {
      if ((i != NODE) && (i != sid)) {
        
        dist2node = rcvdpkt-> mincost[i] + dt0.costs[sid][sid];
        dt0.costs[i][sid] = (dist2node > rcvdpkt-> mincost[i])?rcvdpkt-> mincost[i]:dist2node;

        if (dt0.costs[i][sid] > dist2node) {
          changed = 1;
          minSyncPkt->mincost[i] = dist2node;
        }  
      }
    }

    if (changed == 1) {
      for (int i=0; i<4; i++) {
        if ((i != NODE) && (i != sid)) {
          sendNeighbour0(i);
        }
      }
    }
  }
  printdt0(&dt0);  
}

void linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in dvr.c from 0 to 1 */
	
{
}


void sendNeighbour0(int dest) {
  minSyncPkt->destid = dest;
  tolayer2(*minSyncPkt);  
}

void sendAll0() {
  sendNeighbour0(1);
  sendNeighbour0(2);
  sendNeighbour0(3);
}