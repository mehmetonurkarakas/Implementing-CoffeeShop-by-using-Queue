#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

struct Order // DoneTime is TurnAround time for me
{
    double arrivalTime;
    double orderTime;
    double brewTime;
    double coffePrice;
    double doneTime =0;
    int section = 0;
    struct Order *next;
};

class CashierQueue{ // Standart Queue implementation
    public:

    Order *front = NULL;
    Order *back = NULL;

    void Enqueue(Order *ord)    
	// insertion values from rear side
    {
        ord->next=NULL;
 
        if(front == NULL)
            front = back = ord;
        else
        {
            back->next = ord;
            back = ord;
        }
    }
    void Dequeue()            
	// delete values from front side
    {
        Order *temp;
        if(front == NULL)
            cout<<"Queue is Empty";
        else
        {
            temp= front;
            if (front->next==NULL)
            {
                front = back = NULL;
            }
            delete temp;
        }
    }
};
class BaristaQueue{// My Barista Queue implementation
    public:

    Order *max = NULL;

    void Enqueue(Order *ordr)    
	// insertion values from rear side
    {
        ordr->next=NULL;
 
        if(max == NULL){
            max = ordr;
        }   
        else
        {
            Order *tempr = max;

            if (ordr->coffePrice>tempr->coffePrice)
            {
                ordr->next = tempr;
                max = ordr;
            }
            else{
                while (tempr->next!=NULL&&tempr->next->coffePrice>ordr->coffePrice)
                {
                    tempr = tempr->next;
                }
                ordr->next = tempr->next;
                tempr->next = ordr;
            }
        }
    }
    void Dequeue()            
	// delete values from front side
    {
        Order *temp;
        if(max == NULL)
            cout<<"Queue is Empty";
        else
        {
            temp= max;
            max = max->next;
            delete temp;
        }
    }
};


class Cashier{// Worked time = BusyTime
    public:
    int id;
    double workedTime=0 ;
    double doneTime=0 ;
    bool isFree=true; 
    Order order;
};

class Barista{
    public:
    int id;
    double workedTime=0 ;
    double doneTime=0 ;
    bool isFree = true; 
    Order order;
};

vector<Cashier> createCashier(double countOfCashier){ // I'm creating cashiers and keep it vector form 
    
    vector<Cashier> cashierss;
    for (int i = 1; i <= countOfCashier; i++)
    {
        Cashier c ;
        c.id = i;
        cashierss.push_back(c);
    }
    return cashierss;
}
vector<Barista> createBarist(double countOfBarista){// Same as cashier

    vector<Barista> baristass;
    for (int i = 1; i <= countOfBarista; i++)
    {
        Barista b ;
        b.id = i;
        baristass.push_back(b);
    }
    return baristass;
}
Order createOrder(vector<double> numbersInLine){// Creating order objects
    Order order;
    order.arrivalTime = numbersInLine[0];
    order.orderTime = numbersInLine[1];
    order.brewTime = numbersInLine[2];
    order.coffePrice = numbersInLine[3];
    return order;

}
vector<Order> orders;
CashierQueue cashierQ;
vector<Cashier> cashiers;
vector<Barista> baristas;
BaristaQueue baristaQ;


void goToCashierQ(Order &order){// Going cashier queue
    Order *ord = new Order;
    *ord = order;
    cashierQ.Enqueue(ord);
}


bool goToCashier(Order &order){// Going cashier and all fields initialize
    for (int i = 0; i < cashiers.size(); i++)
        {
            if (cashiers.at(i).isFree)
            {
                order.section = 2;  
                order.doneTime += order.orderTime; 
                cashiers.at(i).order=order;
                cashiers.at(i).isFree=false;
                cashiers.at(i).doneTime = order.doneTime;

                cashiers.at(i).workedTime += order.orderTime;

                
                return true;
            }
        }
    return false;
}

vector<double> turnAround;

bool goToBarista(Order &order){
    for (int i = 0; i < baristas.size(); i++)
    {
        if (baristas.at(i).isFree)
            {
                order.section = 4;
                order.doneTime += order.brewTime;
                turnAround.push_back(order.coffePrice);
                turnAround.push_back(order.doneTime);
                baristas.at(i).order=order;
                baristas.at(i).isFree=false;
                baristas.at(i).doneTime=order.doneTime;
                baristas.at(i).workedTime += order.brewTime;
                return true;
            }              
    }
    return false;
}

