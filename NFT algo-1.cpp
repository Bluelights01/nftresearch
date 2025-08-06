#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<graphics.h>
#include<stdlib.h>
#include<dos.h>
#include<time.h>

#define ROBOSIZE 9
#define EMPTY 0
#define FILLED 1
#define PARTIAL 2
#define BNDX1 00
#define BNDY1 00
#define BNDX2 480
#define BNDY2 640
time_t first,second;
float distr=0;
/* Declaring Structure of Quadtree's Node*/
typedef struct node
{
  int x1;
  int y1;
  int x2;
  int y2;
  int finished;
  node *child1;
  node*child2;
  node*child3;
  node*child4;
  node*parent;
  short status;
  int visited;
} node;
/*** End of Declaration of Quadtree's Node Structure ***/

/*** Declaration of Grid's Node Structure ***/
struct gnode
{
	int x1;
	int y1;
	int x2;
	int y2;
	int status;
	int visited;
};
/*** End of Declaration of Grid's Node Structure ***/

/*** Standard Table For Adjacency Side ***/
  int ADJ[5][5]={
   {1,1,0,0},
   {0,1,0,1},
   {0,0,1,1},
   {1,0,1,0},
  };
/*** End of Table For Adjacency Side ***/





/*** Standard Table of Reflection Check for Node ***/
  int REFLECT[5][5]={
			 {2,3,0,1},
			 {1,0,3,2},
			 {2,3,0,1},
			 {1,0,3,2},
			};
/*** End of Standard Table of Reflection Check for Node ***/

/*** Declaration of Global Variable start ***/
   int start_x;
   int start_y;
   int goal_x;
   int goal_y;

   /*** Variables for Quadtree ***/
   node *current_node=NULL;
   node *goal_node=NULL;
   node *neighbours[50];
   int neighbour_count=0;
   node*path[100];

   /*** Variables for Grid Matrix ***/
   gnode *mat[50][50];
   gnode *grid_path[500];
   int grid_path_count=0;
   int path_count=0;
   int mat_row=0;
   int mat_col=0;
   gnode *cur_node=NULL;
   gnode *gol_node=NULL;
   int cur_row=0;
   int cur_col=0;
   int goal_row=0;
   int goal_col=0;
/*** End of Declaration of Global Variables ***/

/*** Declaration of setup class for Create World Map ***/
   class setup
   {
	int bx1;
	int bx2;
	int by1;
	int by2;
	int obs1[10];
	int obs2[10];
	int obs3[10];
	int obs4[10];

	public:
	setup(void);
	void draw_border(void);
	void draw_obstacles(void);
	};
/*** End of Declaration of setup class for create World Map ***/

/*** constructor of setup class for initialization of variable ***/
	void setup::setup(void)
	{
	  bx1= BNDX1;
	  bx2= BNDX2;
	  by1= BNDY1;
	  by2= BNDY2;
	}
/*** End of constructor of setup class for initialization of variable ***/

/*** Declaration of draw_border() ***/
	void setup::draw_border(void)
	{
		setcolor(WHITE);
		setlinestyle(SOLID_LINE,0,0);
		rectangle(bx1,by1,bx2,by2);
	}
/*** End Declaration of draw_border() for create border for World Map ***/

/*** Declaration of draw_obstacles() ***/
	void setup::draw_obstacles(void)
	{
		setcolor(WHITE);
		setlinestyle(SOLID_LINE,0,0);
		rectangle(80,80,120,120);
		rectangle(200,120,240,160);
		rectangle(240,160,320,240);
		rectangle(160,240,240,320);
		floodfill(100,100,WHITE);
		floodfill(220,140,WHITE);
		floodfill(300,200,WHITE);
		floodfill(200,300,WHITE);
	}
/*** End of Declaration of draw_obstacles() ***/



/*** Declaration of class quadtree for create and manage Quadtree ***/
  class quadtree
  {
		node *root;

		public:
		quadtree(void);
		void develope_tree(node* n);
		node *return_root(void){return root;}
		void set_current_node(node*n,int x,int y);
		void set_goal_node(node*n,int x,int y);
		node *find_neighbour(node*p,int direction);
		void cleanup(node* n);
   };
/*** End of Declaration of class quadtree ***/

/*** Declaration of constructor of quadtree class for initialzing the Node ***/
	quadtree::quadtree()
	{
	  root=new node;
	  root->x1=80;
	  root->y1=80;
	  root->x2=400;
	  root->y2=400;
	  root->child1=NULL;
	  root->child2=NULL;
	  root->child3=NULL;
	  root->child4=NULL;



	  root->parent=NULL;
	  root->finished=0;
	}

/*** End of Declaration of Constructor ***/

