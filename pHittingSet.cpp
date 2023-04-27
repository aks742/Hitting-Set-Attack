
#include <iostream>
#include <list>
#include <set>
#include <ctime>
#include <chrono>
#include <random>

#include <vector>
#include <algorithm>
#include <thread>
#include <future>



//Maximum value of the PARTICIPATION COUNT SHOULD BE (SENDER_COUNT - 1) !!IT PROVIDES CLOSED SENDER ANONYMITY

using namespace std;

#define  M  5
#define TOTAL_RECIPIENT_COUNT  2000
#define SENDER_COUNT 10
#define REPETATION 1
#define NUM_THREADS 3





class Helper{
    public:
       static int getRandomElementFromList(list<int> lst){
            
            int rnd = rand() % lst.size();

            int counter = 0;
            for (int curr : lst){
                if(counter == rnd){
                    return curr;
                }
                counter++;
            }

            return 0;

        }

};

class HTSolver{
    public:
        list<list<int> > hslist;
        list<list<int> > rounds;
        int m;
        

        

        vector<int> convertList2Vector(list<int> inputList){
            vector<int> result;
            for(int i : inputList){
                result.push_back(i);
            }

            return result;
        }

        template<typename Container>
        bool
        compareHSAndNewSet(const Container &hs,
                        const Container &newset)
        {
            
         for(const auto &elem: newset)
            {
                if(std::binary_search(begin(hs), end(hs), elem))
                {
                return true; // found common element
                }
            }
            return false; // no common element
                    }


        HTSolver(int m){
            this->m = m;
        }
        
        void add(list<int> newset){
            if(hslist.size() == 0){    
                for(int i : newset){
                    list<int> tmp_list;
                    tmp_list.push_back(i);    
                    hslist.push_back(tmp_list);
                
                }         
                                
            }
            else{       
                list<list<int> > hscopy;
                hscopy = copyHSList(hslist);
                list<list<int> > new_set_list;
                vector <int> newsetv = convertList2Vector(newset);    
                
                //for (list<int> hs : hscopy){
                std::list<list<int> >::iterator  it;
                it = hscopy.begin();
                
                while(it != hscopy.end()){  
                    vector<future<list<list<int> >>> l;
                   // auto future = std::async(std::launch::async,&HTSolver::process,this,*it,newset,newsetv);
                   l.reserve(4);
                   l.push_back(std::async(std::launch::async,&HTSolver::process,this,*it,newset,newsetv));
                    ++it;
                   if(it != hscopy.end()){            
                        l.push_back(std::async(std::launch::async,&HTSolver::process,this,*it,newset,newsetv));
                    }

                     ++it;
                   if(it != hscopy.end()){            
                        l.push_back(std::async(std::launch::async,&HTSolver::process,this,*it,newset,newsetv));
                    }

                     ++it;
                   if(it != hscopy.end()){            
                        l.push_back(std::async(std::launch::async,&HTSolver::process,this,*it,newset,newsetv));
                    }

                    
                    

                    //list<list<int> > tmp_list = future.get();
                    
                    for(int i = 0; i<l.size();i++){
                        if(l.at(i).valid() == false) continue;
                        l.at(i).wait();
                        list<list<int> > tmp_list = l.at(i).get();
                        printHSList();

                        for (list<int> elem : tmp_list){
                        hslist.push_back(elem);
                        }

                    }
                    

                    

                    
                    ++it;
                    
                    }
                    hslist = clean_duplicates(hslist);
                    
                }

                rounds.push_back(newset);
                
            }
        
        list<list<int> > process(const list<int> &hs,const list<int> &newset,const vector<int> &newsetv){

                    vector<int> hsv = convertList2Vector(hs);
                    list<list<int> > new_set_list;
                    if(compareHSAndNewSet(hsv,newsetv) == true) {return new_set_list;}
                    unsigned change_flag = 0;
                    

                    for (int e:newset){
                        if(hs.size() < m){
                            change_flag = 1;
                            list<int> tmp_list = copyList(hs);
                            tmp_list.push_back(e);
                            tmp_list.sort();
                            new_set_list.push_back(tmp_list);

                        }
                        else{
                            change_flag = 1;
                            break;
                            }

                        }

                    if(change_flag == 1){
                        hslist.remove(hs);
                    }


                    return new_set_list;

        }
        

        list<list<int> > copyHSList(list<list<int> > a){
            list<list<int> > newlist;
            for(list<int> curr : a){
                list<int> tmp;
                for(int i : curr){
                    tmp.push_back(i);
                }
                newlist.push_back(tmp);
            }
            return newlist;

        }

        list<int> copyList(list<int> a){
            list<int> newlist;
            for(int curr : a){
                newlist.push_back(curr);
            }
            return newlist;
        }

