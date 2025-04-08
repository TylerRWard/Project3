#include <iostream>
#include <fstream>

using namespace std;

struct Client {
    string name;
    double survey1, survey2;
};
//for sorting
bool compareBySurvey1(const Client& a, const Client& b){return a.survey1<b.survey1;}
bool compareBySurvey2(const Client& a, const Client& b){return a.survey2<b.survey2;};
//1d
pair<pair<string, string>, double> findClosestPair1D(vector<Client>& clients, int surveyIdx){
    sort(clients.begin(), clients.end(), surveyIdx == 0 ? compareBySurvey1 : compareBySurvey2);
    double minDiff = numeric_limits<double>::max();
    pair<string, string> result;
    for (size_t i = 0; i < clients.size() -1; i++){
        double diff = (surveyIdx ==0)
            ? abs(clients[i+1].survey1 - clients[i].survey1)
            : abs (clients[i+1].survey2-clients[i].survey2);

        if(diff < minDiff){
            minDiff = diff;
            result = {clients[i].name, clients[i+1].name};
        }     
    }
    return {result, minDiff};
}
//2d
double distance (const Client& a, const Client& b){
    return sqrt(pow(a.survey1-b.survey1, 2)+ pow(a.survey2 - b.survey2, 2));
}
//small sets
pair<pair<string, string>, double> bruteForce(vector<Client>& clients){
    double minDist = numeric_limits<double>::max();
    pair<string, string> result;
    for(size_t i = 0; i < clients.size() -1; i++){
        for (size_t j = i +1; j < clients.size(); j++){
            double dist = distance(clients[i], clients[j]);
            if(dist <minDist){
                minDist = dist;
                result = {clients[i].name, clients[j].name};
            }
        }
    }
    return {result, minDist};

}

pair<pair<string, string>, double> closestPairRec(vector<Client>& clients){
    if (clients.size()<=3) return bruteForce(clients);

    size_t mid = clients.size()/2;
    double midX = clients[mid].survey1;

    vector<Client> left(clients.begin(), clients.begin() + mid);
    vector<Client> right(clients.begin() + mid, clients.end());

    auto [leftPair, leftDist] = closestPairRec(left);;
    auto [rightPair, rightDist] = closestPairRec(right);

    double minDist = min(leftDist, rightDist);
    pair<string, string> minPair = (leftDist < rightDist) ? leftPair : rightPair;


    //strip
    vector<Client> strip;
    for(const auto& client : clients){
        if(abs(client.survey1- midX) < minDist) strip.push_back(client);
    }
    sort(strip.begin(), strip.end(), compareBySurvey2);
    
    for(size_t i = 0; i < strip.size(); i++){
        for(size_t j = i + 1; j < min(i+8, strip.size()); j++){
            double dist = distance (strip[i], strip[j]);
            if (dist < minDist){
                minDist = dist;
                minPair = {strip[i].name, strip[j].name};
            }

        }

    }
    return {minPair, minDist};

}

pair<pair<string, string>, double> findClosestPair2D(vector<Client>& clients){
    sort(clients.begin(), clients.end(), compareBySurvey1);
    return closestPairRec(clients);
}

int main(int argc, char* argv[]){
    if(argc!= 3){
        cerr<< "Usage: " <<argv[0] <<" <input_file> <1|2|b>" << endl;
    }

    ifstream file(argv[1]);
    if(!file){
        cerr <<"cannot open file"<<argv[1] <<endl;
    }

    string mode = argv[2];
    vector<Client> clients;
    string name;
    double s1, s2;

    while (file >> name >> s1 >>s2){
        clients.push_back({name, s1, s2});
    }
    file.close();


        // debugging
    for (size_t i = 0; i < clients.size(); i++) {
        cout << "Client " << i + 1 << ": " 
            << clients[i].name << " " 
            << clients[i].survey1 << " " 
            << clients[i].survey2 << endl;
    }


    pair<pair<string, string>, double> result;
    if (mode =="1") result = findClosestPair1D(clients, 0);
    else if (mode =="2") result = findClosestPair1D(clients,1);
    else if (mode =="b") result = findClosestPair2D(clients);
    else{
        cerr<<"Invalid mode: "<<mode<<endl;
        return 1;
    }

    cout << result.first.first<<" " <<result.first.second<< " "<<fixed<<result.second<< endl;
    return 0;
}
 