/*** Declaration of develope_tree() for develope the World Map into Quadtree ***/
  void quadtree::develope_tree(node* n)
  {
	  int i,j;
	  int cnt=0,cnt1=0,cnt2=0;
	  node *temp=n;
	  node *ptr1 = new node;
	  ptr1->parent=n;
	  ptr1->x1=temp->x1;
	  ptr1->y1=temp->y1;
	  ptr1->x2=temp->x1+(temp->x2-temp->x1)/2;
	  ptr1->y2=temp->y1+(temp->y2-temp->y1)/2;

	  for(i=ptr1->y1+2;i<ptr1->y2-2;i++)
	  for(j=ptr1->x1+2;j<ptr1->x2-2;j++)
	  {
		cnt++;
		if (getpixel(j,i)==BLACK)cnt1++;
		if (getpixel(j,i)==WHITE)cnt2++;
	  }
	  if ((cnt1==0)||(cnt2==0))
			ptr1->finished=1;
	  else
			ptr1->finished=0;

	  if ((ptr1->x2-ptr1->x1)<=ROBOSIZE)
			ptr1->finished=1;

	  if ((cnt1==0)&&(cnt2!=0))
			ptr1->status=FILLED;

	  if ((cnt1!=0)&&(cnt2==0))
			ptr1->status=EMPTY;

	  if ((cnt1!=0)&&(cnt2!=0))
			ptr1->status=PARTIAL;

		n->child1=ptr1;
	     //	cout<<cnt1<<","<<cnt2<<"for";
	       //	cout<<"1 passed,";
	       //	getch();

		if (ptr1->finished==0)
			develope_tree(n->child1);
		else
		{
			n->child1->child1=NULL;
			n->child1->child2=NULL;
			n->child1->child3=NULL;
			n->child1->child4=NULL;
		}
		temp=n;
		cnt=0;cnt1=0;cnt2=0;
		node*ptr2=new node;
		ptr2->parent=n;
		ptr2->x1=temp->x1+(temp->x2-temp->x1)/2;
		ptr2->y1=temp->y1;
		ptr2->x2=temp->x2;
		ptr2->y2=temp->y1+(temp->y2-temp->y1)/2;

		for(i=ptr2->y1+2;i<ptr2->y2-2;i++)
		for(j=ptr2->x1+2;j<ptr2->x2-2;j++)
		{
			cnt++;
			if (getpixel(j,i)==BLACK)cnt1++;
			if (getpixel(j,i)==WHITE)cnt2++;
		}

		if ((cnt1==0)||(cnt2==0))
			ptr2->finished=1;
		else
			ptr2->finished=0;

		if ((ptr2->x2-ptr2->x1)<=ROBOSIZE)
			ptr2->finished=1;

		if ((cnt1==0)&&(cnt2!=0))
			ptr2->status=FILLED;

		if ((cnt1!=0)&&(cnt2==0))
			ptr2->status=EMPTY;

		if ((cnt1!=0)&&(cnt2!=0))
			ptr2->status=PARTIAL;

		n->child2=ptr2;
	      //	cout<<cnt1<<","<<cnt2<<"for";
	       //	cout<<"2 passed,";
	     //	getch();

		if (ptr2->finished==0)
			develope_tree(n->child2);
		else
		{
			n->child2->child1=NULL;
			n->child2->child2=NULL;
			n->child2->child3=NULL;
			n->child2->child4=NULL;
		}
		cnt=0;cnt1=0;cnt2=0;
		temp=n;
		node*ptr3=new node;
		ptr3->parent=n;
		ptr3->x1=temp->x1;
		ptr3->y1=temp->y1+(temp->y2-temp->y1)/2;
		ptr3->x2=temp->x1+(temp->x2-temp->x1)/2;
		ptr3->y2=temp->y2;
	      //	cout<<ptr3->x1<<","<<ptr3->y1<<","<<ptr3->x2<<","<<ptr3->y2;
//vijay cout<<ptr4->x1<<","<<ptr4->y1<<","<<ptr4->x2<<","<<ptr4->y2;

		for(i=ptr3->y1+2;i<ptr3->y2-2;i++)
		for(j=ptr3->x1+2;j<ptr3->x2-2;j++)
		{
			cnt++;
			if (getpixel(j,i)==BLACK)
				cnt1++;

			if (getpixel(j,i)==WHITE)
				cnt2++;
		}

		if ((cnt1==0)||(cnt2==0))
			ptr3->finished=1;
		else
		  ptr3->finished=0;

		if ((ptr3->x2-ptr3->x1)<=ROBOSIZE)
			ptr3->finished=1;

		if ((cnt1==0)&&(cnt2!=0))
			ptr3->status=FILLED;

		if ((cnt1!=0)&&(cnt2==0))
			ptr3->status=EMPTY;

		if ((cnt1!=0)&&(cnt2!=0))
			ptr3->status=PARTIAL;

		n->child3=ptr3;
	    //	cout<<cnt1<<","<<cnt2<<"for";
	    //	cout<<"3 passed,";
	     //	getch();

		if (ptr3->finished==0)
			develope_tree(n->child3);
		else
		{
			n->child3->child1=NULL;
			n->child3->child2=NULL;
			n->child3->child3=NULL;
			n->child3->child4=NULL;
		}

		cnt=0;cnt2=0;cnt1=0;
		temp=n;
		node *ptr4=new node;
		ptr4->parent=n;
		ptr4->x1=temp->x1+(temp->x2-temp->x1)/2;
		ptr4->y1=temp->y1+(temp->y2-temp->y1)/2;
		ptr4->x2=temp->x2;
		ptr4->y2=temp->y2;

		for(i=ptr4->y1+2;i<ptr4->y2-2;i++)
		for(j=ptr4->x1+2;j<ptr4->x2-2;j++)
		{
			cnt++;
			if (getpixel(j,i)==BLACK)cnt1++;
			if (getpixel(j,i)==WHITE)cnt2++;
		}

		if ((cnt1==0)||(cnt2==0))
			ptr4->finished=1;
		else
			ptr4->finished=0;

		if ((ptr4->x2-ptr4->x1)<=ROBOSIZE)
			ptr4->finished=1;

		if ((cnt1==0)&&(cnt2!=0))
			ptr4->status=FILLED;

		if ((cnt1!=0)&&(cnt2==0))
			ptr4->status=EMPTY;

		if ((cnt1!=0)&&(cnt2!=0))
			ptr4->status=PARTIAL;

		n->child4=ptr4;
	     //	cout<<cnt1<<","<<cnt2<<"for";
	     //	cout<<"4 passed,";getch();

		if (ptr4->finished==0)
			develope_tree(n->child4);
		else
		{
			n->child4->child1=NULL;
			n->child4->child2=NULL;
			n->child4->child3=NULL;
			n->child4->child4=NULL;
		}
	   return;
   }
/*** End of Declaration of develope_tree() ***/

/*** Declaration of set_current_node() to find the Node where
	 the start location Exist ***/

	void quadtree::set_current_node(node* n,int x,int y)
	{
	   node* t=n;
	   if (t->child1!=NULL)set_current_node(t->child1,x,y);
	   if (t->child2!=NULL)set_current_node(t->child2,x,y);
	   if (t->child3!=NULL)set_current_node(t->child3,x,y);
	   if (t->child4!=NULL)set_current_node(t->child4,x,y);
	   if (t->finished==1)
	   {
		if((t->x1<=x)&&(t->y1<=y)&&(t->x2>=x)&&(t->y2>=y))
			current_node=t;
	   }
	}
/*** End of Declaration of set_current_node() ***/

/*** Declaration of set_goal_node() to find the Node where
	 the goal location Exist ***/
	void quadtree::set_goal_node(node* n,int x,int y)
	{
	   node* t=n;
	   if (t->child1!=NULL)set_goal_node(t->child1,x,y);
	   if (t->child2!=NULL)set_goal_node(t->child2,x,y);
	   if (t->child3!=NULL)set_goal_node(t->child3,x,y);
	   if (t->child4!=NULL)set_goal_node(t->child4,x,y);
	   if (t->finished==1)
	   {
		if((t->x1<=x)&&(t->y1<=y)&&(t->x2>=x)&&(t->y2>=y))
		goal_node=t;}
	   }
/*** End of set_goal_node() ***/