        template<typename Container>
        bool compareHSAndNewSet2(const Container &hs , const Container &newset){
            bool hasCommon = false;          
           for(int x : newset){
               for(int y:hs){
                   if(x == y){
                       hasCommon = true;
                       return hasCommon;
                       
                   }
                   if(y > x){
                       break;
                   }

               }
           }

            return hasCommon;

        }

        void printHSList(void){
            cout<< "[";
            for(list<int> lst : hslist){
                cout<< "[";
                for(int a : lst){
                    cout<< a;
                    cout<<" ";
                }
                cout<<"], ";
            }

            cout<< ']';

        }

        

        list<list<int> > clean_duplicates(list<list<int> > listOfLists)
        {
            
            set<list<int> > setOfLists;
            for(auto iterator=listOfLists.begin(); iterator!=listOfLists.end(); ++iterator)
            {
                setOfLists.insert(*iterator);
            }
            
            listOfLists.clear();
            for(auto item : setOfLists)
            {
                listOfLists.push_back(item);
            }
            
            return listOfLists;
        }

        bool checkAttackFinish(){
            if(hslist.size() == 1){
                return true;
            }
        }
        
  
    
    
};



class Sender{
    public:
        list<int> recipient_set;
        list<list<int> > rounds;
        bool selected = false;
        bool isAlice = false;
    
        Sender(){
            selected = false;
            isAlice = false;
            createRecipientSet();

        }

        void createRecipientSet(){
            
            for(int i =0;i<M;i++){
                recipient_set.push_back((rand() % TOTAL_RECIPIENT_COUNT) + 1);

            }

        }

        int createRound(){
            if(selected == true || isAlice == true)
            {
            return Helper::getRandomElementFromList(recipient_set);
            }
            else{
            return (rand() % TOTAL_RECIPIENT_COUNT) + 1;
            }
        }


};


class Simulation{
    public:
    vector<Sender> senderList;
    Sender alice;
    list<list<int> > rounds;
    int completed_step;
    int part_count;
    HTSolver hts = HTSolver(M);

    Simulation(int _participation_count){
        part_count = _participation_count;
        initSenders();
    }

    void initSenders(){
        Sender s;
        s.selected = true;
        s.isAlice = true;
        alice = s;
        

        for(int i=0;i<SENDER_COUNT - 1;i++){
            Sender os;
            senderList.push_back(os);
        }

    }

    list<int> createRound(){
        list<int> round;
        round.push_back(alice.createRound());
        setParticipatingSenders();
        for(Sender s : senderList){
            round.push_back(s.createRound());
        }

        rounds.push_back(round);
        return round;
    }

    void run(){
        hts.add(createRound());
        while(hts.checkAttackFinish() == false)
        {
        hts.add(createRound());
        //hts.printHSList();
        //cout<<endl;
        }
        completed_step = hts.rounds.size();
    }

    void setParticipatingSenders(){
        std::random_device rd;
        std::default_random_engine rng(rd());
        shuffle(std::begin(senderList), std::end(senderList), rng);

        for(int i=0;i<senderList.size();i++){
            if(i<part_count){
                senderList.at(i).selected = true;
            }
            else{
                senderList.at(i).selected = false;
            }
        }






    }



};

class Experiment{
    public:
    list<Simulation> simulationList;
    list<int> completed_step_set;
    int part_count;
    Experiment(int _participation_count){
        part_count = _participation_count;

    }

    void runExperiment(){
        for(int i=0;i<REPETATION;i++){
            Simulation s = Simulation(part_count);
            s.run();
            completed_step_set.push_back(s.completed_step);
            //simulationList.push_back(s);
        }
        double total = 0.0;
        cout<<"Experiment Completed, Rep:"<<REPETATION<<" Participation Count="<<part_count<<" m="<<M << " batch_size="<<SENDER_COUNT<<" n="<<TOTAL_RECIPIENT_COUNT<<endl<<"[";
        for(int curr : completed_step_set){
            cout<<curr<<", ";
            total = total + curr;
        }
        cout<<"] AVG="<<total/completed_step_set.size()<<endl;
    }





};

int main()
{
    srand((unsigned) time(0));
    
    
    
    HTSolver hts = HTSolver(M);
   
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


   
   auto t1 = high_resolution_clock::now(); 
   for(int i=0;i<1;i++){
   Experiment e = Experiment(i);
   e.runExperiment();
   }
   auto t2 = high_resolution_clock::now();

   

   duration<double, std::milli> ms_double = t2 - t1;
   cout<<endl << "Running takes " << ms_double.count() / 1000 << " seconds";


    return 0;
}



