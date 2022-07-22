#include <iostream>
#include <fstream>
#include <unistd.h>
#include <pthread.h>
#include "BQ.h"
#include "UBQ.h"

// create array for queue every producer wiil start his queue
//create a class BQ use the queue and add semaphore ....
using namespace std;

vector<BQ*> b ;
vector<BQ*> sm;
typedef struct _param {
    int number;
    int newsnum;
    int buffsize;
} prod;

void*  producer(void *arg){
    //create producer
    prod* args = (prod *) arg;
    int news = args->newsnum;
    int produserid = args->number;
    int rcount = 0;
    int sportscount = 0;
    int weathercount = 0;
    string data;
    for (int i = 0; i < news; ++i) {
        if(i%3==0){
            data = "Prouducer " + to_string(produserid) + " SPORTS " + to_string(sportscount);
            sportscount++;
            b[args->number -1]->enqueue(data);
        }
        if(i%3==1){
            data = "Prouducer " + to_string(produserid) + " WEATHER " + to_string(weathercount);
            weathercount++;
            b[args->number -1]->enqueue(data);
        }
        if(i%3==2){
            data = "Prouducer " + to_string(produserid) + " NEWS " + to_string(rcount);
            rcount++;
            b[args->number -1]->enqueue(data);
        }
    }
    b[args->number -1]->enqueue("DONE");
    return ((void *)0);
}
UBQ S = UBQ();
UBQ N = UBQ();
UBQ W = UBQ();
void*  Dispatcher(void *arg){
    bool x = true;
    string data;
    while (x){
        x =false;
        for (int i = 0; i < b.size(); ++i) {
            if(b[i] != NULL) {
                data = b[i]->dequeue();
                if(data == "DONE"){
                    b[i] = NULL;
                } else if(data.find("SPORTS") != string::npos){
                    S.enqueue(data);
                } else if(data.find("WEATHER") != string::npos){
                    W.enqueue(data);
                } else if(data.find("NEWS") != string::npos){
                    N.enqueue(data);
                }
                x = true; //if all the producers are null the code can't get to this point so it gonna stop
            }
        }
    }
    N.enqueue("DONE");
    W.enqueue("DONE");
    S.enqueue("DONE");
}

void*  sCoEditor(void *arg){
    string data = S.dequeue();
    while (data != "DONE") {
        sleep(static_cast<unsigned int>(0.1));
        sm[0]->enqueue(data);
        data = S.dequeue();
    }
    sm[0]->enqueue("DONE");
}

void*  nCoEditor(void *arg){
    string data = N.dequeue();
    while (data != "DONE") {
        sleep(static_cast<unsigned int>(0.1));
        sm[0]->enqueue(data);
        data = N.dequeue();
    }
    sm[0]->enqueue("DONE");
}

void*  wCoEditor(void *arg){
    string data = W.dequeue();
    while (data != "DONE") {
        sleep(static_cast<unsigned int>(0.1));
        sm[0]->enqueue(data);
        data = W.dequeue();
    }
    sm[0]->enqueue("DONE");
}
void*  screenM(void *arg) {
    int i = 0;
    while (i != 3) {
       // sleep(static_cast<unsigned int>(0.1));
        string s = sm[0]->dequeue();
        if (s != "DONE") {
            std::cout << s << std::endl;
        } else {
            i++;
        }
    }
    std::cout << "DONE" << std::endl;
}

int main(int argc, char**argv) {
    std::string myText;
    std::ifstream infile;
    infile.open(argv[1], std::ios::in);
    int z =0;

// Use a while loop together with the getline() function to read the file line by line
//
    while (getline (infile, myText)) {
        // Output the text from the file
        if (myText != "")
            z++;
        std::cout << myText + "\n";
    }
    infile.close();
    infile.open(argv[1], std::ios::in);
    prod prods[z/3];
    int x=0;
    int count =0;
    int cosize=0;
    string text;
    while (getline (infile, text)) {
        // Output the text from the file
        if((count == z/3) && (text != "") ){
            cosize = stoi(text);
        }
        else if (text != "") {
            if(x==0)
                prods[count].number = stoi(text);
            if (x==1)
                prods[count].newsnum = stoi(text);
            if(x==2) {
                prods[count].buffsize = stoi(text);
                count++;
            }
            x++;
            x =  (x%3);
        }

        std::cout << text + "\n";
    }
    infile.close();

    pthread_t ntid[z/3];
    for (int i = 1; i <= z/3; ++i) {

        int err;
        b.push_back(new BQ(prods[i-1].buffsize));
        err =pthread_create(&ntid[i-1], NULL, producer, (void *)&prods[i-1]);
        if (err != 0)
            std::cout << "f";

    }
    int err;
    pthread_t disid;
    err =pthread_create(&disid, NULL, Dispatcher, NULL);

    pthread_t Wco;
    pthread_t Nco;
    pthread_t Sco;
   // BQ editorQ= BQ(cosize);
    sm.push_back(new BQ(cosize));
    int wco;
    wco =pthread_create(&Wco, NULL, wCoEditor, NULL);
    int nco;
    nco =pthread_create(&Nco, NULL, nCoEditor, NULL);
    int sco;
    sco =pthread_create(&Sco, NULL, sCoEditor, NULL);
    pthread_t Scr;
    int screen;
    screen =pthread_create(&Scr, NULL, screenM, NULL);

   // pthread_join(disid, NULL);
    pthread_join(Scr, NULL);

    return 0;
}