/*** Declaration of cleanup() for free the space acquired by quadtree ***/
	void quadtree::cleanup(node* t)
	{
	  if (t->child1!=NULL)cleanup(t->child1);
	  if (t->child2!=NULL)cleanup(t->child2);
	  if (t->child3!=NULL)cleanup(t->child3);
	  if (t->child4!=NULL)cleanup(t->child4);
	  free(t);
	}
/*** End of Declaration of cleanup() ***/

/*** Declaration of find_neighbour() to find the neighbour of current node ***/
	node *quadtree::find_neighbour(node* p,int direction) 	{
	   node* n;
	   int sontype;
	   if(p==p->parent->child1)sontype=0;
	   if(p==p->parent->child2)sontype=1;
	   if(p==p->parent->child3)sontype=2;
	   if(p==p->parent->child4)sontype=3;

	   if((p->parent!=NULL)&&(ADJ[direction][sontype]))
		n= find_neighbour(p->parent,direction);
	   else
		n=p->parent;

		   if (n->finished==1)return n;
		   int r=REFLECT[direction][sontype];
		   if((n!=NULL)&&(n->status==PARTIAL))
		   {
			   if(r==0)return n->child1;
			   if(r==1)return n->child2;
			   if(r==2)return n->child3;
			   if(r==3)return n->child4;
		   }

		  // else  vijay
			  return n;
	}
/*** End of Declaration of find_neighbour() ***/

/*** Declaration of having_common_side() for finding the common side for the small neighbour ***/
int having_common_side(int x1,int y1,int x2,int y2,int X1,int Y1,int X2,int Y2)
	{
		int xmid,ymid;
		xmid=(x1+x2)/2;
		ymid=y2;
		for(int i=X1;i<X2;i++)
		{
			if((i==xmid)&&(ymid==Y1))
			return 1;
		}
		xmid=x2;
		ymid=(y1+y2)/2;

		for(i=Y1;i<=Y2;i++)
		{
			if((i==ymid)&&(xmid==X1))
			return 1;
		}
		xmid=(x1+x2)/2;
		ymid=y1;

		for(i=X1;i<=X2;i++)
		{
			if((i==xmid)&&(ymid==Y2))
			return 1;
		}
		xmid=x1;
		ymid=(y1+y2)/2;

		for(i=Y1;i<=Y2;i++)
		{
			if((i==ymid)&&(xmid==X2))
			return 1;
		}
		return 0;
	}
/*** End of Declaration of having_common_side() ***/

/*** Declaration of find_small_neighbour() for finding small neighbour  within a neighbour ***/
	void find_small_neighbours(node*n)
	{
		node*t=n;
		if(t->child1!=NULL)find_small_neighbours(t->child1);
		if(t->child2!=NULL)find_small_neighbours(t->child2);
		if(t->child3!=NULL)find_small_neighbours(t->child3);
		if(t->child4!=NULL)find_small_neighbours(t->child4);
		if(t->finished==1)
		{
			if(having_common_side(t->x1,t->y1,t->x2,t->y2,
			current_node->x1,current_node->y1,current_node->x2,
			current_node->y2))
			{
				neighbours[neighbour_count++]=t;
			}
		}
	}
/*** End of Declaration of find_small_neighbour() ***/

/*** Declaration of distance() for calculating the distance between two location ***/
   double distance(int x1,int y1,int x2,int y2)
   {
		double dist=sqrt((double)(x2-x1)*(double)(x2-x1)
		+double(y2-y1)*(double)(y2-y1));
		return dist;
   }
/*** End of Declaration of distance() ***/

/*** Declaration of find_path() for finding a shortest path ***/
void find_path(void)
{
	int i;
	node*t=NULL;
	for(i=0;i<neighbour_count;i++)
	{
		t=neighbours[i];
		if((t->x1==goal_node->x1)&&(t->y1==goal_node->y1)
		&&(t->x2==goal_node->x2)&&(t->y2==goal_node->y2))
		{
			current_node=goal_node;
			return;
		}
	}
	node*tem=NULL;
   //	cout<<endl<<"total neighbours"<<endl;

   //	for(i=0;i<neighbour_count;i++) 	{
     //		tem=neighbours[i];
       //	cout<<tem->x1<<","<<tem->y1<<"and"<<tem->x2 			<<","<<tem->y2<<endl;
	 //	if(tem->status==EMPTY)
	   //	cout<<"status:"<<"empty"<<endl;
      //	}
		node*best_node;
		int midx1=(current_node->x1+current_node->x2)/2;
		int midy1=(current_node->y1+current_node->y2)/2;
		int midx2=0;
		int midy2=0 ;
		int midx3=(goal_node->x1+goal_node->x2)/2;
		int midy3=(goal_node->y1+goal_node->y2)/2;
		double dist1=0.0;
		double dist2=0.0;
		double mindist=10000.0;
	//	cout<<"neighbours="<<neighbour_count;
		for(i=0;i<neighbour_count;i++)
		{
//			cout<<"examining node"<<neighbours[i]->x1 					<<","<<neighbours[i]->y1<<endl;
			if(neighbours[i]->visited!=1)
			{
			    if(neighbours[i]->status==EMPTY)
			{
				midx2=(neighbours[i]->x1+neighbours[i]->x2)/2;
				midy2=(neighbours[i]->y1+neighbours[i]->y2)/2;

				dist1=distance(midx1,midy1,midx2,midy2);
				dist2=distance(midx2,midy2,midx3,midy3);
				if(dist1<=0.0)dist1=0.0;
				if(dist2<=0.0)dist2=0.0;
///			cout<<"examining node"<<neighbours[i]->x1<<","<<neighbours[i]->y1<<endl;
			//	cout<<"dist1="<<dist1<<endl<<
			  //	"dist2="<<dist2;

				if((dist1+dist2)<=mindist)
				{
					mindist=dist2+dist1;
					best_node=neighbours[i];
				}
			}
		}
	 }
       //	 cout<<"with distance"<<mindist;
	 best_node->visited=1;
	 path[path_count++]=best_node;
	 current_node=best_node;
     //	 cout<<endl<<"path node";
    //	 cout<<endl<<best_node->x1<<","<<best_node->y1<<"and";
     //	 cout<<best_node->x2<<","<<best_node->y2;
     //	 getch();
}
/*** End of Declaration of find_path() ***/

/*** Declaration fill_node() for filling white colour inside the obstacle ***/
void fill_node(int x1,int y1,int x2,int y2)
{
	int k;
	for(int j=x1;j<=x2;j++)
	for(k=y1;k<y2;k++)
	{
		putpixel(j,k,LIGHTRED);
	}
}
/*** End of Declaration of fill_node() ***/

/*** Declaration of class grid for grid search method ***/
class grid
{
	int row;
	int col;

