/* data structures used by routers
 */
struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
};

struct distance_table 
{
  int costs[4][4];
};

/* variables defined in dvr.c */
extern int TRACE;
extern int YES;
extern int NO;
extern float clocktime;
