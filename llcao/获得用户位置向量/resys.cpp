#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<sstream>

using namespace std;

#define K 20
#define ED 0.0000001
#define GeneNum 50     //"����"�Ż�����
#define USER_NUM  943
#define ITEM_NUM  1683

struct user_info   //�û�������Ϣ
{
    int id;
    int age;
    string sex;
    string occupation;
    string zip;
	double rate[ITEM_NUM];
    double distance[K];
};

struct Pop //�������ģ��������־��ࣩ
{
    user_info clustercenter[K];
};

struct user_position    //�û���λ������
{
	set<int> cluster_age;
	set<int> cluster_sex;
	set<int> cluster_occupation;
	set<int> cluster_location;
	set<int> cluster_rate;
};

typedef set<int> rateSet;

struct commu_re{
	set<int> community;
	set<int> reList;
};

user_info user[USER_NUM];      //����943���û�
user_position user_pos[USER_NUM];  //�����û���Ӧ��λ������
Pop pop;               //����������ĵ�ʵ��
int cluster[K][USER_NUM];
int clusternum[K];
double clustermean[K];
double innerdistance;
double oldinnerdistance;
double btdistance;
double fitness;

commu_re children,teenagers,adults,seniors;  //���������
commu_re male,female;                        //���Ա����
commu_re administrator,artist,executive,engineer,entertainment,healthcare,
		homemaker,librarian,marketing,none,programmer,scientist,student,technician,writer,other;//��ְҵ����
commu_re loc0,loc1,loc2,loc3,loc4,loc5,loc6,loc7,loc8,loc9,loc10; //����������
rateSet rateSetList[K];//�����û����־��༯�ϵ�����

void input()//��������
{
	ifstream in_f1("user.txt");
	if(!in_f1)
		cerr<<"open error!";
     
    for(int i = 0;!in_f1.eof();i++)
    {
        in_f1>>user[i].id;
        in_f1>>user[i].age;
        in_f1>>user[i].sex;
        in_f1>>user[i].occupation;
        in_f1>>user[i].zip;
    }

    in_f1.close();

    int data,num_u,num_i;
    string str;
    ifstream in_f2("u1.base");
	if(!in_f2){
		cerr<<"open error!";
		exit(-1);
	}

    for( ;!in_f2.eof(); )
    {
        in_f2>>num_u;
        in_f2>>num_i;
        in_f2>>data;
        in_f2>>str;
		user[num_u-1].rate[num_i-1] = data;
    }
    in_f2.close();
}
int find(int a[],int n,int b)
{
    int ii;
    for(ii=0;ii<n;ii++)
    if(a[ii]==b) return 1;
    return 0;
}
void sort(int a[],int n)  //ð������
{
    int temp;
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<n-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
}
void init()//��ʼ����������
{
    int i,j;
    int num=0;
    int random;
    int rt[K];
    srand(time(NULL));
    for( ;num<K; )
    {
        random=rand()%USER_NUM;
        if(!find(rt,num,random))
           rt[num++]=random;
    }
    sort(rt,K);
    for(i=0;i<K;i++)
       for(j=0;j<ITEM_NUM;j++)
       {
           pop.clustercenter[i].rate[j]=user[rt[i]].rate[j];
       }
}
void printfpop()    //��ӡ������
{
    int i,j;
    for(i=0;i<K;i++)
    {
        cout<<"Cluster "<<i+1<<" includes: "<<"(";
		for(j=0;j<clusternum[i];j++)
		{
			cout<<cluster[i][j]<<" ";
		}
		cout<<")"<<endl;
    }
	//printf("׼����ֵΪ��%4.5f\n",fitness);
}
double eucliddistance(int x,int y)
{
	double sum=0,len_a=0,len_b=0;
	double result;
	for(int i=0;i<ITEM_NUM;i++)
    {
		sum+=user[x].rate[i]*pop.clustercenter[y].rate[i];
		len_a+=pow(user[x].rate[i],2.0);
		len_b+=pow(pop.clustercenter[y].rate[i],2.0);
	}
	result=sum/(sqrt(len_a)*sqrt(len_b));
	return result;
}
void calcuatedistance()
{
	int i,j;
	for(i=0;i<USER_NUM;i++)
	   for(j=0;j<K;j++)
	   {
		  user[i].distance[j]=eucliddistance(i,j);
	   }
}
void Cluster()
{
    int i,j,k;
    double min;
    int index;
    for(k=0;k<K;k++)
       clusternum[k]=0;
    for(i=0;i<USER_NUM;i++)
    {
        index=0;
        min=user[i].distance[0];
        for(j=1;j<K;j++)
        {
            if(user[i].distance[j]<min)
            {
                min=user[i].distance[j];
                index=j;
            }
        }
        cluster[index][clusternum[index]++]=i;//�ŵ�Ⱥ�У���Ⱥ������+1
    }
	
	oldinnerdistance=innerdistance;  
    innerdistance=0.0;               
    for(i=0;i<K;i++)
    {
        for(j=0;j<clusternum[i];j++)
            innerdistance+=pow(user[cluster[i][j]].distance[i],2);
    }
    fitness=1/(1+innerdistance);
}
void updateclustercenter()
{
    int i,j,k;
    double sum;
    for(i=0;i<K;i++)
    {
        for(j=0;j<ITEM_NUM;j++)
        {
            sum=0;
            for(k=0;k<clusternum[i];k++)
            {
                sum+=user[cluster[i][k]].rate[j];
            }
            pop.clustercenter[i].rate[j]=sum/clusternum[i];
        }
    }
}
void evaluatepop()
{
    calcuatedistance();
    Cluster();
}