 public:
	void develope_matrix();
	void findpath_in_matrix();
};
/*** End of Declaration of class grid ***/

/*** Declaration of develope_matrix() for develop
	 the matrce for uniform grid search method ***/
  void grid::develope_matrix()
  {
		int unit_x=0;
		int unit_y=0;
		int num_col=0;
		int x2=0;
		int y2=0;
		int cnt=0;

		unit_x=(BNDX2-2) - (BNDX1+2);
		unit_y=(BNDY2-2) - (BNDY1+2);
		unit_x=unit_x/40;
		unit_y=unit_y/40;
		x2=BNDX1+unit_x;
		y2=BNDY1+unit_y;
		gnode *ptr=NULL;

for(int y1=BNDY1+2; y1<=BNDY2-2; y1=y1+unit_y, 			y2=y2+unit_y,mat_row++,mat_col=0)
		{
for(int x1=BNDX1+2; x1<=BNDX2-2; 						x1=x1+unit_x,x2=x2+unit_x,mat_col++)
			{
				for(int i=y1; i<=y2; i++)
				{
					for(int j=x1; j<=x2; j++)
					{
						if( getpixel(j,i)==WHITE)
						cnt++;
					}
				}
				  ptr=new gnode;
				  ptr->x1=x1;
				  ptr->x2=x2;
				  ptr->y1=y1;
				  ptr->y2=y2;
				  ptr->visited=0;

				if(cnt==0)
				{
				   ptr->status=0;
				}
				else
				{
				  ptr->status=1;
				}
				cnt=0;
				mat[mat_row][mat_col]=ptr;
				num_col=mat_col;

			}
			x1=BNDX1;
			x2=BNDX1+unit_x;
		 }
		 mat_col=num_col+1;
}
/*** End of Declaration of develope_matrix() ***/

