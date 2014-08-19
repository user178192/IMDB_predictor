
#include <vector>
#include <string>

#include <Serializer.hpp>

using namespace imdb;


#include <iostream>
int main()
{
    /*
    vector<string> sb{"good father", "4 June 1989", "Pacino, Al"};
    Mov a{1,2, {1,2,3}, sb};
    auto *sz = new Serializer<false>("temp.dat");
    //sz.Write(sb);

    vector<Mov> vb;
    vb.push_back(a);
    a.fd[0] = "Inception 20102341";
    vb.push_back(a);
    sz->Write(vb);
    delete sz;


    vector<Mov> vb2;
    auto *rz = new Serializer<true>("temp.dat");
    rz->Read(vb2);
    cout << vb2[1].fd[0] << endl;

    delete rz;
    */


    auto *writer = new Serializer<false>("temp.dat");
    Movie a;
    a.rate_ = 7.77;
    vector<Movie> vb{a};
    a.rate_ = 7.777777;
    vb.push_back(a);
    writer->Write(vb);
    delete writer;



    auto *reader = new Serializer<true>("temp.dat");
    vector<Movie> vc;
    reader->Read(vc);
    cout << vc[1].rate_ << endl;
    delete reader;
}