void Split(string inpt,string output){// I'm doing all thingh there
    ifstream inputFile;
    inputFile.open(inpt.c_str());
    ofstream outputfile;
    outputfile.open(output.c_str(),ios::app);
    int i = 0,j = 0;
    char seperate = ' ';
    string temp;string line1;
    while (getline(inputFile, line1)) // reading line by line 
    {
        stringstream ss(line1);
        vector<double> allNumbers;
        while (getline(ss, temp, seperate))// Splitting IO
        {
            stringstream s(temp);
            double value;
            s >> value;
            if (i==0)
            {
                cashiers = createCashier(value);
                baristas = createBarist(value/3);
            }
            else if (i==1)
            {
                break;
            }
            else{
                allNumbers.push_back(value);
            }
        }
        if (i>1)
        {
            orders.push_back(createOrder(allNumbers));
        }
        i++;
    }
    inputFile.close();

    


    for (int i = 0; i < 6; i++)
    {
        orders.at(i).doneTime += orders.at(i).arrivalTime;
        goToCashier(orders.at(i));
    }

    int maxSizeBaristaQ = 0;
    int maxSizeCashierQ = 0;

    double clock = 0;
    while (true)
    {
        bool orderGoToCashierorQ = false;
        bool cashierJobDone = false;
        bool baristasJobDone = false;
        double smallestTime = 1000;// finding smallest time value for continue all system. all cashiers , and baristas
        for (int i = 0; i < cashiers.size(); i++)
        {
            if (cashiers.at(i).doneTime<smallestTime&&cashiers.at(i).doneTime>clock)
            {
                cashierJobDone = true;
                smallestTime = cashiers.at(i).doneTime;
            }
        }

        for (int i = 0; i < baristas.size(); i++)
        {
            if (baristas.at(i).doneTime<smallestTime&&baristas.at(i).doneTime>clock)
            {
                baristasJobDone = true;
                smallestTime = baristas.at(i).doneTime;
            }
        }

        clock = smallestTime;

        for (int i = 6; i < orders.size(); i++)// if there is and order in that time it goes cashier or cashiers queue
        {
            if (orders.at(i).arrivalTime<smallestTime&&orders.at(i).section==0)
            {
                orders.at(i).doneTime += orders.at(i).arrivalTime;
                if(!goToCashier(orders.at(i)))
                {
                    
                    goToCashierQ(orders.at(i));
                    maxSizeCashierQ += 1;
                    orders.at(i).doneTime = smallestTime;
                    orders.at(i).section=1;
                }
            }
            
        }
        
        if (baristasJobDone)// If the smallest time is from barista I do requirements
        {

            for (int i = 0; i < baristas.size(); i++)
            {
                if (baristas.at(i).doneTime==smallestTime)
                {
                    baristas.at(i).isFree = true;
                    baristas.at(i).order.section = 5;
                    baristas.at(i).doneTime += baristas.at(i).order.doneTime;
                    if (baristaQ.max!=NULL)
                    {
                        Order ord = *baristaQ.max;
                        ord.doneTime = smallestTime;
                        goToBarista(ord);
                        baristaQ.Dequeue();
                    }
                    else{
                        baristas.at(i).isFree == true;
                    }
                }
            }
        }
        else if (cashierJobDone)
        {    

            for (int i = 0; i < cashiers.size(); i++)
            {
                if (cashiers.at(i).doneTime==smallestTime)
                {
                    cashiers.at(i).isFree = true;

                    if (!goToBarista(cashiers.at(i).order))
                    {
                        cashiers.at(i).order.section = 3;
                        Order *ord = new Order;
                        *ord = cashiers.at(i).order;
                        baristaQ.Enqueue(ord);
                    }
                    if (cashierQ.front!=NULL)
                    {
                        Order order = *cashierQ.front;
                        order.doneTime = smallestTime;
                        order.section = 2;  
                        order.doneTime += order.orderTime; 
                        cashiers.at(i).order=order;
                        cashiers.at(i).isFree=false;
                        cashiers.at(i).doneTime = order.doneTime;
                        cashiers.at(i).workedTime += order.orderTime;
                        cashierQ.Dequeue();
                    }
                }
            }
        }

        int tempMaxSize = 0;
        if (baristaQ.max!=NULL)// For find max size of queues
        {
            tempMaxSize+= 1;
            Order *iter = baristaQ.max;
            while (iter->next!=NULL)
            {
                tempMaxSize += 1;
                iter = iter->next;
            }
        }

        if (tempMaxSize>maxSizeBaristaQ)
        {
            maxSizeBaristaQ = tempMaxSize;
        }
        
        
        



        int fre = 0;
        for (int i = 0; i < baristas.size(); i++)// if all baristas is free and it's over
        {
            if (baristas.at(i).isFree&&baristas.at(i).workedTime!=0)
            {
                fre+=1;
            }
        }

        if (fre==baristas.size())
        {

            break;
        }
        

        
    } 
    vector <double> outpt;//Printing stuffs
    outpt.push_back(clock);
    outpt.push_back(maxSizeCashierQ);
    outpt.push_back(maxSizeBaristaQ);
    for (int i = 0; i < cashiers.size(); i++)
    {
        outpt.push_back(cashiers.at(i).workedTime/clock);
    }
    for (int i = 0; i < baristas.size(); i++)
    {
        outpt.push_back(baristas.at(i).workedTime/clock);
    }
    
    
    for (int i = 0; i < orders.size(); i++)
    {
        for (int j = 0; j < turnAround.size(); j+=2)
        {
           if (orders.at(i).coffePrice==turnAround.at(j))
           {
                outpt.push_back(turnAround.at(j+1)-orders.at(i).arrivalTime);
            
                break;
           }
           
        }
        
    }
    for (int i = 0; i < outpt.size(); i++)
    {
        if (i==1)
        {
            outputfile << maxSizeCashierQ << endl;
            
        }
        else if(i==2){
            outputfile << maxSizeBaristaQ << endl;
        }
        else{
            outputfile << setprecision(2) << fixed << outpt.at(i) << endl;
        }
     
    }
    
    outputfile << endl;
    for (int i = 0; i < 22; i++)
    {
        outputfile << "-1" << endl;
    }
    
    
    
}


int main(int argc, char* argv[]){ 

    string input = argv[1];
    string output = argv[2];
    Split(input,output);
    
    return 0;
}