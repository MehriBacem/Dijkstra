#include <iostream>
#include <string>
#include<map>
#include<climits>   // pour utiliser FLT_MAX( infini)
#include<utility>  //Pour utiliser la pair
#include<vector>
#include<fstream>
#include<algorithm>

using namespace std;

class reseau;
class sommet ;

enum Status {
NOT_VISITED,
VISITED
};

class connexion
{

public :

  connexion(sommet* som_s, sommet* som_d , double valeur, int li )
     {
         startingsom= som_s ;
         endingsom = som_d ;
         ligne=li ;
         capacity=valeur ;
     }

      sommet* getStartingsom()
      {
          return startingsom ;
      }
      sommet* getEndingsom()
      {
          return endingsom ;
      }
        double getCapacity()
        {
            return capacity ;
        }
        int getligne()
        {
            return ligne;
        }

        void setStartingsom(sommet* a)
        {
            startingsom=a ;
        }
        void setEndingsom(sommet* b)
        {
            endingsom =b ;
        }
        void setCapacity(double val)
        {
            capacity=val ;
        }
friend void creation(reseau &a,ifstream &flux,string &ligne_1,string &ligne_2);
friend void ligne_suiv(reseau &a,ifstream &flux,string &ch,string &ligne1,string &ligne2,int &i,sommet p,sommet q);
friend void recursive(reseau &a,string source,int tr);
friend void dijkstra(reseau &a,string source,string destination);
     private :
     sommet *startingsom;
        sommet *endingsom;
        double capacity ;
        int ligne ;

};
class  sommet
{

 private :
     string  nom_s ;
     vector<connexion> adjsomList;
       enum Status status;


 public :

    sommet ()
    {
       nom_s="" ;
    }
     sommet ( string nom)
     {
         nom_s = nom ;
         status= NOT_VISITED ;
     }
     sommet(const sommet& o)
     {
         nom_s=o.nom_s;
         adjsomList=o.adjsomList;
     }
     /*~sommet()
{
   adjsomList.clear();
  // cout<<"destruction de sommet "<<getName()<<endl;
}*/
enum Status getStatus()
{
return status;
}
void setStatus(enum Status st)
{
status = st;
}
void addAdjsom(sommet *adj, double cost,int lign)
{

 connexion  newConnexion(this, adj, cost, lign) ;
adjsomList.push_back(newConnexion);
}
vector<connexion>& getAdjsomList()
{
return adjsomList;
}
void displayList()
{

for(std::vector<connexion>::size_type i=0 ; i < adjsomList.size() ; i++)
{
 connexion   arc=adjsomList[i];
cout << nom_s<< " -> " << arc.getEndingsom()->getName() <<" "<<arc.getligne() <<" "<<arc.getCapacity()<<endl ;
}

}
     string getName ()
     {
         return nom_s ;
     }
     void setname (string ch)
     {
         nom_s=ch;
     }
     void setadjsomlist()
     {
         adjsomList.clear();
     }
     friend void creation(reseau &a,ifstream &flux,string &ligne_1,string &ligne_2);
     friend void ligne_suiv(reseau &a,ifstream &flux,string &ch,string &ligne1,string &ligne2,int &i,sommet p,sommet q);
     friend void recursive(reseau &a,string source,int tr);
friend void dijkstra(reseau &a,string source,string destination);
};

