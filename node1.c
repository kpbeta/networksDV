#include <stdio.h>
#include "dvr.h"
#include <stdlib.h>

// Macros
#define INF 999
#define NODE 1

struct distance_table dt1;

struct rtpkt *minSyncPkt; 

// Prototypes
void sendNeighbour1(int dest);
void sendAll1();

void printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

/* students to write the following two routines, and maybe some others */
void rtinit1() 
{
  printf("----------------------------------------------\n");
  printf("Node %d initialized at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");
    
  minSyncPkt = (struct rtpkt *)malloc( sizeof(struct rtpkt) );
  minSyncPkt->sourceid = 1;

  // Initialize the costs
  //-------------------------------------------------
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
        dt1.costs[i][j] = INF;
        dt1.costs[j][i] = INF;
    }
  }
  dt1.costs[NODE][NODE] = 0;

  dt1.costs[NODE][0] = 1;
  dt1.costs[NODE][2] = 1;

  dt1.costs[0][0] = 1;
  dt1.costs[2][2] = 1;
  //-------------------------------------------------

  minSyncPkt->mincost[0] = 1;
  minSyncPkt->mincost[1] = 0;
  minSyncPkt->mincost[2] = 3;
  minSyncPkt->mincost[3] = INF;

  printdt1(&dt1);
  sendAll1();
}

void rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  int sid = rcvdpkt->sourceid;

  int dist2node;
  short changed = 0;

  printf("----------------------------------------------\n");
  printf("Change pkt received at Node %d at time %f \n", NODE, clocktime);
  printf("----------------------------------------------\n");
  
  
  if (rcvdpkt->destid == 1) { //discard if not directed to it

    for (int i=0; i<4; i++) {
      if ((i != NODE) && (i != sid)) {
        
        dist2node = rcvdpkt-> mincost[i] + dt1.costs[sid][sid];
         printf("dist2node to node %d from %d is %d\n", i, sid, dist2node);

        if (dt1.costs[i][sid] > dist2node) {
          changed = 1;
          dt1.costs[i][sid] = dist2node;
          minSyncPkt->mincost[i] = dist2node;
        }
      }
    }


    if (changed == 1) {
      for (int i=0; i<4; i++) {
        if ((i != NODE) && (i != sid)) {
          sendNeighbour1(i);   
        }
      }
    }
  }
  printdt1(&dt1);  
}

void linkhandler1(linkid, newcost)   
int linkid, newcost;   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in dvr.c from 0 to 1 */
	
{
}

void sendNeighbour1(int dest) {
  minSyncPkt->destid = dest;
  tolayer2(*minSyncPkt);  
}


void sendAll1() {
  sendNeighbour1(0); 
  sendNeighbour1(2);
}