/*** Declaration of findpath_in_matrix() for finding the shortest path using grid search method ***/
void grid::findpath_in_matrix()
{
	gnode *t=NULL;
	gnode *cur_ptr=NULL;
	double min_dist=1000.0;
	double dist1=0.0,dist2=0.0;

	for(int i=0;i<mat_row;i++)
	for(int j=0;j<mat_col;j++)
	{
		t=mat[i][j];
if((t->x1<=start_x)&&(t->y1<=start_y)&&(t->x2>=start_x)&& (t->y2>=start_y))
		{
			 cur_node=t;
			 cur_row=i;
			 cur_col=j;
			 cur_node->visited=1;
		}

	if((t->x1<=goal_x)&&(t->y1<=goal_y)&&(t->x2>=goal_x)&&(t->y2>=goal_y))
	{
			gol_node=t;
			goal_row=i;
			goal_col=j;
	 }
	}
	for(int ctr=0;ctr<100;ctr++)
	   grid_path[ctr]=NULL;
 do
 {
	int best_neigh[5];
	int midx1=0;
	int midy1=0;
	int midx2=0;
	int midy2=0;
	int midx3=0;
	int midy3=0;
	int prev_col=0;
	int prev_row=0;
	min_dist=1000.0;
	dist1=0.0;
	dist2=0.0;
	cur_ptr=mat[cur_row][cur_col];
	for(int i=0;i<6;i++)
	best_neigh[i]=0;

	prev_row=cur_row;
	prev_col=cur_col;

	midx1=(mat[cur_row][cur_col]->x1+mat[cur_row][cur_col]->x2)/2;
	midy1=(mat[cur_row][cur_col]->y1+mat[cur_row][cur_col]->y2)/2;
	midx3=(gol_node->x1+gol_node->x2)/2;
	midy3=(gol_node->y1+gol_node->y2)/2;

	if(mat[cur_row-1][cur_col]->visited!=1)
	if(mat[cur_row-1][cur_col]->status==0)
	{
int midx2=(mat[cur_row-1][cur_col]->x1+mat[cur_row-1][cur_col]-> x2)/2;
int midy2=(mat[cur_row-1][cur_col]->y1+mat[cur_row-1][cur_col]-> y2)/2;
		dist1= sqrt((double)(midx2-midx1)*(double)(midx2-						midx1)+double(midy2-midy1)*(double)(midy2-midy1));
dist2= sqrt((double)(midx3-midx2)*(double)(midx3-				midx2)+double(midy3-midy2)*(double)(midy3-midy2));

		if((dist1+dist2)<=min_dist)
		{
			min_dist=dist2+dist1;
			cur_node=mat[cur_row-1][cur_col];
			best_neigh[0]=cur_row-1;
			best_neigh[1]=cur_col;
			grid_path[grid_path_count]=mat[cur_row-1][cur_col];
		}
	}
	dist1=0.0;
	dist2=0.0;


	if(mat[cur_row][cur_col+1]->status==0)
	if(mat[cur_row][cur_col+1]->visited!=1)
	{
		int midx2=(mat[cur_row][cur_col+1]->x1 							+mat[cur_row][cur_col+1]->x2)/2;
int midy2=(mat[cur_row][cur_col+1]->y1+ 					mat[cur_row][cur_col+1]-> y2)/2;
		dist1= sqrt((double)(midx2-midx1)*(double)(midx2-						midx1)+double(midy2-midy1)*(double)(midy2-midy1));
dist2= sqrt((double)(midx3-midx2)*(double)(midx3-				midx2)+double(midy3-midy2)*(double)(midy3-midy2));

		if((dist1+dist2)<=min_dist)
		{
			min_dist=dist2+dist1;
			cur_node=mat[cur_row][cur_col+1];
			best_neigh[0]=cur_row;
			best_neigh[1]=cur_col+1;
			grid_path[grid_path_count]=mat[cur_row][cur_col+1];
		   }
		}
		   dist1=0.0;
		   dist2=0.0;

		if(mat[cur_row+1][cur_col]->status==0)
		if(mat[cur_row+1][cur_col]->visited!=1)
		{
			node *best_node;
int midx2=(mat[cur_row+1][cur_col]->x1 					+mat[cur_row+1][cur_col]->x2)/2;
int midy2=(mat[cur_row+1][cur_col]->y1+ 					mat[cur_row+1][cur_col]->y2)/2;
			int midx3=(gol_node->x1+gol_node->x2)/2;
			int midy3=(gol_node->y1+gol_node->y2)/2;

dist1= sqrt((double)(midx2-midx1)*(double)(midx2-				midx1)+double(midy2-midy1)*(double)(midy2-midy1));
			dist2= sqrt((double)(midx3-midx2)*(double)(midx3-						midx2)+double(midy3-midy2)*(double)(midy3-midy2));

		   if((dist1+dist2)<=min_dist)
		   {
			min_dist=dist2+dist1;
			cur_node=mat[cur_row+1][cur_col];
			best_neigh[0]=cur_row+1;
			best_neigh[1]=cur_col;
			grid_path[grid_path_count]=mat[cur_row+1][cur_col];
		   }
		}
		   dist1=0.0;
		   dist2=0.0;

		if(mat[cur_row][cur_col-1]->status==0)
		if(mat[cur_row][cur_col-1]->visited!=1)
		{
			node *best_node;
int midx2=(mat[cur_row][cur_col-1]->x1+ 					mat[cur_row][cur_col-1]->x2)/2;
int midy2=(mat[cur_row][cur_col-1]->y1+ 					mat[cur_row][cur_col-1]->y2)/2;
			int midx3=(gol_node->x1+gol_node->x2)/2;
			int midy3=(gol_node->y1+gol_node->y2)/2;

			dist1= sqrt((double)(midx2-midx1)*(double)(midx2-						midx1)+double(midy2-midy1)*(double)(midy2-midy1));
dist2= sqrt((double)(midx3-midx2)*(double)(midx3-				midx2)+double(midy3-midy2)*(double)(midy3-midy2));

		   if((dist1+dist2)<=min_dist)
		   {
			min_dist=dist2+dist1;
			cur_node=mat[cur_row][cur_col-1];
			best_neigh[0]=cur_row;
			best_neigh[1]=cur_col-1;
			grid_path[grid_path_count]=mat[cur_row][cur_col-1];
		   }
		}
		   dist1=0.0;
		   dist2=0.0;

		if(mat[cur_row-1][cur_col-1]->status==0)
		if(mat[cur_row-1][cur_col-1]->visited!=1)
		{
			node *best_node;
			int midx2=(mat[cur_row-1][cur_col-1]->x1+ 							mat[cur_row-1][cur_col-1]->x2)/2;
			int midy2=(mat[cur_row-1][cur_col-1]->y1+ 							mat[cur_row-1][cur_col-1]->y2)/2;
			int midx3=(gol_node->x1+gol_node->x2)/2;
			int midy3=(gol_node->y1+gol_node->y2)/2;

dist1= sqrt((double)(midx2-midx1)*(double)(midx2-				midx1)+double(midy2-midy1)*(double)(midy2-midy1));
			dist2= sqrt((double)(midx3-midx2)*(double)(midx3-						midx2)+double(midy3-midy2)*(double)(midy3-midy2));

		   if((dist1+dist2)<=min_dist)
		   {
			min_dist=dist2+dist1;
			cur_node=mat[cur_row-1][cur_col-1];
			best_neigh[0]=cur_row-1;
			best_neigh[1]=cur_col-1;
			grid_path[grid_path_count]=mat[cur_row-1][cur_col-1];
		   }
	}
		 dist1=0.0;
		 dist2=0.0;

		if(mat[cur_row-1][cur_col+1]->status==0)
		if(mat[cur_row-1][cur_col+1]->visited!=1)
		{
			node *best_node;
	int midx2=(mat[cur_row-1][cur_col+1]->x1+ 						mat[cur_row-1][cur_col+1]->x2)/2;
int midy2=(mat[cur_row-1][cur_col+1]->y1+ 					mat[cur_row-1][cur_col+1]->y2)/2;
			int midx3=(gol_node->x1+gol_node->x2)/2;
			int midy3=(gol_node->y1+gol_node->y2)/2;

dist1= sqrt((double)(midx2-midx1)*(double)(midx2-				midx1)+double(midy2-midy1)*(double)(midy2-midy1));
			dist2= sqrt((double)(midx3-midx2)*(double)(midx3-						midx2)+double(midy3-midy2)*(double)(midy3-midy2));

		   if((dist1+dist2)<=min_dist)
		   {
			min_dist=dist2+dist1;
			cur_node=mat[cur_row-1][cur_col+1];
			best_neigh[0]=cur_row-1;
			best_neigh[1]=cur_col+1;
			grid_path[grid_path_count]=mat[cur_row-1][cur_col+1];
		   }
	}

	 dist1=0.0;
	 dist2=0.0;

	if(mat[cur_row+1][cur_col+1]->status==0)
	if(mat[cur_row+1][cur_col+1]->visited!=1)
	{
		node *best_node;
		int midx2=(mat[cur_row+1][cur_col+1]->x1+ 							mat[cur_row+1][cur_col+1]->x2)/2;
int midy2=(mat[cur_row+1][cur_col+1]->y1+ 				mat[cur_row+1][cur_col+1]->y2)/2;
		int midx3=(gol_node->x1+gol_node->x2)/2;
		int midy3=(gol_node->y1+gol_node->y2)/2;

		dist1= sqrt((double)(midx2-midx1)*(double)(midx2-midx1)+ 				double(midy2-midy1)*(double)(midy2-midy1));
dist2= sqrt((double)(midx3-midx2)*(double)(midx3-midx2)+ 		double(midy3-midy2)*(double)(midy3-midy2));

		if((dist1+dist2)<=min_dist)
		{
			min_dist=dist2+dist1;
			cur_node=mat[cur_row+1][cur_col+1];
			best_neigh[0]=cur_row+1;
			best_neigh[1]=cur_col+1;
grid_path[grid_path_count]= mat[cur_row+1][cur_col+1];

		}

	}

	dist1=0.0;
	dist2=0.0;


if(mat[cur_row+1][cur_col-1]->status==0)
	if(mat[cur_row+1][cur_col-1]->visited!=1)
	{
		node *best_node;
		int midx2=(mat[cur_row+1][cur_col-1]->x1+ 							mat[cur_row+1][cur_col-1]->x2)/2;
		int midy2=(mat[cur_row+1][cur_col-1]->y1+ 							mat[cur_row+1][cur_col-1]->y2)/2;
		int midx3=(gol_node->x1+gol_node->x2)/2;
		int midy3=(gol_node->y1+gol_node->y2)/2;
	dist1= sqrt((double)(midx2-midx1)*(double)(midx2-				midx1)+double(midy2-midy1)*(double)(midy2-midy1));
		dist2= sqrt((double)(midx3-midx2)*(double)(midx3-						midx2)+double(midy3-midy2)*(double)(midy3-midy2));

		if((dist1+dist2)<=min_dist)
		{
			min_dist=dist2+dist1;
			cur_node=mat[cur_row+1][cur_col-1];
			best_neigh[0]=cur_row+1;
			best_neigh[1]=cur_col-1;
			grid_path[grid_path_count]= mat[cur_row+1][cur_col-1];
		}
	}
		  grid_path_count++;
		  cur_row=best_neigh[0];
		  cur_col=best_neigh[1];
		  mat[cur_row][cur_col]->visited=1;
		  if(best_neigh[0]==0)
		  {
			cur_row=prev_row;
			cur_col=prev_col;
		  }
	 }
	 while(cur_node!=gol_node);
}
/*** End of Declaration of find_path_in_matrix() ***/