void user_infoCluster(){
	//���������
    for(int j = 0;j<USER_NUM;j++)
	{
		if(user[j].age<12)
			children.insert(j);
        else if(user[j].age>= 12 && user[j].age < 18)
			teenagers.insert(j);
		else if(user[j].age >= 18 && user[j].age < 40)
			adults.insert(j);
		else 
			seniors.insert(j);
   }
   //���Ա����
   for(int j = 0;j<USER_NUM;j++)
   {
	   if(user[j].sex=="M")
		   male.insert(j);
	   else
		   female.insert(j);
   }
   //��ְҵ����
   for(int j = 0;j<USER_NUM;j++)
   {
	   if(user[j].occupation=="administrator")
		   administrator.insert(j);
	   else if(user[j].occupation=="artist")
		   artist.insert(j);
	    else if(user[j].occupation=="executive")
		   executive.insert(j);
	    else if(user[j].occupation=="engineer")
		   engineer.insert(j);
		else if(user[j].occupation=="entertainment")
		   entertainment.insert(j);
		else if(user[j].occupation=="healthcare")
		   healthcare.insert(j);
		else if(user[j].occupation=="homemaker")
		   homemaker.insert(j);
		else if(user[j].occupation=="librarian")
		   librarian.insert(j);
	    else if(user[j].occupation=="marketing")
		   marketing.insert(j);
	    else if(user[j].occupation=="none")
		   none.insert(j);
		else if(user[j].occupation=="programmer")
		   programmer.insert(j);
		else if(user[j].occupation=="scientist")
		   scientist.insert(j);
		else if(user[j].occupation=="student")
		   student.insert(j);
	    else if(user[j].occupation=="technician")
		   technician.insert(j);
		else if(user[j].occupation=="writer")
		   writer.insert(j);
		else
		   other.insert(j);
   }
   //���û����ڵ�������
   for(int j = 0;j<USER_NUM;j++)
   {
	    if(user[j].zip.at(0)=='0')
		   loc0.insert(j);
	   else if(user[j].zip.at(0)=='1')
		   loc1.insert(j);
		else if(user[j].zip.at(0)=='2')
		   loc2.insert(j);
	   else if(user[j].zip.at(0)=='3')
		   loc3.insert(j);
		else if(user[j].zip.at(0)=='4')
		   loc4.insert(j);
	   else if(user[j].zip.at(0)=='5')
		   loc5.insert(j);
		else if(user[j].zip.at(0)=='6')
		   loc6.insert(j);
	   else if(user[j].zip.at(0)=='7')
		   loc7.insert(j);
		else if(user[j].zip.at(0)=='8')
		   loc8.insert(j);
	   else if(user[j].zip.at(0)=='9')
		   loc9.insert(j);
        else
		   loc10.insert(j);
   }

    btdistance=1;
    innerdistance=100000;
    //input();
    //init();
    for(int i=0;(i<GeneNum)&&(btdistance>ED);i++)
    {
        evaluatepop();
        btdistance=oldinnerdistance-innerdistance;
        //cout<<"��"<<i+1<<"��"<<endl;
        //cout<<"�Ż�ֵ��"<<btdistance<<endl;
        updateclustercenter();
    }

	for(int i=0;i<K;i++)
		for(int j=0;j<clusternum[i];j++)
			rateSetList[i].insert(cluster[i][j]);
}
void setUser_pos(){
 //���������,����λ������
    for(int j = 0;j<USER_NUM;j++)
	{
		if(user[j].age<12)
			user_pos[j].cluster_age = children;
        else if(user[j].age>= 12 && user[j].age < 18) 
			user_pos[j].cluster_age = teenagers;
		else if(user[j].age >= 18 && user[j].age < 40)
			user_pos[j].cluster_age = adults;
		else
			user_pos[j].cluster_age = seniors;
   }
   //���Ա����,����λ������
   for(int j = 0;j<USER_NUM;j++)
   {
	   if(user[j].sex=="M")
		   user_pos[j].cluster_sex = male;
	   else
		   user_pos[j].cluster_sex = female;
   }
   //��ְҵ����,����λ������
   for(int j = 0;j<USER_NUM;j++)
   {
	   if(user[j].occupation=="administrator")
		   user_pos[j].cluster_occupation = administrator;
	   else if(user[j].occupation=="artist")
		   user_pos[j].cluster_occupation = artist;
	    else if(user[j].occupation=="executive")
		   user_pos[j].cluster_occupation = executive;
	    else if(user[j].occupation=="engineer")
		   user_pos[j].cluster_occupation = engineer;
		else if(user[j].occupation=="entertainment")
		   user_pos[j].cluster_occupation = entertainment;
		else if(user[j].occupation=="healthcare")
		   user_pos[j].cluster_occupation = healthcare;
		else if(user[j].occupation=="homemaker")
		   user_pos[j].cluster_occupation = homemaker;
		else if(user[j].occupation=="librarian")
		   user_pos[j].cluster_occupation = librarian;
	    else if(user[j].occupation=="marketing")
		   user_pos[j].cluster_occupation = marketing;
	    else if(user[j].occupation=="none")
		   user_pos[j].cluster_occupation = none;
		else if(user[j].occupation=="programmer")
		   user_pos[j].cluster_occupation = programmer;
		else if(user[j].occupation=="scientist")
		   user_pos[j].cluster_occupation = scientist;
		else if(user[j].occupation=="student")
		   user_pos[j].cluster_occupation = student;
	    else if(user[j].occupation=="technician")
		   user_pos[j].cluster_occupation = technician;
		else if(user[j].occupation=="writer")
		   user_pos[j].cluster_occupation = writer;
		else
		   user_pos[j].cluster_occupation = other;
   }
   //���û����ڵ�������,����λ������
   for(int j = 0;j<USER_NUM;j++)
   {
	    if(user[j].zip.at(0)=='0')
		   user_pos[j].cluster_location = loc0;
	   else if(user[j].zip.at(0)=='1')
		   user_pos[j].cluster_location = loc1;
		else if(user[j].zip.at(0)=='2')
		   user_pos[j].cluster_location = loc2;
	   else if(user[j].zip.at(0)=='3')
		   user_pos[j].cluster_location = loc3;
		else if(user[j].zip.at(0)=='4')
		   user_pos[j].cluster_location = loc4;
	   else if(user[j].zip.at(0)=='5')
		   user_pos[j].cluster_location = loc5;
		else if(user[j].zip.at(0)=='6')
		   user_pos[j].cluster_location = loc6;
	   else if(user[j].zip.at(0)=='7')
		   user_pos[j].cluster_location = loc7;
		else if(user[j].zip.at(0)=='8')
		   user_pos[j].cluster_location = loc8;
	   else if(user[j].zip.at(0)=='9')
		   user_pos[j].cluster_location = loc9;
        else
		   user_pos[j].cluster_location = loc10;
   }
   for(int j=0;j<USER_NUM;j++)
   {
	   for(int m=0;m<K;m++)
		   for(int n=0;n<clusternum[m];n++)
			   if(j==cluster[m][n])
				   user_pos[j].cluster_rate = rateSetList[m]; 
   }

}
void printUser_pos(){
	 for(int j = 0;j<USER_NUM;j++)
    {
	          if(j<=8)
	          cout<<"�û�"<<j+1<<"  ��λ������Ϊ��  ";
	          if(j>8&&j<=98)
	          cout<<"�û�"<<j+1<<" ��λ������Ϊ��  ";
              if(j>98)
	          cout<<"�û�"<<j+1<<"��λ������Ϊ��  ";

			  if(user_pos[j].cluster_age == children)
				  cout<<"["<<"children,";
			  if(user_pos[j].cluster_age == teenagers)
				  cout<<"["<<"teenagers,";
			  if(user_pos[j].cluster_age == adults)
				  cout<<"["<<"adults,";
			  if(user_pos[j].cluster_age == seniors)
				  cout<<"["<<"seniors,";
		   
			  if(user_pos[j].cluster_sex == male)
				  cout<<" M,";
			  if(user_pos[j].cluster_sex == female)
				  cout<<" F,";

			  if(user_pos[j].cluster_occupation == administrator)
				  cout<<" administrator,";
			  if(user_pos[j].cluster_occupation == artist)			 
				  cout<<" artist,";
			  if(user_pos[j].cluster_occupation == executive)
				  cout<<" executive,";
			  if(user_pos[j].cluster_occupation == engineer)
				  cout<<" engineer,";
			  if(user_pos[j].cluster_occupation == entertainment)			  
					cout<<" entertainment,";
			  if(user_pos[j].cluster_occupation == healthcare)
				   cout<<" healthcare,";
			  if(user_pos[j].cluster_occupation == homemaker)
				  cout<<" homemaker,";
			  if(user_pos[j].cluster_occupation == librarian)
				  cout<<" librarian,";
			  if(user_pos[j].cluster_occupation == marketing)
				  cout<<" marketing,";
			  if(user_pos[j].cluster_occupation == none)
				  cout<<" none,";
			  if(user_pos[j].cluster_occupation == programmer)
				  cout<<" programmer,";
			  if(user_pos[j].cluster_occupation == scientist)
				  cout<<" scientist,";
			  if(user_pos[j].cluster_occupation == student)
				  cout<<" student,";
			  if(user_pos[j].cluster_occupation == technician)
				  cout<<" technician,";
			  if(user_pos[j].cluster_occupation == writer)
				  cout<<" writer,";
			  if(user_pos[j].cluster_occupation == other)
				  cout<<" other,";

			  if(user_pos[j].cluster_location == loc0)
				  cout<<" loction 0,";
			  if(user_pos[j].cluster_location == loc1)
				  cout<<" loction 1,";
			  if(user_pos[j].cluster_location == loc2)
				  cout<<" loction 2,";
			  if(user_pos[j].cluster_location == loc3)
				  cout<<" loction 3,";
			  if(user_pos[j].cluster_location == loc4)
				  cout<<" loction 4,";
			  if(user_pos[j].cluster_location == loc5)
				  cout<<" loction 5,";
			  if(user_pos[j].cluster_location == loc6)
				  cout<<" loction 6,";
			  if(user_pos[j].cluster_location == loc7)
				  cout<<" loction 7,";
			  if(user_pos[j].cluster_location == loc8)
				  cout<<" loction 8,";
			  if(user_pos[j].cluster_location == loc9)
				  cout<<" loction 9,";

			  for(int i=0;i<K;i++){
                  if(user_pos[j].cluster_rate == rateSetList[i])
					  cout<<" RateSet"<<i+1<<"]";
			  }

			  cout<<endl;
	 }
}

int main()
{
	input();
	init();
    user_infoCluster();
	setUser_pos();
	printUser_pos();
}