class reseau
{  private:
    vector<sommet*> somList;  //tableau des sommets
    bool TrouverCycle;

public:
    void clearVisited()
{
for(std::vector<sommet*>::size_type i = 0; i < somList.size(); i++)
{
somList[i]->setStatus(NOT_VISITED);
}
}
void addNewsommet(sommet *nsom)
{
somList.push_back(nsom);
}

sommet* findSomByName(string name)
{
for(std::vector<sommet*>::size_type i = 0 ; i < somList.size() ; i++)
{
if(somList[i]->getName() == name)
return somList[i];
}
return NULL;
}


reseau()
{
TrouverCycle = false;
}

/*~reseau()
{
for(std::vector<sommet*>::size_type i=0 ; i < somList.size() ; i++)
delete somList[i];
somList.clear();
//cout<<"destruction reseau"<<endl;
}*/


void displayGraph()
{
for(std::vector<sommet*>::size_type i=0 ; i < somList.size() ; i++)
{
somList[i]->displayList();
}
}
friend void creation(reseau &a,ifstream &flux,string &ligne_1,string &ligne_2);
friend void ligne_suiv(reseau &a,ifstream &flux,string &ch,string &ligne1,string &ligne2,int &i,sommet p,sommet q);
friend void recursive(reseau &a,string source,int tr);
friend void dijkstra(reseau &a,string source,string destination);
};



void ligne_suiv(reseau &a,ifstream &flux,string &ch,string &ligne1,string &ligne2,int &i,sommet p,sommet q);
void creation(reseau &a,ifstream &fluxx,string &ch,string &ligne_1,string &ligne_2,int &i,sommet p,sommet q)
{
    ligne_1=ligne_2;
    p.setname(ligne_1);
    p.setadjsomlist();
    q.setadjsomlist();
    if(a.findSomByName(ligne_1)==NULL)
    {
        a.addNewsommet(&p);
        getline(fluxx,ligne_2);
        if(ligne_2=="")
        {
            ligne_suiv(a,fluxx,ch,ligne_1,ligne_2,i,p,q);
        }
        else
        {
            q.setname(ligne_2);
            q.setadjsomlist();
            a.findSomByName(ligne_1)->addAdjsom(&q,2,i);
            creation(a,fluxx,ch,ligne_1,ligne_2,i,p,q);
        };
    }
    else
    {
        getline(fluxx,ligne_2);
        if(ligne_2=="")
        {
            ligne_suiv(a,fluxx,ch,ligne_1,ligne_2,i,p,q);
        }
        else
        {
            q.setname(ligne_2);
            q.setadjsomlist();
            a.findSomByName(ligne_1)->addAdjsom(&q,2,i);
            creation(a,fluxx,ch,ligne_1,ligne_2,i,p,q);
        };
    };
}
void dijkstra(reseau &a,string source,string destination)
{
    map<sommet*,pair<int,sommet*> > mymap;
    vector<sommet*>::const_iterator itr=a.somList.begin();

    while(itr!=a.somList.end())
    {
        mymap[(*itr)]=make_pair(INT_MAX,new sommet);
        ++itr;

    };
    mymap[a.findSomByName(source)]=make_pair(0,new sommet);

bool tout_visite=false;

    itr=a.somList.begin();

while(tout_visite==false)
{
    itr=a.somList.begin();
    while(itr!=a.somList.end())
    {
        if ((*itr)->getName()==source)
        {

            vector<connexion>::iterator ptr=(*itr)->adjsomList.begin();

            while(ptr!=(*itr)->adjsomList.end())
            {
                if((mymap.find(a.findSomByName((ptr)->getEndingsom()->getName()))->second).first > (mymap.find(a.findSomByName((ptr)->getStartingsom()->getName()))->second).first + (ptr)->getCapacity())
                {
                   (mymap.find(a.findSomByName((ptr)->getEndingsom()->getName()))->second).first= (mymap.find(a.findSomByName((ptr)->getStartingsom()->getName()))->second).first + (ptr)->getCapacity();
                   (mymap.find(a.findSomByName((ptr)->getEndingsom()->getName()))->second).second=(*itr);
                };
                 ++ptr;
            };
            (*itr)->setStatus(VISITED);
        };
        ++itr;
    };


    map<sommet*,pair<int,sommet*> >::iterator ctr=mymap.begin();
    int indmin=1000;
    string ch;
    while(ctr!=mymap.end())
    {
        if((indmin>((ctr)->second).first) && (((ctr)->first)->getStatus()==NOT_VISITED))
           {
               indmin=((ctr)->second).first;
               ch=((ctr)->first)->getName();
           };
        ++ctr;
    };
    source=ch;
    tout_visite=true;
    ctr=mymap.begin();
    while(ctr!=mymap.end())
    {
        if(((ctr)->first)->getStatus()==NOT_VISITED)
          {
              tout_visite=false;
              break;
          };
        ++ctr;
    };

 };
    map<sommet*,pair<int,sommet*> >::iterator ctr=mymap.begin();
   // while(ctr!=mymap.end())
    //{
      //  cout<<"<"<<((*ctr).first)->getName()<<",<"<<((*ctr).second).first<<","<<(((*ctr).second).second)->getName()<<">>"<<endl;
        //++ctr;
    //}

cout<<"Temps de parcours = "<<(mymap.find(a.findSomByName(destination))->second).first<<endl;
vector<string> T ; 
while(destination!="")
{  string ch ;
    ch=(mymap.find(a.findSomByName(destination))->first)->getName() ;
    T.push_back(ch) ;
    destination=((mymap.find(a.findSomByName(destination))->second).second)->getName();
};
reverse(T.begin(),T.end()) ;

cout<<"\n"<<endl ;

for(int i=0;i<T.size();i++)
{ 
cout<<T[i]<<"-->" ;
}

}