/*** Declaration of accept_location() for accept start and goal
	 locatin as input and validate it against Map Location ***/
void accept_location(int *goal_x, int *goal_y, int *start_x, int *start_y)
{
  clrscr();
  do{
	   clrscr();
	   cout<<" set goal x-location:";
	   cin>>*goal_x;
	   cout<<" set goal y-location:";
	   cin>>*goal_y;

	if((*goal_x>BNDX2) || (*goal_x<BNDX1))
	{
		cout<<"goal_x must be in between "<<BNDX1<<" and"<<BNDX2<<endl;
		getch();
	}

	if((*goal_y>BNDY2) || (*goal_y<BNDY1))
	{
		cout<<"goal_y must be in between "<<BNDY1<<" and"<<BNDY2<<endl;
		getch();
	}

	 }while((*goal_x>BNDX2) || (*goal_x<BNDX1) );

	 do{
		  clrscr();
		  cout<<" set starting x-location:";
		  cin>>*start_x;
		  cout<<" set starting y-location:";
		  cin>>*start_y;

		if((*start_x<BNDX1) || (*start_x>BNDX2))
		{
			cout<<"start_x must be in between "<<BNDX1<<" and"<<BNDX2<<endl;
			getch();
		}

		if((*start_y<BNDY1) || (*start_y>BNDY2))
		{
			cout<<"start_y must be in between "<<BNDY1<<" and"<<BNDY2<<endl;
			getch();
		}

	 }while((*start_x<BNDX1) || (*start_x>BNDX2));
}
/*** End of declaration of accept_location() ***/

/*** Declaration of check_start_location_with_obstacle() to
	 check whether the start location is inside any obstacles ***/
	int check_start_location_with_obstacle(int *goal_x,int *goal_y)
	{
		if(getpixel(*goal_x,*goal_y)==WHITE)
		return(1);
	}
/*** End of declaration of check_start_location_with_obstacle() ***/

/*** Declaration of check_goal_location_with_obstacle() to
	 check whether the goal location is inside any obstacles ***/
	int check_goal_location_with_obstacle(int *start_x,int *start_y)
	{
		if(getpixel(*start_x,*start_y)==WHITE)
		return(1);
	}
/*** End of declaration of check_goal_location_with_obstacle() ***/

int check_path(node *l,node *m)
{

///////////////
int a=(l->x1+l->x2)/2,b=(l->y1+l->y2)/2,c=(m->x1+m->x2)/2,d=(m->y1+m->y2)/2;///

float x,y,p,x1,y1,x2,y2;
float dx,dy;
float slope=0;
x1=a;
y1=b;
x=a;
y=b;
x2=c;
y2=d;
setcolor(YELLOW);
circle(x1,y1,5);
circle(x2,y2,5);
setcolor(WHITE);

    dx = (x2 - x1);
    dy = (y2 - y1);
if(dx!=0)
{
slope=fabs(dy/dx);
}
if(slope<=1)
{
	if(x1<x2&&y1<y2)
	{
	 while(x<=x2&&y<y2)
	    {
		x=x++;
		y=y+slope;
		 if(getpixel(x,y)==WHITE) return 0;
  //	      putpixel(x,y,RED);
	   }

	}
	else if(x1<x2&&y1>y2)
	{
	 while(x <= x2&&y>y2)
	    {
		x=x++;
		y=y-slope;
		 if(getpixel(x,y)==WHITE) return 0;
    //	      putpixel(x,y,RED);
	   }
	}
	else if(x1>x2&&y1<y2)
	{
	 while(x>=x2&&y<y2)
	    {
		x=x--;
		y=y+slope;
		 if(getpixel(x,y)==WHITE) return 0;
      //	      putpixel(x,y,RED);
	   }
	}
	else if(x>x2&&y1>y2)
	{
	 while(x>=x2&&y>y2)
	    {
		x=x--;
		y=y-slope;
		 if(getpixel(x,y)==WHITE) return 0;
	//      putpixel(x,y,RED);
	   }
	}
}
else
{
	if(x1<x2&&y1<y2)
	{
		while(x<=x2)
	    {
		x=x+1/slope;
		y++;
		if(getpixel(x,y)==WHITE) return 0;
	  //    putpixel(x,y,RED);
	   }
	}
	else if(x1<x2&&y1>y2)
	{
	while(x<=x2&&y>y2)
	    {

		x=x+1/slope;
		y--;
		if(getpixel(x,y)==WHITE) return 0;
	    //  putpixel(x,y,RED);
	   }
	}
	else if(x1>x2&&y1<y2)
	{
		while(x>=x2&&y<y2)
		    {
			x=x-1/slope;
			y=y++;
			if(getpixel(x,y)==WHITE) return 0;
	      //	      putpixel(x,y,RED);

		     }
	}
	else if(x1>x2&&y1>y2)
	{
	while(x>=x2&&y>y2)
	    {
		x=x-1/slope;
		y=y--;
		 if(getpixel(x,y)==WHITE) return 0;
	    //  putpixel(x,y,RED);

	   }
	}

}

///////////////////
return 1;
}
void draw_line(node *a,node *b)
{

int x1,x2,y1,y2;
x1=(a->x1+a->x2)/2;
y1=(a->y1+a->y2)/2;
x2=(b->x1+b->x2)/2;
y2=(b->y1+b->y2)/2;
setcolor(YELLOW);
line(x1,y1,x2,y2);
}