void ligne_suiv(reseau &a,ifstream &flux,string &ch,string &ligne1,string &ligne2,int &i,sommet p,sommet q)
{
    if(ch=="Ligne 14-b")
        {
            a.clearVisited();
        string source ;
         string destination;
         cout<<"          ------------------------------------------------------------------   "<<endl;
         cout<<"                   **** La premiere partie du projet ****               "<<endl;
		  cout<< "             **** Calcul du plus court chemin entre deux stations qcqs****          "<<endl;
         cout<<"          ------------------------------------------------------------------   "<<endl;
        cout<<"Donner la source : "<<endl;
       getline(cin,source);
        cout<<"Donner la destination :"<<endl ;
        getline(cin,destination); 
        int tr ;
        cout<<"Donner la ligne "<<endl;
        cin>>tr ;
        
        
        recursive(a,source,tr);
        cout<<"Le parcours  suivi est :"<<endl;
        dijkstra(a,source,destination);
        }
    else
    {
        i++;
               getline(flux,ch);
               getline(flux,ligne1);
               getline(flux,ligne2);
               p.setname(ligne1); p.setadjsomlist();
               q.setname(ligne2); q.setadjsomlist();
               if(a.findSomByName(ligne1)==NULL)
               {
                   a.addNewsommet(&p);
                   a.findSomByName(ligne1)->addAdjsom(&q,2,i);
               }
               else
               {
                   a.findSomByName(ligne1)->addAdjsom(&q,2,i);
               }
               creation(a,flux,ch,ligne1,ligne2,i,p,q);
    }

}

void recursive(reseau &a,string source,int tr)
{
    int n;
    bool exist=false;
    for(std::vector<connexion>::size_type i=0;i<a.findSomByName(source)->getAdjsomList().size();i++)
    {
        if(a.findSomByName(source)->getAdjsomList()[i].getligne()==tr)
        {
            n=i;
            exist=true;
        }else
        a.findSomByName(source)->getAdjsomList()[i].setCapacity(8);
    };
    if(exist==true)
    {
        source=a.findSomByName(source)->getAdjsomList()[n].getEndingsom()->getName();
        recursive(a,source,tr);
    }
    return;

}


int main()
{
	     reseau a ;
        ifstream flux("ville.txt", ios::in);  // on ouvre en lecture

       if(flux)
       {   int i=0;
           string ch;
           string ligne1;
           string ligne2;
           sommet p;
           sommet q;
           ligne_suiv(a,flux,ch,ligne1,ligne2,i,p,q);
       }
        else
     cerr << "Impossible d'ouvrir le fichier !" << endl;



	    return 0;
}