/*** Declaration of main() Start ***/
 void main()
 {
 int gd=DETECT,gm;
 initgraph(&gd,&gm,"C:\TURBOC3\BGI");
	int driver,mode,temp_color;
	int check_goal_obst=0;
	int check_start_obst=0;

	setup s;
	 do{
		 if(check_goal_obst==1)
		 {
			cout<<"start_x and start_y is inside obstacle, give a valid 					input"<<endl;
			getch();
		 }

		 if(check_start_obst==1)
		 {
cout<<"goal_x and goal_y is inside obstacle, give a valid input"<<endl;
			getch();
		 }

		 accept_location(&goal_x, &goal_y, &start_x, &start_y);
		 driver=DETECT;
		 initgraph(&driver,&mode,"C:\TURBOC3\BGI");
		 temp_color=getcolor();
		 setcolor(YELLOW);
		 circle(goal_x,goal_y,5);
		 outtextxy(goal_x+7,goal_y,"Goal Location");
		 setcolor(YELLOW);
		 circle(start_x,start_y,5);
		 outtextxy(start_x+7,start_y,"Start Location");
		 setcolor(temp_color);
		 s.draw_border();
		 s.draw_obstacles();
	check_goal_obst=check_goal_location_with_obstacle( &goal_x,&goal_y);

	check_start_obst=check_start_location_with_obstacle(&start_x,&start_y);
	closegraph();

	}while((check_goal_obst==1)|| (check_start_obst==1));

	int choice=0;
	clrscr();
	gotoxy(25,5);
	cout<<"1. Quadtree Search Method"<<endl;
	gotoxy(25,7);
	cout<<"2. Grid Search Method"<<endl;
	gotoxy(25,9);
	cout<<"3. Exit"<<endl<<endl;
	gotoxy(28,12);
	cout<<"   Enter your choice (1-3) :";
	cin>>choice;

	if(choice==1)
	{         first=time(NULL);
		 driver=DETECT;
		 initgraph(&driver,&mode,"C:\TURBOC3\BGI");
		 temp_color=getcolor();
		 setcolor(YELLOW);
		 circle(goal_x,goal_y,5);
		 outtextxy(goal_x+7,goal_y,"Goal Location");
		 setcolor(YELLOW);
		 circle(start_x,start_y,5);
		 outtextxy(start_x+7,start_y,"Start Location");
		 setcolor(temp_color);
		 s.draw_border();
		 s.draw_obstacles();
		 quadtree q;
		 q.develope_tree(q.return_root());
	     //	 getch();
		 closegraph();

		 q.set_current_node(q.return_root(),start_x,start_y);
		 path[path_count++]=current_node;
		 current_node->visited=1;
		 q.set_goal_node(q.return_root(),goal_x,goal_y);

	       //	 cout<<"current_node="<<current_node->x1<<","<<current_node->y1;
	       //	 cout<<"and"<<current_node->x2<<","<<current_node->y2<<endl;
	    //	 getch();
		 path_count=0;
		 for(int j=0;j<100;j++)
		 path[j]=NULL;
		 node*nn=NULL;
		 node*en=NULL;
		 node*sn=NULL;
		 node*wn=NULL;
		 current_node->visited=1;

		 do
		 {
			neighbour_count=0;

			for(j=0;j<50;j++)
			neighbours[j]=NULL;

			nn=q.find_neighbour(current_node,0);
			en=q.find_neighbour(current_node,1);
			sn=q.find_neighbour(current_node,2);
			wn=q.find_neighbour(current_node,3);

		if(! ( (nn->x1>=BNDX1-5)&&(nn->y1>=BNDY1-5) && (nn->x2<=BNDX2+5)&&(nn->y2<=BNDY2+5)) )nn=NULL;

		if(!((en->x1>=BNDX1-5)&&(en->y1>=BNDY1-5)&&(en->x2<=BNDX2+5)&&(en->y2<=BNDY2+5)))
		en=NULL;

if(!((sn->x1>=BNDX1-5)&&(sn->y1>=BNDY1-5)&&(sn->x2<=BNDX2+5)&&(sn->y2<=BNDY2+5)))
		sn=NULL;

if(!((wn->x1>=BNDX1-5)&&(wn->y1>=BNDY1-5)&&(wn-> x2<=BNDX2+5)  &&(wn->y2<=BNDY2+5)))
		wn=NULL;

	   //	if(nn!=NULL) cout<<nn->x1<<","<<nn->y1<<"and "<<nn->x2<<","<<nn->y2<<endl;
	    //	if(en!=NULL)cout<<en->x1<<","<<en->y1<<"and "<<en->x2<<","<<en->y2<<endl;

//if(sn!=NULL) cout<<sn->x1<<","<<sn->y1<<"and "<<sn->x2<<","<<sn->y2<<endl;
//if(wn!=NULL) cout<<wn->x1<<","<<wn->y1<<"and "<<wn-> x2 <<","<<wn->y2<<endl;

		if(nn!=NULL)
			if(nn->child1==NULL)
				neighbours[neighbour_count++]=nn;

		if(en!=NULL)
			if(en->child1==NULL)
				neighbours[neighbour_count++]=en;

		if(sn!=NULL)
			if(sn->child1==NULL)
				neighbours[neighbour_count++]=sn;

		if(wn!=NULL)
			if(wn->child1==NULL)
				neighbours[neighbour_count++]=wn;

		 if(nn!=NULL)
			if(nn->child1!=NULL)
				find_small_neighbours(nn);

		 if(en!=NULL)
			if(en->child1!=NULL)
				find_small_neighbours(en);

		 if(sn!=NULL)
			if(sn->child1!=NULL)
				find_small_neighbours(sn);

		 if(wn!=NULL)
			if(wn->child1!=NULL)
				find_small_neighbours(wn);
		  /***   displaying all neighbours ***/
		 node*tem;
  //		 cout<<endl<<"total neighbours"<<endl;
	      //	 for(int i=0;i<neighbour_count;i++)
	       //	 {
		 //	tem=neighbours[i];
	//cout<<tem->x1<<","<<tem->y1<<"and "<<tem-> x2<<","<<tem-> y2<<endl;
	  //		if(tem->status==EMPTY)
	    //			cout<<"status:"<<"empty"<<endl;
		// }
		 find_path();
	  }while(current_node!=goal_node);
     //	  cout<<endl<<"path blocks are";
    //	  for(int i=0;i<path_count;i++)
	//  {
// cout<<endl<<path[i]->x1<<","<<path[i]->y1<< "and"<<path[i]->  x2<<","<<path[i]->y2;
  //	  }
		  initgraph(&driver,&mode,"C:\TURBOC3\BGI");
		  s.draw_border();
		  s.draw_obstacles();
		  setcolor(YELLOW);
		  circle(goal_x,goal_y,5);
		  char x1c[40],y1c[40],x2c[40],y2c[40];
		  sprintf(x1c,"%d",start_x);
		   sprintf(x2c,"%d",goal_x);
		   sprintf(y1c,"%d",start_y);
		   sprintf(y2c,"%d",goal_y);
		   strcat(x1c,", ");
		   strcat(x1c,y1c);
		   strcat(x2c,", ");
		   strcat(x2c,y2c);
		   strcat(x1c,"start position");
		   strcat(x2c,"goal position");
		  outtextxy(goal_x+10,goal_y+10,x2c);
		  setcolor(YELLOW);
		  circle(start_x,start_y,5);
		  outtextxy(start_x+10,start_y+10,x1c);
		  path_count--;
		  setcolor(WHITE);
		      q.set_current_node(q.return_root(),start_x,start_y);

		      setcolor(GREEN);
			///drwing old path
		  int midx= (path[0]->x1+path[0]->x2)/2;
		  int midy= (path[0]->y1+path[0]->y2)/2;
		line(start_x,start_y,midx,midy);
		distr+=distance(start_x,start_y,midx,midy);
		  for(int ss=0;ss<path_count;ss++)
		  {
			int midx= (path[ss]->x1+path[ss]->x2)/2;
			int midy= (path[ss]->y1+path[ss]->y2)/2;
			int midx1=(path[ss+1]->x1+path[ss+1]->x2)/2;
			int midy1=(path[ss+1]->y1+path[ss+1]->y2)/2;
			line(midx,midy,midx1,midy1);
			distr+=distance(midx,midy,midx1,midy1);
		  }
			 midx= (path[ss]->x1+path[ss]->x2)/2;
			 midy= (path[ss]->y1+path[ss]->y2)/2;
		 line(midx,midy,goal_x,goal_y);
		 distr+=distance(midx,midy,goal_x,goal_y);
		 /////drawing new path
		 second=time(NULL);
		 cout<<"Time\t"<<"Distance\t"<<endl;
		 cout<<"  "<<difftime(second,first)<<"\t"<<distr<<"\t"<<"FOR_GREEN_PATH"<<endl;
		 distr=0;
	       ////////
	      // cout<<"current_node"<<current_node<<" "<<endl;
		  //	for(int counter=0;counter<=path_count;counter++)
		    //	{
		//	cout<<" "<<path[counter];
		     //	}
		//	cout<<endl;
	      //		cout<<"goal_node "<<goal_node<<" ";
		//	cout<<endl;
		 node *new_path[100];

	    //	 cout<<endl<<"current_node"<<current_node<<endl;

		 new_path[0]=current_node;
		// cout<<" path_count "<<path_count<<endl;
		// cout<<"path[0] "<<path[path_count];
		 int newpath_counter=0;

		 for(int i=0;i<=path_count;i++)
		 {
		 new_path[i+1]=path[i];
		 }
		 new_path[++i]=goal_node;
		 newpath_counter=++i;
	       //	  cout<<endl<<"new path"<<endl;
	      //	 for(i=0;i<newpath_counter;i++)
	       //	 {
		// cout<<" "<<new_path[i];
	      //	 }
		 /////////////
		// initgraph(&gd,&gm,"c:\turboc3\bgi");
		 node *opt_path[100];
		 int optpath_counter=1;
		 node *hold=NULL;
		 opt_path[0]=current_node;
		 for(i=0;i<newpath_counter;i++)
		 {
		 hold=new_path[i+1];
		// cout<<"hello";
		 for(j=i+2;j<newpath_counter;j++)
		 {
		     if(1==check_path(new_path[i],new_path[j]))
		     {
		       hold=new_path[j];
		       i=j-1;
		     }
		  //   getch();

		 }
		 opt_path[optpath_counter++]=hold;
		 }

		//   cout<<endl<<"optimised path"<<endl;

	//       for(i=0;i<optpath_counter;i++)
	  //     {
	    //   cout<<opt_path[i]<< " ";
	      // }
	      // getch();
	      distr=0;
	      opt_path[0]->x1=start_x;
	      opt_path[0]->y1=start_y;
	       opt_path[0]->x2=start_x;
	      opt_path[0]->y2=start_y;

	      opt_path[optpath_counter-1]->x1=goal_x;
	      opt_path[optpath_counter-1]->y1=goal_y;
	       opt_path[optpath_counter-1]->x2=goal_x;
	      opt_path[optpath_counter-1]->y2=goal_y;
	       for(i=0;i<optpath_counter-2;i++)
	       {
	       draw_line(opt_path[i],opt_path[i+1]);
	       distr+=distance((opt_path[i]->x1+opt_path[i]->x2)/2,(opt_path[i]->y1+opt_path[i]->y2)/2,(opt_path[i+1]->x1+opt_path[i+1]->x2)/2,(opt_path[i+1]->y2+opt_path[i+1]->x2)/2);
	       }
	       second=time(NULL);
		cout<<"  "<<difftime(second,first)<<"\t"<<distr<<"\t"<<"FOR_YELLOW_PATH";
			 getch();
		 closegraph();
		 q.cleanup(q.return_root());


	  }
	  else if(choice==2)
	  {
		clrscr();
		driver=DETECT;
		initgraph(&driver,&mode,"C:\TURBOC3\BGI");
		temp_color=getcolor();
		s.draw_border();
		s.draw_obstacles();
		setcolor(YELLOW);
		circle(goal_x,goal_y,5);
		outtextxy(goal_x+7,goal_y,"Goal Location");
		setcolor(YELLOW);
		circle(start_x,start_y,5);
		outtextxy(start_x+7,start_y,"Start Location");
		setcolor(temp_color);
		grid g;
		g.develope_matrix();
		g.findpath_in_matrix();
		getch();
		closegraph();
		initgraph(&driver,&mode,"C:\TURBOC3\BGI");
		s.draw_border();
		s.draw_obstacles();
		setcolor(YELLOW);
		circle(goal_x,goal_y,5);
		setcolor(YELLOW);
		outtextxy(goal_x+7,goal_y,"Goal Location");
		circle(start_x,start_y,5);
		outtextxy(start_x+7,start_y,"Start Location");
		setcolor(temp_color);
		path_count--;
		int midx1,midx2;
		int midy1,midy2;
		for(int ctr=0;ctr<grid_path_count;ctr++)
		{
			midx1= (grid_path[ctr]->x1+grid_path[ctr]->x2)/2;
			midy1= (grid_path[ctr]->y1+grid_path[ctr]->y2)/2;
			midx2= (grid_path[ctr+1]->x1+grid_path[ctr+1]-> x2)/2;
			midy2= (grid_path[ctr+1]->y1+grid_path[ctr+1]-> y2)/2;
			if((midx2<goal_x)||(midy2<goal_y))
			{
				setcolor(WHITE);
				setlinestyle(CENTER_LINE,1,2);
				line( midx1,midy1,midx2,midy2);
			}
		 }
			getch();
			closegraph();
   }
	  else
	  exit(0);
}
/*** End of main() ***/

					/*** End of the Program ***/
